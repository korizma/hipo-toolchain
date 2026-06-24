CC := gcc
CXX := g++
LEX := flex
YACC := bison

SRC_DIR := src
BUILD_DIR := bin
OBJ_DIR := $(BUILD_DIR)/obj
TARGET := $(BUILD_DIR)/ss

INPUT ?= tests/nivo-c/isr_terminal.s

GRAMMAR := $(SRC_DIR)/grammar/grammar.y
LEXER := $(SRC_DIR)/grammar/lexer.l
PARSER_C := $(SRC_DIR)/grammar/grammar.tab.c
PARSER_H := $(SRC_DIR)/grammar/grammar.tab.h
LEXER_C := $(SRC_DIR)/grammar/lex.yy.c
GENERATED_C := $(PARSER_C) $(LEXER_C)
GENERATED_H := $(PARSER_H)

CFLAGS ?= -g
CXXFLAGS ?= -g
CPPFLAGS ?=
LDFLAGS ?=
LDLIBS ?=

C_SOURCES := $(filter-out $(GENERATED_C),$(shell find $(SRC_DIR) -name '*.c')) $(GENERATED_C)
CPP_SOURCES := $(shell find $(SRC_DIR) -name '*.cpp')

C_OBJECTS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.c.o,$(C_SOURCES))
CPP_OBJECTS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.cpp.o,$(CPP_SOURCES))
OBJECTS := $(C_OBJECTS) $(CPP_OBJECTS)
DEPS := $(OBJECTS:.o=.d)

.PHONY: all run clean

all: $(TARGET)

run: $(TARGET)
	./$(TARGET) $(INPUT)

$(TARGET): $(OBJECTS) | $(BUILD_DIR)
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(OBJ_DIR)/%.c.o: $(SRC_DIR)/%.c $(PARSER_H)
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -MP -c $< -o $@

$(OBJ_DIR)/%.cpp.o: $(SRC_DIR)/%.cpp $(PARSER_H)
	@mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MMD -MP -c $< -o $@

$(PARSER_C) $(PARSER_H): $(GRAMMAR)
	$(YACC) -d -o $(PARSER_C) $(GRAMMAR)

$(LEXER_C): $(LEXER) $(PARSER_H)
	$(LEX) -o $(LEXER_C) $(LEXER)

$(BUILD_DIR):
	mkdir -p $@

clean:
	rm -rf $(BUILD_DIR) $(GENERATED_C) $(GENERATED_H) ss

-include $(DEPS)
