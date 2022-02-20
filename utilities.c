#define _GNU_SOURCE
/*************** C STDLIB ***************/
#include <assert.h>
#include <complex.h>
#include <ctype.h>
#include <errno.h>
#include <fenv.h>
#include <float.h>
#include <inttypes.h>
#include <iso646.h>
#include <limits.h>
#include <locale.h>
#include <math.h>
#include <setjmp.h>
#include <signal.h>
#include <stdalign.h>
#include <stdarg.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>
#include <string.h>
#include <tgmath.h>
#include <threads.h>
#include <time.h>
#include <uchar.h>
#include <wchar.h>
#include <wctype.h>
/****************************************/

#define streq(s1, s2) (strcmp((s1), (s2)) == 0)
#define strcaseeq(s1, s2) (strcasecmp((s1), (s2)) == 0)

#define eprintf(format, ...) fprintf(stderr, format __VA_OPT__(,) __VA_ARGS__)


#define sign(x) ((x > 0) - (x < 0))

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

// array length
#define ARR_LEN(array) (sizeof(array) / sizeof *(array))


// string literal or char array length
#define len(str) (sizeof(str) - 1)

// take a random element from an array
#define take_elem(array) ((array)[rand() % ARR_LEN(array)])
// take a random char from a nul-terminated string literal / char array
#define take_char(str) ((str)[rand() % (sizeof(str) - 1)])

#define stringify(x) (#x)


char lower[] = "abcdefghijklmnopqrstuvwxyz";
char upper[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

char alpha[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

char print[] = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
char graph[] =  "!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";


int cmp_ints_ascending (const void *a_, const void *b_)
{
	int a = *((int*)a_), b = *((int*)b_);
	return (a > b) - (a < b);
}

int cmp_ints_descending (const void *a_, const void *b_)
{
	int a = *((int*)a_), b = *((int*)b_);
	return (a < b) - (a > b);
}

int *sort_ascending (size_t length, const int array[length])
{
	size_t nb_bytes = length * sizeof *array;
	int *sorted = malloc(nb_bytes);
	memcpy(sorted, array, nb_bytes);
	qsort(sorted, length, sizeof *sorted, cmp_ints_ascending);
	return sorted;
}

int rand_range (int min, int max) // inclusive
{
	return min + rand() / (RAND_MAX / (max - min + 1) + 1);
}

double float_rand (double min, double max)
{
	double scale = rand() / (double)RAND_MAX;
	return min + scale * (max - min);
}

unsigned rand_unsigned (void)
{
	unsigned n = 0;
	int nb_bytes = 1 + (rand() % (sizeof n));

	for (int i = 0; i < nb_bytes; i++) {
		n <<= 8;
		n |= rand() % (1 + UCHAR_MAX);
	}
	return n;
}

void print_array (size_t length, const int array[length])
{
	if (length == 0) {
		printf("{}\n");
		return;
	}
	printf("{");
	for (size_t i = 0; i < length - 1; i++)
		printf("%d, ", array[i]);
	printf("%d}\n", array[length - 1]);
}

void print_matrix (size_t rows, size_t cols, const int matrix[rows][cols])
{
	printf("{\n");
	for (size_t row = 0; row < rows; row++) {
		if (cols == 0) {
			printf("\t{}\n");
			continue;
		}

		printf("\t{");
		for (size_t col = 0; col < cols - 1; col++)
			printf("%d, ", matrix[row][col]);
		printf("%d}\n", matrix[row][cols - 1]);
	}
	printf("}\n");
}

void print_words (const char *const words[])
{
	if (words[0] == NULL) {
		printf("{}\n");
		return;
	}
	printf("{");
	for (; words[1]; words++)
		printf("\"%s\", ", words[0]);
	printf("\"%s\"}\n", words[0]);
}

void shuffle (size_t length, int array[length])
{
	for (size_t i = 0; i < length; i++) {
		size_t j = rand() % length;
		int tmp = array[i];
		array[i] = array[j];
		array[j] = tmp;
	}
}


int strcasecmp (const char *s1, const char *s2)
{
	for (; *s1 && *s2 && tolower(*s1) == tolower(*s2); s1++, s2++)
		;
	return tolower(*s1) - tolower(*s2);
}

int strncasecmp (const char *s1, const char *s2, size_t n)
{
	if (n == 0)
		return 0;

	for (size_t i = 1; i < n && *s1 && *s2 && tolower(*s1) == tolower(*s2); i++, s1++, s2++)
		;
	return tolower(*s1) - tolower(*s2);
}

char *strdup (const char *string)
{
	return strcpy(malloc(strlen(string) + 1), string);
}

int asprintf (char **string_ptr, const char *format, ...)
{
	*string_ptr = NULL;

	va_list args;

	va_start(args, format);
	int nb_chars = vsnprintf(NULL, 0, format, args);
	va_end(args);

	if (nb_chars == -1)
		return -1;

	*string_ptr = malloc(nb_chars + 1);

	va_start(args, format);
	int retval = vsprintf(*string_ptr, format, args);
	va_end(args);

	return retval;
}

int vasprintf (char **string_ptr, const char *format, va_list arg)
{
	va_list copy;
	va_copy(copy, arg);

	int nb_chars = vsnprintf(NULL, 0, format, arg);

	*string_ptr = malloc(nb_chars + 1);

	int retval = vsprintf(*string_ptr, format, copy);
	va_end(copy);

	return retval;
}










typedef unsigned      char uchar;
typedef unsigned       int uint;
typedef unsigned      long ulong;
typedef unsigned long long ullong;
typedef          long long llong;

typedef   int8_t i8;
typedef  uint8_t u8;
typedef  int16_t i16;
typedef uint16_t u16;
typedef  int32_t i32;
typedef uint32_t u32;
typedef  int64_t i64;
typedef uint64_t u64;
