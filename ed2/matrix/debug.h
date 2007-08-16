/*
 * debug.h
 * Copyright (C) 2005 Gustavo Cipriano Mota Sousa <gustavomota@inf.ufg.br>
 *
 * Contains a set of useful macros and functions for debugging purposes.
 */
#ifndef DEBUG_H_
#define DEBUG_H_

#ifndef QUIET
#define debug(fmt,...) fprintf(stderr, fmt, __VA_ARGS__)
#else
#define debug(fmt,...)
#endif


#endif /* DEBUG_H_ */
