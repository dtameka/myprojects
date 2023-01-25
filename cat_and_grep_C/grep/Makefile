.PHONY: all s21_grep test
CC = gcc
CCFLAGS = -std=c11 -Wall -Wextra -Werror
CCDIR = -I ./
UNAME := $(shell uname -s)
YELLOW='\033[0;33m'
YELLOWBOLD='\033[1;33m'
GREEN='\033[0;32m'
NC='\033[0m'

default: all

all: s21_grep test

s21_grep:
	$(CC) $(CCFLAGS) $(CCDIR) s21_grep.c -o s21_grep

test: s21_grep
	@echo ${YELLOWBOLD}"Goal test:..."${NC}
	@chmod u+x s21_grep_test.sh
	@./s21_grep_test.sh
	@echo ${YELLOW}"Goal test: Tests are completed!"${NC}
	@echo ""

check:
	cppcheck *.c *.h
	@cp ../../materials/linters/CPPLINT.cfg CPPLINT.cfg;
	@cd ../.. && python3 materials/linters/cpplint.py --extensions=c --repository=. src/grep/s21_grep.c src/grep/s21_grep.h
	@rm -f *.cfg
	@echo ${GREEN}"Goal check: All passed!"${NC}
	@echo ""

leaks:
	@echo ${YELLOWBOLD}"Goal leaks:..."${NC}
ifeq ($(UNAME), Linux)
	valgrind --tool=memcheck --leak-check=full ./s21_grep -f pattern grep_test.txt grep_test1.txt grep_test2.txt
else ifeq ($(UNAME), Darwin)
	leaks --atExit -- ./s21_grep -f pattern grep_test.txt grep_test1.txt grep_test2.txt
endif
	@echo ${GREEN}"Goal leaks: All passed!"${NC}
	@echo ""

checkit:
	@cp ../../materials/linters/CPPLINT.cfg CPPLINT.cfg
	@cd ../.. && python3 materials/linters/cpplint.py --extensions=c --repository=. src/grep/s21_grep.c src/grep/s21_grep.h
	@rm -f *.cfg
	@echo ${GREEN}"Goal lint: All passed!"${NC}
	@echo ""

clean:
	@echo ${YELLOWBOLD}"Goal clean:..."${NC}
	@rm -f s21_grep s21_grep.h.gch 
	@rm -rf grep_test_results.txt s21_grep_test_results.txt *.cfg
	@echo ${GREEN}"Goal clean: All is whiped out!"${NC}
	@echo ""

rebuild: clean all
