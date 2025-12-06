#include "light.h"
#include "string.h"

#include "str.h"

#define INITIAL_MEM_SIZE 32

struct str {
    char *raw_string;
    int len;
    int mem_size;
};

void
new_str(Str *str_r, char *s)
{
    NEW0(*str_r);
    (*str_r)->len = strlen(s);
    (*str_r)->mem_size = INITIAL_MEM_SIZE;
    while ((*str_r)->mem_size < (*str_r)->len + 1) {
        (*str_r)->mem_size <<= 1;
    }
    CALLOC((*str_r)->raw_string, (*str_r)->mem_size);
    strcpy((*str_r)->raw_string, s);
}

void
free_str(Str *str_r)
{
    FREE((*str_r)->raw_string);
    FREE(*str_r);
}

char *
raw_string(Str str)
{
    return str->raw_string;
}

int
str_length(Str str)
{
    return str->len;
}

void
str_extend(Str str, char *s)
{
    int new_len;

    new_len = str->len + strlen(s);
    while (str->mem_size < new_len) {
        str->mem_size <<= 1;
    }
    REALLOC_ARRAY(str->raw_string, str->mem_size);
    strcpy(str->raw_string + str->len, s);
    str->len = new_len;
}

void
str_append(Str str, char c)
{
    char s[2] = {c, '\0'};
    str_extend(str, s);
}
