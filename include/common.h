#ifndef COMPILER_COMMON_H_
#define COMPILER_COMMON_H_
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#define str_cpy(s1, s2); s1 = (char *)malloc(sizeof(s2)); strcpy(s1, s2);
#define newp(t) (t *)malloc(sizeof(t))

typedef char bool;
#define true 1
#define false 0

static char * str_cat(const char * s1, const char * s2) {
	char * s = (char *)malloc(sizeof(s1) + sizeof(s2));
	sprintf(s, "%s%s", s1, s2);
	return s;
}

#endif // COMPILER_COMMON_H_
