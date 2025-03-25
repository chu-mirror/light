CFLAGS = -g -Iinclude

SOURCES = \
src/hash.c \
src/memory.c \
src/list.c \
src/hash_table.c

HEADERS = \
include/hash.h \
include/memgr.h \
include/list.h \
include/hash_table.h

OBJECTS = $(SOURCES:.c=.o)

all: test

$(OBJECTS) : $(HEADERS)

test: $(OBJECTS) test.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f test $(OBJECTS)
