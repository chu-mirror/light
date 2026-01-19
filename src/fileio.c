#include <stdint.h>
#include <unistd.h>

#include "fileio.h"

#define BUFFER_SIZE 1024

static uint8_t buffer[BUFFER_SIZE];

void
fileio_write_str(int fd, Str s)
{
    int n, wtn = 0;
    while (wtn < str_length(s)) {
        n = write(
            fd,
            raw_string(s) + wtn,
            str_length(s) - wtn < BUFFER_SIZE ? str_length(s) - wtn :
                                                BUFFER_SIZE
        );
        wtn += n;
    }
}

void
fileio_read_str(int fd, Str s)
{
    int n;
    while (n = read(fd, buffer, BUFFER_SIZE - 1)) {
        buffer[n] = '\0';
        str_extend(s, buffer);
    }
}

void
fileio_copy_file(int fd1, int fd2)
{
    int n;
    while (n = read(fd2, buffer, BUFFER_SIZE)) {
        int m, wtn = 0;
        while (wtn < n) {
            m = write(fd1, buffer + wtn, n - wtn);
            wtn += m;
        }
    }
}
