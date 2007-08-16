#include "alias.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <newds/base.h>
#include <newds/red_black.h>

#include "newshell.h"

static red_black_t *aliases;

typedef struct {
    char *cmd;
    char *alias;
} alias_t;

static int
alias_cmp(const void *a, const void *b)
{
    const alias_t *a0 = a;
    const alias_t *a1 = b;

    return strcmp(a0->cmd, a1->cmd);
}

static void
alias_delete(void *alias)
{
    alias_t *self = alias;

    free(self->cmd);
    free(self->alias);
    free(self);
}

static alias_t *
alias_find(const char *cmd)
{
	alias_t dummy_alias;

	dummy_alias.cmd = (char *) cmd;

	return red_black_find(aliases, &dummy_alias);
}

static void
alias_add(const char *cmd, char *alias)
{
    alias_t *self = alias_find(cmd);
	
	if (self == NULL) {
		self = malloc(sizeof(alias_t));
    	assert(self);

    	self->cmd = strdup(cmd);
    	red_black_insert(aliases, self);
	}

	self->alias = alias;
}

void
alias_init(void)
{
    aliases = red_black_new_full(alias_cmp, alias_delete, TRUE);
}

void
alias_finalize(void)
{
    red_black_delete(aliases);
    aliases = NULL;
}

void
alias_set(const char *cmd, char *alias[]) {
    char **p;
    int cmd_len = 0;
    char *result;

    for (p = alias; *p; p++) {
        cmd_len += strlen(*p) + 1;
    }

    result = malloc(cmd_len + 1);
    assert(result);

    *result = 0;
    for (p = alias; *p; p++) {
        strcat(result, *p);
        strcat(result, " ");
    }

    alias_add(cmd, result);
}

static char *
alias_mount_command(const char *alias_cmd, char *argv[])
{
	char *result, **arg;
	size_t len = strlen(alias_cmd);

	for (arg = argv; *arg != NULL; arg++) {
		len += 1 + strlen(*arg);
	}

	result = malloc(len + 1);
	strcpy(result, alias_cmd);

	for (arg = argv; *arg != NULL; arg++) {
		strcat(result, " ");
		strcat(result, *arg);
	}

	return result;
}


boolean
alias_handle(int argc, char *argv[], char *envp[])
{
	static char *alias_cmd_line = NULL;

	alias_t *alias = alias_find(argv[0]);

	if (alias == NULL || alias_cmd_line != NULL) {
		return FALSE;
	}


	alias_cmd_line = alias_mount_command(alias->alias, argv + 1);
	job_run_command(alias_cmd_line, envp);
	free(alias_cmd_line);
	alias_cmd_line = NULL;

    return TRUE;
}
