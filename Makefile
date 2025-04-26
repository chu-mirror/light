CFLAGS = -g -Iinclude

-include build.mk

SOURCES = $(LIGHT_FULL_SOURCES)
HEADERS = $(LIGHT_FULL_HEADERS)
OBJECTS = $(SOURCES:.c=.o)

all: test

$(OBJECTS) : $(HEADERS)

test: $(OBJECTS) test.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f test $(OBJECTS)
