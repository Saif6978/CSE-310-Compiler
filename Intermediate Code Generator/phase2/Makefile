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
	@echo "Generating ANTLR4 Phase 2 C++ visitor & lexer..."
	@cd $(GRAMMAR_DIR) && "$(ANTLR4)" -v 4.13.2 -Dlanguage=Cpp -visitor -no-listener -o ../$(GEN_DIR) CSubset.g4
	@touch $(GEN_SENTINEL)

generate: $(GEN_SENTINEL)

$(TARGET): $(GEN_SENTINEL) $(USER_SRCS)
	@echo "Compiling $(TARGET) for Phase 2..."
	@$(CXX) $(CXXFLAGS) $(INCLUDES) $(USER_SRCS) $(GEN_SRCS) $(LIBS) $(LDFLAGS) -o $(TARGET)
	@echo "Build successful: $(TARGET)"

run: $(TARGET)
	@if [ -z "$(INPUT)" ]; then \
		echo "Usage: make run INPUT=test/test1_i.c"; \
	else \
		LD_LIBRARY_PATH=/usr/local/lib ./$(TARGET) $(INPUT); \
	fi

test: $(TARGET)
	@echo "=========================================="
	@echo "       Running Phase 2 Test Suite         "
	@echo "=========================================="
	@FAILED=0; \
	EXPECTED_FILE="$(TEST_DIR)/output.txt"; \
	TESTS="test1_i test2_i test3_i test4_i test5_i test6_i test7_i recursion1_i recursion2_i func loop exp"; \
	for T in $$TESTS; do \
		echo "Testing $$T.c..."; \
		rm -f 2205119_code.asm optimizedcode.asm code.out opt.out actual.txt expected.txt; \
		LD_LIBRARY_PATH=/usr/local/lib ./$(TARGET) $(TEST_DIR)/$$T.c > /dev/null 2>&1 || { echo "  [FAIL] Compilation error on $$T.c"; FAILED=$$((FAILED + 1)); continue; }; \
		fasm 2205119_code.asm code.out > /dev/null 2>&1 || { echo "  [FAIL] FASM failed on $$T.c"; FAILED=$$((FAILED + 1)); continue; }; \
		chmod +x code.out; \
		./code.out | sed '/^[[:space:]]*$$/d' > actual.txt || { echo "  [FAIL] Runtime failure on $$T.c"; FAILED=$$((FAILED + 1)); continue; }; \
		awk -v test="$$T.out" '$$0 == test {found=1; next} found && /^[-]+$$/ {next} found && /^[a-zA-Z0-9_]+\.out$$/ {exit} found {print}' "$$EXPECTED_FILE" | sed '/^[[:space:]]*$$/d' > expected.txt; \
		if [ -s expected.txt ]; then \
			if diff -w actual.txt expected.txt > /dev/null 2>&1; then \
				echo "  [PASS] $$T.c (Output matches golden reference)"; \
			else \
				echo "  [FAIL] $$T.c (Output mismatch):"; \
				diff -u expected.txt actual.txt; \
				FAILED=$$((FAILED + 1)); \
			fi; \
		else \
			echo "  [PASS] $$T.c (Compiled, assembled & executed successfully)"; \
		fi; \
	done; \
	rm -f code.out opt.out actual.txt expected.txt; \
	echo "=========================================="; \
	if [ $$FAILED -eq 0 ]; then \
		echo "ALL PHASE 2 TESTS PASSED SUCCESSFULLY!"; \
	else \
		echo "$$FAILED TESTS FAILED."; \
		exit 1; \
	fi

clean:
	@rm -rf $(GEN_DIR) $(TARGET) 2205119_code.asm optimizedcode.asm code.out opt.out actual.txt expected.txt lexLogFile.txt
	@echo "Cleaned Phase 2 generated files and outputs."
