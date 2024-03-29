// Copyright 2022 Daraban Albert-Timotei
/**
 * Useful structures/macros
 * 
 * https://ocw.cs.pub.ro/courses/so/laboratoare/resurse/die
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#define ALLOC_ERROR "Allocation error\n"

/* useful macro for handling error codes */
#define DIE(assertion, call_description)				\
	do {												\
		if (assertion) {								\
			fprintf(stderr, "(%s, %d): ",				\
					__FILE__, __LINE__);				\
			perror(call_description);					\
			exit(errno);				        		\
		}												\
	} while (0)

#endif  // UTILS_H_
