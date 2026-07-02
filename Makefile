CXX := g++
LEX := flex
YACC := bison

SRC_DIR := src
INC_DIR := inc
GRAMMAR_DIR := grammar
BUILD_DIR := build
OBJ_DIR := $(BUILD_DIR)/obj

ASM_TARGET := assembler
LD_TARGET := linker
EMU_TARGET := emulator

PARSER_C := $(GRAMMAR_DIR)/grammar.tab.c
PARSER_H := $(GRAMMAR_DIR)/grammar.tab.h
LEXER_C := $(GRAMMAR_DIR)/lex.yy.c
GENERATED_PARSER := $(PARSER_C) $(PARSER_H) $(LEXER_C)

CXXFLAGS ?= -g
CPPFLAGS ?= -I$(INC_DIR) -I$(GRAMMAR_DIR)
LDFLAGS ?=
LDLIBS ?= -pthread

MAIN_SOURCES := $(SRC_DIR)/asm_main.cpp $(SRC_DIR)/ld_main.cpp $(SRC_DIR)/emu_main.cpp
COMMON_SOURCES := $(filter-out $(MAIN_SOURCES),$(wildcard $(SRC_DIR)/*.cpp))
COMMON_OBJECTS := $(patsubst %.cpp,$(OBJ_DIR)/%.cpp.o,$(COMMON_SOURCES))

ASM_OBJECTS := \
	$(OBJ_DIR)/$(SRC_DIR)/asm_main.cpp.o \
	$(OBJ_DIR)/$(GRAMMAR_DIR)/grammar.tab.c.o \
	$(OBJ_DIR)/$(GRAMMAR_DIR)/lex.yy.c.o \
	$(OBJ_DIR)/$(GRAMMAR_DIR)/helper.cpp.o \
	$(COMMON_OBJECTS)

LD_OBJECTS := \
	$(OBJ_DIR)/$(SRC_DIR)/ld_main.cpp.o \
	$(COMMON_OBJECTS)

EMU_OBJECTS := \
	$(OBJ_DIR)/$(SRC_DIR)/emu_main.cpp.o \
	$(COMMON_OBJECTS)

DEPS := $(ASM_OBJECTS:.o=.d) $(LD_OBJECTS:.o=.d) $(EMU_OBJECTS:.o=.d)

.PHONY: all clean generate-parser

all: $(ASM_TARGET) $(LD_TARGET) $(EMU_TARGET)

generate-parser: $(GENERATED_PARSER)

$(ASM_TARGET): $(ASM_OBJECTS)
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(LD_TARGET): $(LD_OBJECTS)
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(EMU_TARGET): $(EMU_OBJECTS)
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(PARSER_C) $(PARSER_H): $(GRAMMAR_DIR)/grammar.y
	$(YACC) -d -o $(PARSER_C) $<

$(LEXER_C): $(GRAMMAR_DIR)/lexer.l $(PARSER_H)
	$(LEX) -o $@ $<

$(OBJ_DIR)/%.cpp.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MMD -MP -c $< -o $@

$(OBJ_DIR)/%.c.o: %.c
	@mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MMD -MP -x c++ -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(ASM_TARGET) $(LD_TARGET) $(EMU_TARGET) $(GENERATED_PARSER)

-include $(DEPS)
