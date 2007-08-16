#include "history.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char *history[100];
int history_position = 0;
int history_start = 0;

void history_init(void) {
    int i;

    for (i = 0; i < 100; i++) {
        history[i] = NULL;
    }
}

void history_add(char *command_line) {
    if (history[history_position])
        free(history[history_position]);

    history[history_position] = command_line;

    history_position = (history_position + 1) % 100;

    if (history_position == history_start)
        history_start = (history_start + 1) % 100;
}

void history_print(void) {
    int p, i;
    i = 0;
    for (p = history_start; history[p] && ((p + 1) % 100 != history_start);
            p++) {
        printf("%d %s\n", i++, history[p]);
    }
}

void history_finalize(void) {
    char buf[BUFSIZ];
    FILE *fp;
    int p;

    strncpy(buf, getenv("HOME"), BUFSIZ);
    strncat(buf, "/.newshell_history", BUFSIZ);

    if ((fp = fopen(buf, "a")) == NULL) {
        return;
    }

    for (p = history_start; history[p] && ((p + 1) % 100 != history_start);
            p++) {
        fprintf(fp, "%s\n", history[p]);
    }

    fclose(fp);
}
