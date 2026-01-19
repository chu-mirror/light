#ifndef LIGHT_FILEIO_H
#define LIGHT_FILEIO_H

#include "str.h"

void fileio_write_str(int fd, Str s);
void fileio_read_str(int fd, Str s);
void fileio_copy_file(int fd1, int fd2);

#endif
