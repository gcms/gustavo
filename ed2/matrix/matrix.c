/*
 * matrix.c
 * Copyright (C) 2005 Gustavo Cipriano Mota Sousa <gustavomota@inf.ufg.br>
 *
 * Matrix manipulation functions implementation.
 */

#include <stdlib.h>
#include <stdio.h>
#include "matrix.h"

int **matrix_new(size_t w, size_t h) {
    int **result;
    int  i;
    size_t size = w * h;

    result = malloc(sizeof(int *) * w);
    if (result == NULL)
        return NULL;

    result[0] = malloc(sizeof(int) * size);
    if (result[0] == NULL) {
        free(result);
        return NULL;
    }

    for (i = 1; i < size; i++) {
        result[i] = result[i - 1] + h;
    }

    return result;
}

/*
 * Este e o algoritmo responsavel por encontrar um elemento em uma matriz
 * ordenada.
 *
 * Parametros:
 * (value) e o valor a ser buscado na matriz
 * (sx), (sy) sao as coordenadas do ponto inicial da matriz a ser buscada
 * dentro de (matrix)
 * (w), (h) sao a largura e altura da matriz a ser buscada
 *
 * Retorno:
 * Retorna 0 se nao foi encontrado (value) em (matrix), senao retorna um
 * valor diferente de 0.
 *
 * A ideia por tras do algoritmo e encontrar o ponto na diagonal principal
 * no qual o valor anterior na diagonal seja menor que (value) e o proximo
 * valor na diagonal seja maior que (value). Dessa forma, a partir deste
 * ponto os valores a equerda e acima do valor anterior sao menores que
 * (value), e os valores a direita e abaixo do proximo valor sao maiores
 * que (value). Logo sobram dois quadrantes para localizar a busca.
 *
 * 1) O primeiro passo do algoritmo e determinar o tamanho da menor dimensao,
 * e armazena-lo em (size), isso e feito para que a diagonal utilizada
 * durante o algoritmo nao passe os limites da matriz.
 *
 * 2) Logo apos testamos se a matriz possui ao menos um elemento, caso
 * contrario, esta retorna 0
 *
 * 3) Para determinar o ponto que procuramos na diagonal principal, fazemos
 * uma especie de busca binaria pela diagonal principal, comecamos com
 * um apontador para o ponto inicial (left), e para o final (right) e
 * entao calculamos o ponto central (middle). Logo apos, testamos (value)
 * com o elemento do meio, caso sejam iguais, encontramos o elemento, caso
 * (value) seja menor que o elemento mediano, sabemos que (value)
 * nao se encontra a direita nem abaixo do elemento mediano, e podemos passar
 * o elemento final (right) para o ponto imediatamente anterior ao elemento
 * do meio na diagonal. O contrario e feito se (value) for maior que o
 * elemento do meio.
 *
 * 4) Apos o passo anterior podemos delimitar os quadrantes que ainda
 * precisam ser buscados, e para realizar essa operacao chamamos a funcao
 * recursivamente.
 */

static int bsearch_helper(int **matrix, int value,
        unsigned int sx, unsigned int sy,
        size_t w, size_t h, point *p) {
    unsigned int left, right, middle;

    size_t size = (w < h ? w : h);

    if (size == 0) {
        return 0;
    }

    left = 0;
    right = size - 1;


    while (left < size && right < size && left <= right) {
        middle = (left + right) / 2;

        if (value == matrix[sx + middle][sy + middle]) {
            if (p != NULL) {
                p->x = sx + middle;
                p->y = sy + middle;
            }
            return 1;
        }

        if (value < matrix[sx + middle][sy + middle]) {
            right = middle - 1;
            middle = right;
        }

        if (value > matrix[sx + middle][sy + middle]) {
            left = middle + 1;
            middle = left;
        }
    }

    /*
     *     ------- w -------
     *    sx
     *  sy X X X X # # # # #  |
     *     X X X X # # # # #  |
     *     X X X X # # # # #  |
     *     X X X r # # # # #  h
     *     # # # # l X X X X  |
     *     # # # # X X X X X  |
     *     # # # # X X X X X  |
     *
     *     Os argumentos usados nas chamadas recursivas podem ser deduzidos
     *     da imagem acima que representa a configuracao dos valores neste
     *     ponto. (r) representa o valor de (right) e (l) o valor de (left),
     *     (X) sao os pontos a serem descartados e (#) os pontos que precisam
     *     ser procurados.
     *
     *     Obs: Os valores (right) e (left) sao relativos a (sx), (sy)
     *
     *     1st quadrante:
     *          sx = sx + left;
     *          sy = sy;
     *          w = w - left;
     *          h = left;
     *
     *     3rd quadrante:
     *          sx = sx;
     *          sy = sy + left;
     *          w = left;
     *          h = h - left;
     *
     */

    return bsearch_helper(matrix, value, sx + left, sy, w - left, left, p)
        || bsearch_helper(matrix, value, sx, sy + left, left, h - left, p);
}

int matrix_bsearch(int **matrix, int value, size_t w, size_t h, point *p) {
    return bsearch_helper(matrix, value, 0, 0, w, h, p);
}

void matrix_delete(int **matrix) {
    free(matrix[0]);
    free(matrix);
}
