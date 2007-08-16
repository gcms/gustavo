#ifndef BUILTIN_H
#define BUILTIN_H

#include "string_command.h"

void
builtin_init(void);

boolean
builtin_handle(int argc, char *argv[MAX_TOKEN], char *envp[]);

void
builtin_finalize(void);


#endif /* BUILTIN_H */
