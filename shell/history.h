#ifndef HISTORY_H
#define HISTORY_H

void history_init(void);

void history_add(char *command_line);

void history_print(void);

void history_finalize(void);

#endif /* HISTORY_H */
