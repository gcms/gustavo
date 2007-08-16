/*
 * matrix_test.c
 * Copyright (C) 2005 Gustavo Cipriano Mota Sousa <gustavomota@inf.ufg.br>
 *
 * Creates a new matrix, fill it with ordered values, and them search
 * for the elements in the matrix. Used for testing.
 */

#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <stdio.h>
#include "debug.h"


#include "matrix.h"

#define max(a,b) (a > b ? a : b)

static void fill_matrix(int **matrix, size_t w, size_t h) {
    int x, y;

    srand(time(NULL));

    for (x = 0; x < w; x++) {
        for (y = 0; y < h; y++) {
            matrix[x][y] = (x == 0 && y == 0 ? rand() % 10 :
                x == 0 ? matrix[x][y - 1] + rand() % 10 :
                y == 0 ? matrix[x - 1][y] + rand() % 10 :
                max(matrix[x - 1][y], matrix[x][y - 1]) + rand() % 10) + 1;
        }
    }
}

static void print_matrix(int **matrix, size_t w, size_t h) {
    size_t x, y;

    for (y = 0; y < h; y++) {
        for (x = 0; x < w; x++) {
            debug("%3d%c", matrix[x][y], x == w - 1 ? '\n' : ' ');
        }
    }
}

static void test_matrix(size_t w, size_t h) {
    int **matrix;
    int x, y;

    matrix = matrix_new(w, h);
    if (matrix == NULL) {
        return;
    }
    fill_matrix(matrix, w, h);
    print_matrix(matrix, w, h);

    for (x = 0; x < w; x++) {
        for (y = 0; y < h; y++) {
            debug("matrix[%d][%d] = %d\n", x, y, matrix[x][y]);
            assert(matrix_bsearch(matrix, matrix[x][y], w, h, NULL));
        }
    }

    assert(!matrix_bsearch(matrix, matrix[w - 1][h - 1] + 1, w, h, NULL));
    matrix_delete(matrix);
}

void matrix_test1(void) {
    test_matrix(10, 10);
    test_matrix(30, 30);
    test_matrix(100, 100);
    test_matrix(1000, 3);
}
