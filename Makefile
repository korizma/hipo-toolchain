CC := gcc
CXX := g++
AR := ar
LEX := flex
YACC := bison

SRC_DIR := src
C_SRC_DIR := $(SRC_DIR)/c_src
GRAMMAR_DIR := $(SRC_DIR)/grammar
BUILD_DIR := bin
OBJ_DIR := $(BUILD_DIR)/obj

ASM_TARGET := $(BUILD_DIR)/assembler
LD_TARGET := $(BUILD_DIR)/linker
EMU_TARGET := $(BUILD_DIR)/emulator
COMMON_LIB := $(BUILD_DIR)/libcommon.a

ASM_INPUT ?= tests/nivo-c/isr_terminal.s
LD_ARGS ?=
EMU_ARGS ?=

GRAMMAR := $(GRAMMAR_DIR)/grammar.y
LEXER := $(GRAMMAR_DIR)/lexer.l
PARSER_C := $(C_SRC_DIR)/grammar.tab.c
PARSER_H := $(C_SRC_DIR)/grammar.tab.h
LEXER_C := $(C_SRC_DIR)/lex.yy.c
GENERATED_GRAMMAR := $(PARSER_C) $(PARSER_H) $(LEXER_C)

CFLAGS ?= -g
CXXFLAGS ?= -g
CPPFLAGS ?= -I$(C_SRC_DIR) -I$(GRAMMAR_DIR)
LDFLAGS ?=
LDLIBS ?=

COMMON_CPP_SOURCES := $(shell find $(C_SRC_DIR) -name '*.cpp')
COMMON_OBJECTS := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.cpp.o,$(COMMON_CPP_SOURCES))

ASM_MAIN_OBJECT := $(OBJ_DIR)/asm_main.cpp.o
LD_MAIN_OBJECT := $(OBJ_DIR)/ld_main.cpp.o
EMU_MAIN_OBJECT := $(OBJ_DIR)/emu_main.cpp.o

ASM_GRAMMAR_OBJECTS := $(OBJ_DIR)/c_src/grammar.tab.c.o $(OBJ_DIR)/c_src/lex.yy.c.o $(OBJ_DIR)/grammar/helper.cpp.o
ASM_OBJECTS := $(ASM_MAIN_OBJECT) $(ASM_GRAMMAR_OBJECTS)
LD_OBJECTS := $(LD_MAIN_OBJECT)
EMU_OBJECTS := $(EMU_MAIN_OBJECT)

DEPS := $(ASM_OBJECTS:.o=.d) $(LD_OBJECTS:.o=.d) $(EMU_OBJECTS:.o=.d) $(COMMON_OBJECTS:.o=.d)

.PHONY: all build build-asm build-ld build-emu build_asm build_ld build_emu asm ld emu run-asm run-ld run-emu run_asm run_ld run_emu test regenerate-grammar clean

all: build

build: build-asm build-ld build-emu

build-asm:
	$(MAKE) regenerate-grammar
	$(MAKE) $(ASM_TARGET)

build-ld: $(LD_TARGET)

build-emu: $(EMU_TARGET)

build_asm: build-asm

build_ld: build-ld

build_emu: build-emu

asm: build-asm

ld: build-ld

emu: build-emu

run-asm: build-asm
	./$(ASM_TARGET) $(ASM_INPUT)

run-ld: build-ld
	./$(LD_TARGET) $(LD_ARGS)

run-emu: build-emu
	./$(EMU_TARGET) $(EMU_ARGS)

run_asm: run-asm

run_ld: run-ld

run_emu: run-emu

test: build
	SS_TEST_SKIP_BUILD=1 ./tests/run_all.sh

regenerate-grammar: | $(C_SRC_DIR)
	$(YACC) -d -o $(PARSER_C) $(GRAMMAR)
	$(LEX) -o $(LEXER_C) $(LEXER)

$(COMMON_LIB): $(COMMON_OBJECTS) | $(BUILD_DIR)
	$(AR) rcs $@ $^

$(ASM_TARGET): $(ASM_OBJECTS) $(COMMON_LIB) | $(BUILD_DIR)
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(LD_TARGET): $(LD_OBJECTS) $(COMMON_LIB) | $(BUILD_DIR)
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(EMU_TARGET): $(EMU_OBJECTS) $(COMMON_LIB) | $(BUILD_DIR)
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(OBJ_DIR)/%.c.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -MP -c $< -o $@

$(OBJ_DIR)/c_src/grammar.tab.c.o: $(C_SRC_DIR)/grammar.tab.c | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MMD -MP -x c++ -c $< -o $@

$(OBJ_DIR)/c_src/lex.yy.c.o: $(C_SRC_DIR)/lex.yy.c | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MMD -MP -x c++ -c $< -o $@

$(OBJ_DIR)/%.cpp.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MMD -MP -c $< -o $@

$(BUILD_DIR) $(OBJ_DIR) $(C_SRC_DIR):
	mkdir -p $@

clean:
	rm -rf $(BUILD_DIR) $(GENERATED_GRAMMAR)

-include $(DEPS)
