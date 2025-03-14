SRC := $(shell find . -name "*.cpp")

.PHONY := module_test disassemble

module_test: $(SRC)
	g++ $^ -o $@

module_test_debug: $(SRC)
	g++ -g $^ -o $@

disassemble: module_test
	objdump --disassemble ./$^
