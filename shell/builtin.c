#include "builtin.h"

#include "jobs.h"
#include "history.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <newds/base.h>
#include <newds/red_black.h>

typedef void (*builtin_func_t)(int argc, char *argv[]);

typedef struct {
    const char *command_name;
    builtin_func_t builtin_func;
} builtin_t;

static int
builtin_cmp(const void *a, const void *b) {
    const builtin_t *b0, *b1;

    b0 = a;
    b1 = b;

    return strcmp(b0->command_name, b1->command_name);
}

static red_black_t *builtin_functions = NULL;

static void
builtin_add(const char *command_name, builtin_func_t builtin_func) {
    builtin_t *self = malloc(sizeof(builtin_t));
    assert(self);

    self->command_name = command_name;
    self->builtin_func = builtin_func;

    red_black_insert(builtin_functions, self);
}

static void
jobs_handler(int argc, char *argv[]) {
    job_jobs();
}

static void
fg_handler(int argc, char *argv[]) {
    int jid;

    if (argc > 1)
        jid = atoi(argv[1]);
    else
        jid = -1;

    job_fg(jid);
}

static void
bg_handler(int argc, char *argv[]) {
    int jid;

    if (argc > 1)
        jid = atoi(argv[1]);
    else
        jid = -1;

    job_bg(jid);
}

static void
history_handler(int argc, char *argv[]) {
    history_print();
}

static void
cd_handler(int argc, char *argv[]) {
	static char last_dir[BUFSIZ];
	static boolean iniciado = FALSE;

    if (argc < 2) {
		getcwd(last_dir, BUFSIZ);
		iniciado = TRUE;

        chdir(getenv("HOME"));
    } else if (strcmp(argv[1], "-") == 0 && iniciado) {
		chdir(last_dir);
    } else {
		getcwd(last_dir, BUFSIZ);
		iniciado = TRUE;

        chdir(argv[1]);
    }
}

#include <sys/stat.h>
#include <limits.h>

static void
umask_handler(int argc, char *argv[]) {
    mode_t result;
    if (argc < 2) {
        result = umask(0022);
        umask(result);
    } else {
        mode_t mask = strtoul(argv[1], NULL, 0);
        result = umask(mask);
    }

    printf("0%03o\n", result);
}

#include "alias.h"

static void
alias_handler(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "wrong number of arguments\n");
        return;
    }
    alias_set(argv[1], argv + 2);
}

void
builtin_init(void)
{
    builtin_functions = red_black_new_full(builtin_cmp, free, TRUE);
    builtin_add("jobs", jobs_handler);
    builtin_add("fg", fg_handler);
    builtin_add("bg", bg_handler);
    builtin_add("history", history_handler);
    builtin_add("cd", cd_handler);
    builtin_add("umask", umask_handler);
    
    alias_init();
    builtin_add("alias", alias_handler);

    /* umask_init() */
    umask(0022);
}

boolean
builtin_handle(int argc, char *argv[MAX_TOKEN], char *envp[])
{
    builtin_t dummy, *builtin;
    static int inside = 0;
    inside++;

    dummy.command_name = argv[0];
    builtin = red_black_find(builtin_functions, &dummy);

    if (builtin) {
        builtin->builtin_func(argc, argv);
        return TRUE;
    }

    return alias_handle(argc, argv, envp);
}

void
builtin_finalize(void) {
    red_black_delete(builtin_functions);
    builtin_functions = NULL;

    alias_finalize();
}
