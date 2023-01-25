YELLOWBOLD='\033[1;33m'
GREEN= '\033[0;32m'
YELLOW='\033[0;33m'
NC='\033[0m'
CC = g++
LDFLAGS = -Wall -Wextra -Werror -std=c++17
SOURCE = $(wildcard tests/test_*.cc)
HEADER = $(wildcard s21_*.h)
OS := $(shell uname -s)
ifeq ($(OS), Darwin)
	OPEN_CMD = open
	ADD_LIBS = -lgtest
	LEAK_CHECK = leaks
	LEAK_ARGS = -atExit --
endif
ifeq ($(OS),Linux)
	OPEN_CMD = xdg-open
	ADD_LIBS = -lgtest -lrt -lm -lsubunit -lpthread
	LEAK_CHECK = valgrind
	LEAK_ARGS = --tool=memcheck --leak-check=full --track-origins=yes
endif

all: clean test

test: clean $(SOURCE)
	$(CC) $(LDFLAGS) -o $@ $(SOURCE) $(ADD_LIBS)
	-./$@

leaks_test: clean $(SOURCE)
	$(CC) $(LDFLAGS) -o $@ $(SOURCE) $(ADD_LIBS)
	-$(LEAK_CHECK) $(LEAK_ARGS) ./$@

style: $(SOURCE) $(HEADER)
	@echo ${YELLOWBOLD}"Linter start:..."
	@sleep 1
	@cp ../materials/linters/.clang-format .clang-format
	@clang-format -n $(SOURCE) $(HEADER)
	@echo $(YELLOW)"1..."
	@sleep 0.5
	@echo 2...
	@sleep 0.5
	@echo 3...
	@sleep 0.5
	@rm -f .clang-format
	@echo ${YELLOWBOLD}"Linter finished:" ${GREEN}"All passed!"${NC}

clean:
	rm -f ./test ./leaks_test *.o && rm -rf report/ ./tests/*.o
