.PHONY: all test clean symbol-table lexer parser icg test-symbol-table test-lexer test-parser test-icg

all: symbol-table lexer parser icg

symbol-table:
	@$(MAKE) -C "Symbol Table" all

lexer:
	@$(MAKE) -C "Lexical Analyzer" all

parser:
	@$(MAKE) -C "Syntax and Semantic Analyzer" all

icg:
	@$(MAKE) -C "Intermediate Code Generator" all

test: test-symbol-table test-lexer test-parser test-icg
	@echo "=========================================="
	@echo " ALL 4 COMPILER MODULES PASSED ALL TESTS! "
	@echo "=========================================="

test-symbol-table:
	@echo "=== [1/4] Testing Symbol Table ==="
	@$(MAKE) -C "Symbol Table" test

test-lexer:
	@echo "=== [2/4] Testing Lexical Analyzer ==="
	@$(MAKE) -C "Lexical Analyzer" test

test-parser:
	@echo "=== [3/4] Testing Syntax and Semantic Analyzer ==="
	@$(MAKE) -C "Syntax and Semantic Analyzer" test

test-icg:
	@echo "=== [4/4] Testing Intermediate Code Generator ==="
	@$(MAKE) -C "Intermediate Code Generator" test

clean:
	@$(MAKE) -C "Symbol Table" clean
	@$(MAKE) -C "Lexical Analyzer" clean
	@$(MAKE) -C "Syntax and Semantic Analyzer" clean
	@$(MAKE) -C "Intermediate Code Generator" clean
	@echo "All build outputs cleaned."
