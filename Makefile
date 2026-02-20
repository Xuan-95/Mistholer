CC = gcc
CFLAGS = -I/opt/homebrew/include -Wall -Wextra -pedantic
LDFLAGS = -L/opt/homebrew/lib -lraylib -lm

SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)

mistholer: $(OBJECTS)
	$(CC) $(OBJECTS) -o mistholer $(LDFLAGS)
	rm -f $(OBJECTS)

debug: CFLAGS += -g -O0
debug: $(OBJECTS)
	$(CC) $(OBJECTS) -o mistholer $(LDFLAGS)
	dsymutil mistholer
	rm -f $(OBJECTS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f mistholer $(OBJECTS)
