#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/param.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <errno.h>

#include <newds/base.h>
#include <newds/red_black.h>
#include <newds/list.h>

#include "string_command.h"
#include "history.h"
#include "jobs.h"
#include "builtin.h"

static void print_prompt(void) {
    char *login = getlogin();
    char wd[BUFSIZ];
	char absolute_home[BUFSIZ];
    char hostname[BUFSIZ];

	realpath(getenv("HOME"), absolute_home);
    getcwd(wd, BUFSIZ);

	if (strcmp(wd, absolute_home) == 0) {
		strcpy(wd, "~");
	}

    gethostname(hostname, BUFSIZ);
    printf("%s@%s: %s %c ", login, hostname, wd, getuid() == 0 ? '#' : '$');
}


static job_t *
job_from_command_line(const char *command_line, char *envp[])
{
    job_t *job;

    char *work_copy, *history_copy;
    char *cmds[MAX_TOKEN];
    int n_cmds;

	int argc;
	char *argv_buf[MAX_TOKEN];

	int i;

    char *tmp_string;

    work_copy = strdup(string_command_trim_start(command_line));
	string_command_trim_end(work_copy);

    n_cmds = string_command_tokenize(work_copy, cmds, "|");

    /*
	printf("n_cmds = %d\n", n_cmds);
    */

	if (n_cmds == 0) {
		return NULL;
	}

	history_copy = strdup(string_command_trim_start(command_line));
    string_command_trim_end(history_copy);
    history_add(history_copy);
    /*
    printf("HISTORYCOPY = %s\n", history_copy);
    */

    if (n_cmds == 1) {
        char *builtin_copy = strdup(cmds[0]);
        boolean is_builtin;

        argc = string_command_tokenize(builtin_copy, argv_buf, " \t");

        is_builtin = builtin_handle(argc, argv_buf, envp);

        free(builtin_copy);

        if (is_builtin)
            return NULL;
    }

	/* TODO: store in the history, and free */
    job = job_new(history_copy);

	job_set_bg(job, string_command_bg(work_copy));


	tmp_string = string_command_write_to(cmds[n_cmds - 1]);
	if (tmp_string) {
        tmp_string = string_command_trim(tmp_string);
        job_set_stdout(job, tmp_string);
	}

	tmp_string = string_command_read_from(cmds[0]);
    if (tmp_string) {
        tmp_string = string_command_trim(tmp_string);
        job_set_stdin(job, tmp_string);
    }

	for (i = 0; i < n_cmds; i++) {
		argc = string_command_tokenize(cmds[i], argv_buf, " \t");

		job_add_program(job, argv_buf[0], argc, argv_buf, envp);

	}

	free(work_copy);

	return job;
}


static void sigint_handler(int sig) {
    /*
    printf("CTRL-C %d\n", sig);
    */
}

void job_run_command(const char *command, char *envp[]) {
    job_t *job = job_from_command_line(command, envp);

    if (job) {
		job_run(job);

            /*
            printf("running on %d\n", job->bg);
            */

	}
}

int
main(int argc, char *argv[], char *envp[])
{
	char buff[BUFSIZ];
    int rc;

    signal(SIGTTOU, SIG_IGN);
    signal(SIGINT, sigint_handler);

    rc = setpgid(0, 0);
    assert(rc != -1);


    builtin_init();
    history_init();

    job_init();

    
    print_prompt();
	while (fgets(buff, BUFSIZ, stdin)) {
		job_run_command(buff, envp);

        job_wait();

        print_prompt();
	}

    job_finalize();

    history_finalize();
    builtin_finalize();

    return 0;
}
