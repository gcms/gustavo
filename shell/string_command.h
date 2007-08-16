#ifndef STRING_COMMAND_H
#define STRING_COMMAND_H

#include <newds/base.h>

#define MAX_TOKEN 256

int
string_command_tokenize(char *cmd_line, char *args[MAX_TOKEN],
        const char *sep);

char *
string_command_read_from(char *string);

char *
string_command_write_to(char *string);

char *
string_command_trim(char *string);

const char *
string_command_trim_start(const char *string);

void
string_command_trim_end(char *string);

boolean
string_command_bg(char *string);


#endif /* STRING_COMMAND_H */
