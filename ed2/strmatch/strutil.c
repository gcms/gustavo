#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "strutil.h"

char *strsub(const char *text, const char *old, const char *new) {
    unsigned int i = 0;
    size_t text_len, old_len, new_len;
    const char *p, *q;
    char *r, *result;

    old_len = strlen(old);

    /* if the string to be substituted is empty, just return a copy of text */
    if (old_len == 0) {
        return strdup(text);
    }

    /* find the number of occurrences of old in text */
    for (p = text; p != NULL; p = boyermoore(p, old)) {
        p += old_len;
        i++;
    }

    /* if old doesn't occur in text, return a copy of text */
    if (i == 0) {
        return strdup(text);
    }

    text_len = strlen(text);
    new_len = strlen(new);

    /* allocates space to hold the resulting string */
    result = malloc(text_len - (i * old_len) + (i * new_len) + 1);

    /* r points to the result, p points to the occurrences of old in text, and
     * q points to the text, just after the occurrence */
    for (r = result, p = strstr(text, old), q = text; p != NULL;
            p = strstr(p, old)) {
        memcpy(r, q, p - q);
        r += p - q;
        memcpy(r, new, new_len);
        r += new_len;
        p += old_len;
        q = p;
    }

    memcpy(r, q, text + text_len - q);
    r += text + text_len - q;
    *r = '\0';

    return result;
}

#define NUM_CHARS 256

char *boyermoore(const char *big, const char *little) {
    size_t m, n;
    unsigned int last[NUM_CHARS];
    unsigned int i;

    m = strlen(little);
    n = strlen(big);

    if (m == 0) {
        return (char *) big;
    } else if (m > n) {
        return NULL;
    }

    for (i = 0; i < NUM_CHARS; i++) {
        last[i] = 0;
    }

    /* marks the last occurrence of a character */
    for (i = 0; i < m; i++) {
        last[(unsigned char) little[i]] = i;
    }

    for (i = m - 1; i < n; i += m - last[(unsigned char) big[i + 1]]) {
        int j, k;

        for (j = m - 1, k = i; j >= 0; j--, k--) {
            if (little[j] != big[k]) {
                break;
            }
        }

        /* if the loop ended without break */
        if (j < 0) {
            return (char *) big + i - m + 1;
        }

    }

    return NULL;
}

int wildmatch(const char *text, const char *patt) {
    size_t m, n;
    unsigned int i, j;
    int greedy = 0;

    m = strlen(patt);
    n = strlen(text);

    for (j = 0, i = 0; j <= n && i <= m; /* */) {
        if (patt[i] == '#') {
            i++;
            greedy = 1;
        } else {
            greedy = 0;
        }

        if (greedy && i < m) {
            while (j < n) {
                if (text[j] == patt[i]
                        && wildmatch(text + j + 1, patt + i + 1)) {
                   return 1;
                }

                j++;
            }
            if (i == m) {
                return 1;
            }

            return 0;
        } else {
            if (text[j] == patt[i]) {
                j++;
                i++;
            } else {
                return 0;
            }
        }
    }

    return 1;
}

unsigned int hash(const char *p) {
    const char *str;
    size_t i, len;
    unsigned int hash;

    str = p;
    len = strlen(str);

    hash = 0;
    
    for (i = 0; i < len; i++)
        hash += str[i] * pow(31, len - i - 1);

    return hash;

}
