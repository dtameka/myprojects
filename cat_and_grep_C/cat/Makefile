.PHONY: all s21_cat test
CC = gcc
CCFLAGS = -std=c11 -Wall -Wextra -Werror
CCDIR = -I ./
UNAME := $(shell uname -s)
YELLOW='\033[0;33m'
YELLOWBOLD='\033[1;33m'
GREEN= '\033[0;32m'
NC='\033[0m'

default: all

all: s21_cat test

s21_cat:
#	$(CC) $(CCFLAGS) s21_cat.c s21_cat.h -o s21_cat
	$(CC) $(CCFLAGS) $(CCDIR) s21_cat.c -o s21_cat

test: s21_cat
	@echo ${YELLOWBOLD}"Goal test:..."${NC}
	@chmod u+x s21_cat_test.sh
	@./s21_cat_test.sh
	@echo ${YELLOW}"Goal test: Tests are completed!"${NC}
	@echo ""

check:
	cppcheck *.c *.h
	@cp ../../materials/linters/CPPLINT.cfg CPPLINT.cfg;
	@cd ../.. && python3 materials/linters/cpplint.py --extensions=c --repository=. src/cat/s21_cat.c src/cat/s21_cat.h
#	cd ../.. && python3 materials/linters/cpplint.py --extensions=c src/cat/s21_cat.c src/cat/s21_cat.h
	@rm -f *.cfg
	@echo ${GREEN}"Goal check: All passed!"${NC}
	@echo ""

leaks:
ifeq ($(UNAME), Linux)
	valgrind --tool=memcheck --leak-check=full ./s21_cat -bs cat_test.txt
else ifeq ($(UNAME), Darwin)
	leaks --atExit -- ./s21_cat -bs cat_test.txt
endif
	@echo ${GREEN}"Goal leaks: All passed!"${NC}
	@echo ""

checkit:
	@echo ${YELLOWBOLD}"Goal lint:..."${NC}
	@cp ../../materials/linters/CPPLINT.cfg CPPLINT.cfg
	@cd ../.. && python3 materials/linters/cpplint.py --extensions=c --repository=. src/cat/s21_cat.c src/cat/s21_cat.h
#	python3 ../../materials/linters/cpplint.py --extensions=c *.c *.h
	@rm -f *.cfg
	@echo ${GREEN}"Goal lint: All passed!"${NC}
	@echo ""

clean:
	@rm -f s21_cat s21_cat.h.gch 
	@rm -rf s21_cat_test_results.txt cat_test_results.txt *.cfg *.py
	@echo ${GREEN}"Goal clean: All is whiped out!"${NC}
	@echo ""

rebuild: clean all
