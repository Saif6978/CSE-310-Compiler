CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude -g
ASAN_FLAGS = -fsanitize=address

SRCS = src/main.cpp
TARGET = symbol_table

.PHONY: all clean run test

all: $(TARGET)

$(TARGET): $(SRCS) include/*.h
	$(CXX) $(CXXFLAGS) $(ASAN_FLAGS) $(SRCS) -o $(TARGET)

run: $(TARGET)
	./$(TARGET) test/input.txt test/output.txt

test: $(TARGET)
	./$(TARGET) test/input.txt test/output.txt
	@if diff -u test/output.txt test/sample_output.txt > /dev/null; then \
		echo "Verification PASSED: test/output.txt matches test/sample_output.txt exactly!"; \
	else \
		echo "Verification FAILED: output differs from sample_output.txt"; \
		diff -u test/output.txt test/sample_output.txt; \
		exit 1; \
	fi

clean:
	rm -f $(TARGET) test/output.txt output.txt
