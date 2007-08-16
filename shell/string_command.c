#include "string_command.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

static boolean
char_is_one_of(char ch, const char *chs) {
    while (*chs) {
        if (ch == *chs) {
            return TRUE;
        }
        chs++;
    }

    return FALSE;
}

int
string_command_tokenize(char *cmd_line, char *args[MAX_TOKEN],
        const char *sep)
{
    char *p;
    int n_tokens = 0;

    for (p = strtok(cmd_line, sep); p != NULL; p = strtok(NULL, sep)) {
        assert(n_tokens < MAX_TOKEN);
        args[n_tokens++] = p;
    }

    args[n_tokens] = NULL;

    return n_tokens;
}
char *
string_command_read_from(char *str) {
	char *p;

	p = strchr(str, '<');

	if (p) {
		*p = '\0';
		return p + 1;
	}

	return NULL;
}
char *
string_command_write_to(char *str) {
	char *p;

	p = strchr(str, '>');

	if (p) {
		*p = '\0';
		return p + 1;
	}

	return NULL;
}

char *
string_command_trim(char *string) {
    string = (char *) string_command_trim_start(string);
    string_command_trim_end(string);

    return string;
}

const char *
string_command_trim_start(const char *string)
{
	while (*string && char_is_one_of(*string, " \t\n\r"))
		string++;


	return string;
}

void string_command_trim_end(char *string) {
	char *p;

	for (p = string + strlen(string) - 1; p > string
			&& char_is_one_of(*p, " \t\n\r"); p--);

	*(p + 1)  = '\0';
}

boolean
string_command_bg(char *string) {
	char *p;

	p = string + strlen(string) - 1;

	if (*p == '&') {
		*p = '\0';
		return TRUE;
	}

	return FALSE;
}
