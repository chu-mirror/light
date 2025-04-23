CFLAGS = -g -Iinclude

SOURCES = \
src/hash.c \
src/closure.c \
src/memgr.c \
src/list.c \
src/hash_table.c \
src/assoc_table.c

HEADERS = \
include/light.h \
include/idiom.h \
include/hash.h \
include/memgr.h \
include/closure.h \
include/list.h \
include/hash_table.h \
include/assoc_table.h

OBJECTS = $(SOURCES:.c=.o)

all: test

$(OBJECTS) : $(HEADERS)

test: $(OBJECTS) test.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f test $(OBJECTS)
