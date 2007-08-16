#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <errno.h>

#include <newds/base.h>
#include <newds/red_black.h>
#include <newds/list.h>

#include "jobs.h"

red_black_t *processes;
list_t *jobs;

job_t *fg_job;


struct job_node {
    char *path;

    int argc;
    char **argv;
    char **envp;

	int status;
    pid_t pid;

	job_t *job;
    job_node_t *next;
};

struct job {
    int jid;

    job_node_t *head;

	const char *cmd_line;

    char *read_from;
    char *write_to;
    char *write_error;

    int n_progs;
    pid_t pgid;

    boolean bg;
    boolean stopped;
};

static int
job_cmp(const void *a, const void *b) {
    const job_t *j0, *j1;

    j0 = a;
    j1 = b;

    assert(j0->jid != -1);
    assert(j1->jid != -1);

    return j0->jid - j1->jid;
}

static int
job_node_cmp(const void *a, const void *b) {
	const job_node_t *j0, *j1;

	j0 = a;
	j1 = b;

	assert(j0->pid != -1);
	assert(j1->pid != -1);

	return j0->pid - j1->pid;
}
void
job_init(void) {
    fg_job = NULL;
	processes = red_black_new_full(job_node_cmp, NULL, TRUE);
    jobs = list_new_full(job_cmp, NULL);
}


job_t *
job_new(const char *cmd_line)
{
    static int jid = 0;

    job_t *self = malloc(sizeof(job_t));

    self->jid = ++jid;

	self->cmd_line = cmd_line;

    self->head = NULL;
    self->read_from = NULL;
    self->write_to = NULL;
    self->write_error = NULL;

    self->n_progs = 0;
    self->pgid = -1;

    self->bg = FALSE;
    self->stopped = FALSE;

    return self;
}

void
job_set_stdin(job_t *self, const char *filename)
{
    self->read_from = strdup(filename);
}

void
job_set_stdout(job_t *self, const char *filename)
{
    self->write_to = strdup(filename);
}

void
job_set_stderr(job_t *self, const char *filename)
{
    self->write_error = strdup(filename);
}

void
job_set_bg(job_t *self, boolean bg) {
    self->bg = bg;
}

void
job_add_program(job_t *self, const char *filename,
                int argc, char *argv[], char *envp[])
{
    job_node_t *node = malloc(sizeof(job_node_t));

    /*
	printf("creating node: argc = %d\n", argc);
    */

    node->path = strdup(filename);
    node->argc = argc;

    node->argv = malloc(sizeof(char *) * (argc + 1));
    node->argv[argc] = NULL;
    while (argc-- > 0) {
        node->argv[argc] = strdup(argv[argc]);
    }
    
    node->envp = envp;

	node->status = -1;
    node->pid = -1;

	node->job = self;

    node->next = self->head;
    self->head = node;

    self->n_progs++;
}

void
job_run(job_t *self)
{
    job_node_t *n;
    int n_pipes;
    int *fildes;
    int i;

    int rc;

    n_pipes = self->n_progs - 1;

    fildes = malloc(sizeof(int) * 2 * n_pipes);

    for (i = 0; i < n_pipes; i++) {
        pipe(&fildes[i * 2]);
    }

    for (n = self->head, i = 0; n != NULL; n = n->next, i++) {
        int child;
        int close_p = 0;

        assert(i < self->n_progs);
        assert(child != -1);

		
        /*
		printf("%s is %d\n", n->path, i);
        */

        child = fork();
        if (child == 0) {
            if (self->pgid == -1) {
                rc = setpgid(getpid(), 0);
                assert(rc != -1);

                if (!self->bg) {
                    rc = tcsetpgrp(0, getpgid(0));
                    assert(rc != -1);
                }
            } else {
                rc = setpgid(0, self->pgid);
                assert(rc != -1);
            }
			
            if (self->head == n && self->write_to != NULL) {
                assert(i == 0);
                close(1);
                rc = open(self->write_to, O_CREAT | O_WRONLY);
                if (rc == -1)
                    perror("open");
            }
            if (n == self->head && self->write_error) {
                assert(i == 0);
                close(2);
                rc = open(self->write_error, O_CREAT | O_WRONLY);
                if (rc == -1)
                    perror("open");
            }
            if (n->next == NULL && self->read_from) {
                assert(i == self->n_progs - 1);
                close(0);
                rc = open(self->read_from, O_RDONLY);
                if (rc == -1)
                    perror("open");
            }

            if (i > 0) {
                close(1);
                dup(fildes[(i - 1) * 2 + 1]);
                while (close_p < (i - 1) * 2 + 1) {
                    close(fildes[close_p++]);
                }
                close_p++;
            }
            if (i < self->n_progs - 1) {
                close(0);

                dup(fildes[i * 2]);
                while (close_p < i * 2) {
                    close(fildes[close_p++]);
                }
                close_p++;
                while (close_p < 2 * n_pipes) {
                    close(fildes[close_p++]);
                }
            }

            free(fildes);
 
			/* execve(n->path, n->argv, n->envp); */
            /*
            printf("running %s\n", n->path);
            printf("n->path = %s\n", n->path);
            char **arg = n->argv;
            while (*arg) {
                printf("%s\n", *arg);
                arg++;
            }
            */
            
            execvp(n->path, n->argv);
            fprintf(stderr, "%d: execvp: %s\n", errno, strerror(errno));
            exit(0);
            break;
        } else {
            /* parent */
            if (self->pgid == -1) {
                self->pgid = child;
            }

            n->pid = child;
        }
    }

    for (i = 0; i < 2 * n_pipes; i++) {
        close(fildes[i]);
    }
    free(fildes);

	/*
    assert(self->pgid != -1);
    if (!self->bg) {
        tcsetpgrp(0, self->pgid);
        for (n = self->head; n != NULL; n = n->next) {
            int status;
            waitpid(n->pid, &status, 0);
            printf("%d exited with %p\n", n->pid, (void *) status);
        }
        tcsetpgrp(0, getpgid(0));
    }
	*/

    assert(self->pgid != -1);
    if (!self->bg) {
        assert(fg_job == NULL);
        fg_job = self;

        /*
        printf("tcgetpgrp(0) = %d\n", tcgetpgrp(0));
        printf("tcsetpgrp(0, %d)\n", self->pgid);
        printf("%d\n", self->head->pid);
        */


         /*
         assert(isatty(0));
         rc = tcsetpgrp(0, job->pgid);
         if (rc == -1) {
             perror("tcsetpgrp");
         }
         assert(rc != -1);
         */
    } else {
        printf("[%d] %d\n", self->jid, self->pgid);
	    fg_job = NULL;
    }

	for (n = self->head; n; n = n->next) {
        /*
	    printf("INSERTING %d\n", n->pid);
        */
    	red_black_insert(processes, n);
    }

    list_prepend(jobs, self);
}

void
job_wait(void)
{
    int rc;

    while (1) {
	    pid_t pid;
		int status, flags;
		job_node_t dummy, *proc_node, *n;

		if (fg_job) {
		    flags = WUNTRACED | WCONTINUED;
        } else {
			flags = WNOHANG | WUNTRACED | WCONTINUED;
        }

		pid = waitpid(-1, &status, flags);

		if (pid == -1) {
		    break;
		}

		if (pid == 0) {
			break;
        }
			
		dummy.pid = pid;

        /*
		printf("LOOKING for %d\n", pid);
        */
		proc_node = red_black_find(processes, &dummy);
		assert(proc_node);
		proc_node->status = status;
#define STATUS_EXITED(s) (WIFEXITED(s) || (WIFSIGNALED(s) \
            && WTERMSIG(s) != SIGSTOP && !WIFCONTINUED(s)))


        /*
        printf("WIFEXITED? %s\n", WIFEXITED(status) ? "YES" : "NO");
        printf("WIFSIGNALED? %s\n", WIFSIGNALED(status) ? "YES" : "NO");
        printf("WTERMSIG = %d\n", WTERMSIG(status));
        printf("WCONTINUED? = %s\n", WIFCONTINUED(status) ? "YES" : "NO");
        */

		if (STATUS_EXITED(status)) {
		    for (n = proc_node->job->head; n != NULL; n = n->next) {
                /*
                printf("%s %p %s\n", n->path, (void *) n->status,
                        STATUS_EXITED(n->status) ? "YES" : "NO");
                        */
                if (!STATUS_EXITED(n->status)) {
				    break;
                }
            }

			if (n == NULL) {
    			if (proc_node->job == fg_job)
	    			fg_job = NULL;
                else
    	    		printf("[%d] Exited %s\n",
	    						proc_node->job->jid, proc_node->job->cmd_line);

                /* Remove processes of job from process map */
                for (n = proc_node->job->head; n; n = n->next) {
                    /*
                    printf("REMOVING %d\n", n->pid);
                    */
                    red_black_remove(processes, n);
                }

                /* Remove job from job list */
                assert(list_remove(jobs, proc_node->job));
				job_delete(proc_node->job);
			}

			continue;
		}

		if (WIFSTOPPED(status)) {
		    if (proc_node->job == fg_job) {
                /*
                printf("tcsetpgrp(0, %d)\n", getpgid(0));
                */

                assert(isatty(0));
				rc = tcsetpgrp(0, getpgid(0));
                assert(rc != -1);

    			printf("[%d] STOPPED\t%s\n", fg_job->jid, fg_job->cmd_line);
	
                fg_job = NULL;
			}

            proc_node->job->stopped = TRUE;

            continue;
        }

        if (WIFCONTINUED(status)) {
            for (n = proc_node->job->head; n; n = n->next) {
                if (WIFSTOPPED(n->status))
                    break;
            }

            if (n == NULL) {
                proc_node->job->stopped = FALSE;
            }

            continue;
        }
            
    }

    /*
    printf("tcsetpgrp(0, %d)\n", getpgid(0));
    */
    assert(isatty(0));
    rc = tcsetpgrp(0, getpgid(0));
    assert(rc != -1);
}

void
job_node_delete(job_node_t *node)
{
	while (node->argc--) {
		free(node->argv[node->argc]);
	}
	free(node->argv);

	free(node->path);
	free(node);
}

void
job_delete(job_t *self)
{
    job_node_t *n, *next;

    for (n = self->head; n != NULL; n = next) {
        next = n->next;
        job_node_delete(n);
    }

	if (self->read_from)
		free(self->read_from);

	if (self->write_to)
		free(self->write_to);

	if (self->write_error)
		free(self->write_error);

    free(self);
}

static void
continue_job(job_t *job) {
    if (job->stopped) {
        job_node_t *n;

        for (n = job->head; n; n = n->next) {
            if (WIFSTOPPED(n->status)) {
                kill(n->pid, SIGCONT);
            }
        }
    }
}

static job_t *
find_job(int jid) {
    job_t dummy;

    return_val_if_fail(!list_empty(jobs), NULL);

    if (jid == -1)
        return list_first(jobs);

    dummy.jid = jid;

    return list_find(jobs, &dummy);
}

void
job_fg(int jid) {
    int rc;
    if (list_empty(jobs)) {
        puts("No jobs.");
    } else {
        assert(fg_job == NULL);
        
        fg_job = find_job(jid);
        if (fg_job == NULL) {
            printf("Invalid job number.\n");
            return;
        }
        fg_job->bg = FALSE;

        /*        
        printf("tcsetpgrp(0, %d)\n", fg_job->pgid);
        */

        assert(isatty(0));
        rc = tcsetpgrp(0, fg_job->pgid);
        assert(rc != -1);
      
        continue_job(fg_job);  
    }
}

void
job_bg(int jid) {
    if (list_empty(jobs)) {
        puts("No jobs.");
    } else {
        job_t *j = find_job(jid);
        assert(fg_job == NULL);
        
        if (j == NULL) {
            printf("Invalid job number.\n");
            return;
        }

        continue_job(j);
    }
}

void
job_jobs(void) {
    iterator_t *itr;

    for (itr = list_iterator(jobs); iterator_has_more(itr);
            iterator_next(itr)) {
        job_t *j = iterator_get(itr);
        printf("[%d] ", j->jid);

        if (j->stopped)
            printf("STOPPED\t");

        printf("%s\n", j->cmd_line);
    }

    iterator_delete(itr);
}


boolean
job_iterate(void *value, void *user_data) {
    job_delete((job_t *) value);
    return TRUE;
}

void
job_finalize(void) {
    red_black_delete(processes);
    processes = NULL;
    list_iterate(jobs, job_iterate, NULL);
    list_delete(jobs);
    jobs = NULL;
}
