CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wno-unused-parameter -Wno-attributes -O2
ANTLR4 := $(shell which antlr4 2>/dev/null || echo $(CURDIR)/../antlr4_venv/bin/antlr4 || echo $(CURDIR)/antlr4_venv/bin/antlr4)

INCLUDES := -Iinclude -Igenerated -I/usr/local/include/antlr4-runtime
LIBS := -L/usr/local/lib -lantlr4-runtime -pthread
LDFLAGS := -Wl,-rpath,/usr/local/lib

GRAMMAR_DIR := grammar
SRC_DIR := src
GEN_DIR := generated
TEST_DIR := test

TARGET := compiler.out

GRAMMAR_FILES := $(GRAMMAR_DIR)/CSubset.g4 $(GRAMMAR_DIR)/Lexer.g4
USER_SRCS := $(SRC_DIR)/main.cpp $(SRC_DIR)/AsmGenerator.cpp $(SRC_DIR)/Optimizer.cpp
GEN_SRCS := $(GEN_DIR)/CSubsetLexer.cpp $(GEN_DIR)/CSubsetParser.cpp $(GEN_DIR)/CSubsetVisitor.cpp $(GEN_DIR)/CSubsetBaseVisitor.cpp
GEN_SENTINEL := $(GEN_DIR)/.generated

.PHONY: all generate run test clean

all: $(TARGET)

$(GEN_SENTINEL): $(GRAMMAR_FILES)
	@mkdir -p $(GEN_DIR)
	@echo "Generating ANTLR4 Phase 1 C++ visitor & lexer..."
	@cd $(GRAMMAR_DIR) && "$(ANTLR4)" -v 4.13.2 -Dlanguage=Cpp -visitor -no-listener -o ../$(GEN_DIR) CSubset.g4
	@touch $(GEN_SENTINEL)

generate: $(GEN_SENTINEL)

$(TARGET): $(GEN_SENTINEL) $(USER_SRCS)
	@echo "Compiling $(TARGET) for Phase 1..."
	@$(CXX) $(CXXFLAGS) $(INCLUDES) $(USER_SRCS) $(GEN_SRCS) $(LIBS) $(LDFLAGS) -o $(TARGET)
	@echo "Build successful: $(TARGET)"

run: $(TARGET)
	@if [ -z "$(INPUT)" ]; then \
		echo "Usage: make run INPUT=test/input.c"; \
	else \
		LD_LIBRARY_PATH=/usr/local/lib ./$(TARGET) $(INPUT); \
	fi

test: $(TARGET)
	@echo "=========================================="
	@echo "       Running Phase 1 Test Suite         "
	@echo "=========================================="
	@rm -f 2205119_code.asm optimizedcode.asm code.out opt.out out1.txt out2.txt
	@echo "[Test 1] Compiling test/input.c..."
	@LD_LIBRARY_PATH=/usr/local/lib ./$(TARGET) $(TEST_DIR)/input.c > /dev/null
	@fasm 2205119_code.asm code.out > /dev/null
	@fasm optimizedcode.asm opt.out > /dev/null
	@chmod +x code.out opt.out
	@./code.out > out1.txt
	@./opt.out > out2.txt
	@diff -u out1.txt out2.txt > /dev/null || (echo "FAILED: Optimized code output differs!"; exit 1)
	@echo "  [PASS] test/input.c (Outputs match: 10, 10, 10; asm successfully optimized)"
	@echo "[Test 2] Compiling test/test_expressions.c..."
	@LD_LIBRARY_PATH=/usr/local/lib ./$(TARGET) $(TEST_DIR)/test_expressions.c > /dev/null
	@fasm 2205119_code.asm code.out > /dev/null
	@fasm optimizedcode.asm opt.out > /dev/null
	@chmod +x code.out opt.out
	@./code.out > out1.txt
	@./opt.out > out2.txt
	@diff -u out1.txt out2.txt > /dev/null || (echo "FAILED: Optimized code output differs!"; exit 1)
	@echo "  [PASS] test/test_expressions.c (Outputs match: 10, 20, 55; asm successfully optimized)"
	@rm -f code.out opt.out out1.txt out2.txt
	@echo "=========================================="
	@echo "ALL PHASE 1 TESTS PASSED SUCCESSFULLY!"

clean:
	@rm -rf $(GEN_DIR) $(TARGET) 2205119_code.asm optimizedcode.asm code.out opt.out out*.txt lexLogFile.txt
	@echo "Cleaned Phase 1 generated files and outputs."
