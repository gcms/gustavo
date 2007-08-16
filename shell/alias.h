#ifndef ALIAS_H
#define ALIAS_H

#include <newds/base.h>

void
alias_init(void);

void
alias_set(const char *alias_cmd, char *alias[]);

boolean
alias_handle(int argc, char *argv[], char *envp[]);

void
alias_finalize(void);

#endif /* ALIAS_H */
