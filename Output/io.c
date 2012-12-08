#include <stdio.h>
#include <stdarg.h>

#ifndef DEBUG
#define DEBUG	0
#endif /* DEBUG */

#define debug_print(fmt, ...) \
    do { if (DEBUG) fprintf(stderr, fmt, __VA_ARGS__); } while (0)

void read (int count, ...)
{
    int i;
    va_list arguments;

    va_start (arguments, count);
    for (i = 0; i < count; i++) {
	int *value = va_arg (arguments, int *);

	scanf ("%d", value);
	debug_print ("[%d] = %d\n", i, *value);
    }
    va_end (arguments);
}

void write (int count, ...)
{
    int i;
    va_list arguments;

    va_start (arguments, count);
    debug_print ("count: %d\n", count);
    for (i = 0; i < count; i++) {
	debug_print ("[%d] = ", i);
	printf ("%d\n", va_arg (arguments, int));
    }
    va_end (arguments);
}

#if DEBUG
/*int main ()
{
    int arr[] = { 1, 2, 3, 4, 5 };

    write (5, arr[0], arr[1], arr[2], arr[3], arr[4]);
    read (2, &arr[1], &arr[3]);
    write (5, arr[0], arr[1], arr[2], arr[3], arr[4]);

    return 0;
}*/
#endif /* DEBUG */

