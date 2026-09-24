CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -Wno-unused-parameter -Wno-attributes -O2
ANTLR4 := $(shell which antlr4 2>/dev/null || echo $(CURDIR)/antlr4_venv/bin/antlr4)

INCLUDES := -Iinclude -Igenerated -I/usr/local/include/antlr4-runtime
LIBS := -L/usr/local/lib -lantlr4-runtime -pthread
LDFLAGS := -Wl,-rpath,/usr/local/lib

GRAMMAR_DIR := grammar
SRC_DIR := src
GEN_DIR := generated
TEST_DIR := test

TARGET := compiler.out

GRAMMAR_FILES := $(GRAMMAR_DIR)/CSubset.g4 $(GRAMMAR_DIR)/Lexer.g4
USER_SRCS := $(SRC_DIR)/main.cpp $(SRC_DIR)/CSubsetVisitorImpl.cpp
GEN_SRCS := $(GEN_DIR)/CSubsetLexer.cpp $(GEN_DIR)/CSubsetParser.cpp $(GEN_DIR)/CSubsetVisitor.cpp $(GEN_DIR)/CSubsetBaseVisitor.cpp
GEN_SENTINEL := $(GEN_DIR)/.generated

.PHONY: all generate run test clean

all: $(TARGET)

$(GEN_SENTINEL): $(GRAMMAR_FILES)
	@mkdir -p $(GEN_DIR)
	@echo "Generating ANTLR4 C++ visitor & lexer..."
	@cd $(GRAMMAR_DIR) && "$(ANTLR4)" -v 4.13.2 -Dlanguage=Cpp -visitor -no-listener -o ../$(GEN_DIR) CSubset.g4
	@touch $(GEN_SENTINEL)

generate: $(GEN_SENTINEL)

$(TARGET): $(GEN_SENTINEL) $(USER_SRCS)
	@echo "Compiling $(TARGET)..."
	@$(CXX) $(CXXFLAGS) $(INCLUDES) $(USER_SRCS) $(GEN_SRCS) $(LIBS) $(LDFLAGS) -o $(TARGET)
	@echo "Build successful: $(TARGET)"

run: $(TARGET)
	@if [ -z "$(INPUT)" ]; then \
		echo "Usage: make run INPUT=test/input1.txt"; \
	else \
		LD_LIBRARY_PATH=/usr/local/lib ./$(TARGET) $(INPUT); \
	fi

test: $(TARGET)
	@echo "=========================================="
	@echo "       Running Offline 3 Test Suite       "
	@echo "=========================================="
	@FAILED=0; \
	for i in 1 2 3 4 5; do \
		echo "Testing input$$i.txt..."; \
		rm -f log.txt error.txt lexLogFile.txt; \
		LD_LIBRARY_PATH=/usr/local/lib ./$(TARGET) $(TEST_DIR)/input$$i.txt > /dev/null 2>&1 || true; \
		LOG_DIFF=0; \
		ERR_DIFF=0; \
		diff -w -B $(TEST_DIR)/log$$i.txt log.txt > /dev/null 2>&1 || LOG_DIFF=1; \
		diff -w -B $(TEST_DIR)/error$$i.txt error.txt > /dev/null 2>&1 || ERR_DIFF=1; \
		if [ $$LOG_DIFF -eq 0 ] && [ $$ERR_DIFF -eq 0 ]; then \
			echo "  [PASS] Test $$i (log and error match)"; \
		else \
			echo "  [FAIL] Test $$i:"; \
			[ $$LOG_DIFF -ne 0 ] && echo "    - log$$i.txt mismatch"; \
			[ $$ERR_DIFF -ne 0 ] && echo "    - error$$i.txt mismatch"; \
			FAILED=$$((FAILED + 1)); \
		fi; \
	done; \
	echo "=========================================="; \
	if [ $$FAILED -eq 0 ]; then \
		echo "ALL 5 TESTS PASSED SUCCESSFULLY!"; \
	else \
		echo "$$FAILED TESTS FAILED."; \
		exit 1; \
	fi

clean:
	@rm -rf $(GEN_DIR) $(TARGET) log.txt error.txt lexLogFile.txt
	@echo "Cleaned generated files and outputs."
