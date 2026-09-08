CC = gcc
CFLAGS = -I/opt/homebrew/include -Iexternal/cjson -Iexternal/raygui -Wall -Wextra -pedantic
LDFLAGS = -L/opt/homebrew/lib -lraylib -lm

SOURCES = $(wildcard *.c) external/cjson/cJSON.c
OBJECTS = $(SOURCES:.c=.o)

mistholer: $(OBJECTS)
	$(CC) $(OBJECTS) -o mistholer $(LDFLAGS)
	rm -f $(OBJECTS)

debug: CFLAGS += -g -O0
debug: $(OBJECTS)
	$(CC) $(OBJECTS) -o mistholer $(LDFLAGS)
	rm -f $(OBJECTS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f mistholer $(OBJECTS)
