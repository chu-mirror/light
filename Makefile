CFLAGS = -g -Iinclude

SOURCES := $(addprefix src/,$(shell ./finddeff test.c include))
OBJECTS := $(SOURCES:.c=.o)

all: test

test: $(OBJECTS) test.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f test $(OBJECTS)
