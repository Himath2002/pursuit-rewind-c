CC ?= cc

TARGET := build/pursuit-rewind
TEST_TARGET := build/test-suite
OBJECT_DIR := build/obj
INCLUDE_DIR := include

STANDARD_FLAGS := -std=c11
WARNING_FLAGS := -Wall -Wextra -Wpedantic -Werror
OPTIMIZATION_FLAGS ?= -O2
CPPFLAGS += -I$(INCLUDE_DIR)
CFLAGS += $(STANDARD_FLAGS) $(WARNING_FLAGS) $(OPTIMIZATION_FLAGS)

CORE_SOURCES := src/game.c src/history.c src/map_loader.c src/movement.c
APP_SOURCES := $(CORE_SOURCES) src/main.c src/random_source.c src/renderer.c src/terminal.c
APP_OBJECTS := $(APP_SOURCES:src/%.c=$(OBJECT_DIR)/%.o)
TEST_OBJECTS := $(OBJECT_DIR)/test_suite.o $(CORE_SOURCES:src/%.c=$(OBJECT_DIR)/%.o)
DEPENDENCIES := $(APP_OBJECTS:.o=.d) $(TEST_OBJECTS:.o=.d)

.PHONY: all check clean debug run

all: $(TARGET)

$(TARGET): $(APP_OBJECTS)
	@mkdir -p $(dir $@)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(TEST_TARGET): $(TEST_OBJECTS)
	@mkdir -p $(dir $@)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(OBJECT_DIR)/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -MP -c $< -o $@

$(OBJECT_DIR)/test_suite.o: tests/test_suite.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -MP -c $< -o $@

check: $(TEST_TARGET)
	./$(TEST_TARGET)

run: $(TARGET)
	./$(TARGET) maps/classic.map

debug: OPTIMIZATION_FLAGS := -O0 -g3
debug: clean all

clean:
	rm -rf build

-include $(DEPENDENCIES)

