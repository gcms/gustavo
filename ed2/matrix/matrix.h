/*
 * matrix.h
 * Copyright (C) 2005 Gustavo Cipriano Mota Sousa <gustavomota@inf.ufg.br>
 *
 * Matrix manipulation functions.
 *
 * TODO: Extend theimplementation to allow the use of user defined data types.
 */

#ifndef MATRIX_H_
#define MATRIX_H_
#include <stddef.h>
#include "point.h"

/*
 * Returns a newly allocated matrix of width columns and height lines.
 * The matrix returned by this function can be directly accessed by using
 * the [] operator.
 */
int **matrix_new(size_t height, size_t width);

/*
 * Returns 1 if an ordered matrix contains value, or 0 if doesn't.
 * The coordinates of the point are store in p if the element was found
 * and p is different of NULL.
 */
int matrix_bsearch(int **matrix, int value, size_t w, size_t h, point *p);


/*
 * Free a previously allocated matrix.
 */
void matrix_delete(int **matrix);


#endif /* MATRIX_H_ */
