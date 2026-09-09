CC = gcc
CPPFLAGS += -Isrc -I/opt/homebrew/include -Iexternal/cjson -Iexternal/raygui
CFLAGS = -Wall -Wextra -pedantic
LDFLAGS = -L/opt/homebrew/lib
LDLIBS = -lraylib -lm

BUILD_DIR = build/release
SOURCES = $(wildcard src/*.c src/*/*.c src/*/*/*.c) external/cjson/cJSON.c
OBJECTS = $(patsubst %.c,$(BUILD_DIR)/%.o,$(SOURCES))
DEPS = $(OBJECTS:.o=.d)

.PHONY: all mistholer debug clean

all: mistholer

mistholer: $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS) $(LDLIBS)

debug:
	$(MAKE) BUILD_DIR=build/debug CFLAGS="$(CFLAGS) -g -O0" mistholer

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -MP -c $< -o $@

clean:
	rm -rf build
	rm -f mistholer

-include $(DEPS)
