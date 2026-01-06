#ifndef LIGHT_STR_H
#define LIGHT_STR_H

typedef struct str *Str;

void new_str(Str *str_r);
void free_str(Str *str_r);
char *raw_string(Str str);
int str_length(Str str);
void str_extend(Str str, char *s);
void str_append(Str str, char c);
void str_clean(Str str);

#endif
