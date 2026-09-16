#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <criterion/parameterized.h>


// helper used with all functions that have string parameters
char *cr_strdup(const char *str)
{
    char *ptr = cr_malloc(strlen(str) + 1);
    if (ptr)
        strcpy(ptr, str);
    return ptr;
}