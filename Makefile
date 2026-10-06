# gradebook-cli —— 使用标准 C++17，无第三方依赖
CXX      ?= g++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -O2
BUILD    ?= build

.PHONY: all test clean

all: $(BUILD)/gradebook-cli

$(BUILD)/gradebook-cli: src/main.cpp src/gradebook.cpp src/gradebook.h
	@mkdir -p $(BUILD)
	$(CXX) $(CXXFLAGS) src/main.cpp src/gradebook.cpp -o $@

test: $(BUILD)/tests
	./$(BUILD)/tests

$(BUILD)/tests: tests/test_gradebook.cpp src/gradebook.cpp src/gradebook.h
	@mkdir -p $(BUILD)
	$(CXX) $(CXXFLAGS) tests/test_gradebook.cpp src/gradebook.cpp -o $@

clean:
	rm -rf $(BUILD)
