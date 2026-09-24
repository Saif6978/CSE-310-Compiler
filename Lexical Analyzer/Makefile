CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude -g
FLEX = flex

TARGET = lexer
FLEX_SRC = src/lexer.l
LEX_C = src/lex.yy.c

.PHONY: all clean run test

all: $(TARGET)

$(LEX_C): $(FLEX_SRC) include/*.h
	$(FLEX) -o $(LEX_C) $(FLEX_SRC)

$(TARGET): $(LEX_C)
	$(CXX) $(CXXFLAGS) $(LEX_C) -o $(TARGET)

run: $(TARGET)
	./$(TARGET) test/input1.txt token.txt log.txt

test: $(TARGET)
	@echo "--- Testing input1 ---"
	./$(TARGET) test/input1.txt token1.txt log1.txt
	@diff -u token1.txt test/input1_token.txt && echo "Test 1 Tokens: PASSED"
	@diff -u log1.txt test/input1_log.txt && echo "Test 1 Logs: PASSED"
	@echo "--- Testing input2 ---"
	./$(TARGET) test/input2.txt token2.txt log2.txt
	@diff -u token2.txt test/input2_token.txt && echo "Test 2 Tokens: PASSED"
	@diff -u log2.txt test/input2_log.txt && echo "Test 2 Logs: PASSED"
	@echo "--- Testing input3 ---"
	./$(TARGET) test/input3.txt token3.txt log3.txt
	@diff -u token3.txt test/input3_token.txt && echo "Test 3 Tokens: PASSED"
	@diff -u log3.txt test/input3_log.txt && echo "Test 3 Logs: PASSED"
	@rm -f token1.txt log1.txt token2.txt log2.txt token3.txt log3.txt
	@echo "All 3 test cases passed with 100% exact match!"

clean:
	rm -f $(TARGET) $(LEX_C) lex.yy.c token*.txt log*.txt *.log *.txt
