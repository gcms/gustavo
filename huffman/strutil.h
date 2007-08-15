#ifndef STR_UTIL_H_
#define STR_UTIL_H_


/**
 * Substitutes all occurrences of old in text, by new, and return a newly
 * allocated string containing this result
 * @param text string to be searched for
 * @param old value to be searched
 * @param new value that will substitute old
 * @return a newly allocated string with all occurrences of old substituted
 * by new
 */
char *strsub(const char *text, const char *old, const char *new);

/**
 * Return the first occurrence of a substring in another string using the
 * Boyer-Moore algorithm.
 * @param big string containing the string to be found
 * @param little string wanted
 * @return pointer to the first occurrence of little in big
 */
char *boyermoore(const char *big, const char *little);

/**
 * Returns whether text matches with the pattern patt, specified according to
 * the following constraints.
 * A character other than '#' in the pattern string, must match exactly the
 * same character in the text pattern, the '#' character may match any number
 * of different characters.
 * For example, the following expressions must be true:
 *    wildmatch("x[i] = x[i-1] + 1;", "x#[#]#=#;");
 *     wildmatch("x2[i]=x[i-1]+1; y= z;", "x#[#]#=#;");
 */
int wildmatch(const char *text, const char *patt);

/**
 * Calculatess a hash code for a string of characters.
 * @param str string to be hashed
 * @return hash value for the string specified by str
 */
unsigned int hash(const char *str);


#endif /* STR_UTIL_H_ */
