CFLAGS = -g -Iinclude

SOURCES = src/hash.c src/list.c
OBJECTS = $(SOURCES:.c=.o)

all: test

test: $(OBJECTS) test.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f test $(OBJECTS)
