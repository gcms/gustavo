#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <limits.h>

#include "iterator.h"
#include "fileutil.h"
#include "list.h"
#include "hashtable.h"

#define WORD_SEPARATORS " \n\t\r.,;:!?"
#define LINE_SEPARATORS "\n"

#define getword(fp) (get_string_token(fp, WORD_SEPARATORS))
#define getline(fp) (get_string_token(fp, LINE_SEPARATORS))

static unsigned int word_no;
static unsigned int key_no;
static unsigned int colisions;
static unsigned int word_size = UINT_MAX;


/**
 * Strips the leading '#' from strings.
 * @param str string with leading '#'s
 * @return a pointer to the first character different of '#'
 */
static const char *strip(const char *str) {
    while (*str == '#') str++;

    return str;
}

#include <math.h>
/**
 * Hashes a character string key.
 * @param key key to be hashed
 * @return hash value
 */
static unsigned int wordhash(const void *key) {
    size_t i, len;
    const char *str = strip((const char *) key);
    unsigned int hash = 0;

    len = strlen(str);

    /* use only the first word_size characters of the key */
    len = len < word_size ? len : word_size;

    hash = 0;
    
    for (i = 0; i < len; i++)
        hash += str[i] * pow(31, len - i - 1);

    return hash;
}


/**
 * Compares two character strings.
 * @param s1 string1
 * @param s2 string2
 * @return a negative number if s1 comes before s2 in the lexicographic order,
 * zero if both are equal, and a positive number of s1 is after s2.
 */
static int wordcmp(const char *s1, const char *s2) {
    unsigned int i;

    /* only the first word_size characters need to be checked */
    for (i = 0; i < word_size && s1[i] != 0 && s2[i] != 0; i++) {
        if (s1[i] - s2[i] != 0) {
            return s1[i] - s2[i];
        }
    }

    return 0;
}

/**
 * Checks the first word_size characters of two strings for equality.
 * @param a string1
 * @param b string2
 * @return true if the first word_size characters of a and b are equal,
 * false otherwise
 */
static bool wordequals(const void *a, const void *b) {
    /* strip the leading '#' from the strings */
    return wordcmp(strip((const char *) a), strip((const char *) b)) == 0;
}

/**
 * Compares two entry_t structures.
 * @param a entry1
 * @param b entry2
 * @return @see wordcmp
 */
static int entrycmp(const void *a, const void *b) {
    const entry_t *e1 = a;
    const entry_t *e2 = b;

    return wordcmp(e1->key, e2->key);
}


/**
 * Load symbols from a file to a table. Symbols are defined as character
 * strings found in the file starting with a leading '#'. For each symbol
 * loaded a new list is created as a value associated to the given symbol.
 * @param table table to which the symbols will be loaded
 * @param fp file from which the symbols will be loaded
 */
static void load_symbols(hash_table_t *table, FILE *fp) {
    char *word;

    /* read the words marked with '#' at start */
    while ((word = getword(fp)) != NULL) {
        word_no++;      /* statistics */

        if (word[0] == '#' && !hash_table_contains(table, word)) {
            /* use only the first word_size characters */
            char *str = (char *) strip(word);
            size_t len = strlen(str);
            len = len < word_size ? len : word_size;
            str[len] = 0;

            key_no++;   /* statistics */

            hash_table_put(table, word, list_new(NULL));
        } else {
            free(word);
        }
    }
}

/**
 * Load words from a file and add the line number to the list associated with
 * each word found in the table.
 * @param table table that will be filled
 * @param fp file with the words
 */
static void fill_table(hash_table_t *table, FILE *fp) {
    unsigned int line_no;
    char *line, *word;

    line_no = 0;    /* mark the line number */
    while ((line = getline(fp)) != NULL) {
        word = strtok(line, WORD_SEPARATORS);

        line_no++;

        while (word != NULL) {
            void *list;

            if ((list = hash_table_get(table, word)) != NULL) {
                list_append(list, (void *) line_no);
            }

            word = strtok(NULL, WORD_SEPARATORS);
        }

        free(line);
    }

}


int main(int argc, char *argv[]) {
    FILE *fp;
    unsigned int i;
    size_t nelem;
    entry_t *e;
    hash_table_t *table;

    if (argc < 2) {
        fprintf(stderr, "usage: %s <filename> [ word size ]\n", argv[0]);
        return 1;
    }

    if (argc >= 3) {
        long size =  strtol(argv[2], NULL, 0);
        word_size = size == 0 ? word_size : size;
    }

    if ((fp = fopen(argv[1], "r")) == NULL) {
        perror("fopen");
        return -1;
    }
    
    table = hash_table_new(wordhash, wordequals);

    word_no = 0;    /* file variables */
    key_no = 0;

    /* load_symbols() alter word_no and key_no */
    load_symbols(table, fp);
    
    fseek(fp, 0, SEEK_SET);

    fill_table(table, fp);

    e = hash_table_entries(table);
    nelem = hash_table_size(table);
    colisions = hash_table_colisions(table);

    hash_table_delete(table, NULL, NULL);

    qsort(e, nelem, sizeof(entry_t), entrycmp);

    for (i = 0; i < nelem; i++) {
        void *it;

        printf("%20s\t", strip((char *) e[i].key));

        for (it = list_iterator(e[i].value); iterator_has_more(it);
                iterator_next(it)) {
            printf("%3d ", (int) iterator_get(it));
        }
        puts("");

        free(e[i].key);
        list_delete(e[i].value, false);
    }

    free(e);

    puts("");

    printf("Palavras lidas       : %4d\n", word_no);
    printf("Ocorrencia de chaves : %4d\n", key_no);
    printf("Numero de colisoes   : %4d\n", colisions);

    return 0;
}
