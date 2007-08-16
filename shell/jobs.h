#ifndef JOBS_H
#define JOBS_H

#include <newds/base.h>
#include <sys/types.h>

typedef struct job job_t;
typedef struct job_node job_node_t;


void
job_init(void);

void
job_finalize(void);

job_t *
job_new(const char *cmd_line);

void
job_set_stdin(job_t *self, const char *filename);

void
job_set_stdout(job_t *self, const char *filename);

void
job_set_stderr(job_t *self, const char *filename);

void
job_set_bg(job_t *self, boolean bg);

void
job_add_program(job_t *self, const char *filename,
                int argc, char *argv[], char *envp[]);
void
job_run(job_t *self);

void
job_wait(void);

void
job_node_delete(job_node_t *node);

void
job_delete(job_t *self);

/* class functions */
void
job_fg(int jid);

void
job_bg(int jid);

void
job_jobs(void);

#endif /* JOBS_H */
