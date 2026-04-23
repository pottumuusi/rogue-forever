.PHONY: module_test memory_error_detector

module_test:
	gcc -DDEBUG=1 ./*.c -o $@

memory_error_detector:
	gcc -fsanitize=address -DDEBUG=1 ./*.c -o $@
