.PHONY: install test gcov_report
CFLAGS=-Wall -Werror -Wextra -std=c11
GCOV=-fprofile-arcs -ftest-coverage
PROJECT=Calculus
SOURCE=s21_test.c s21_smartcalc.c s21_creditcalc.c s21_depositcalc.c
HEADER=s21_smartcalc.h s21_creditcalc.h s21_depositcalc.h
OBJECT=s21_smartcalc.o s21_creditcalc.o s21_depositcalc.o
LIB=s21_smartcalc.a s21_depositcalc.a s21_creditcalc.a
BUILD=Calculus
OUTPUTDIR=Application

OS=$(shell uname -s)
SHELL:=/bin/bash
ifeq ($(OS), Darwin)
	CHECKFLAG=-lcheck
	QMAKEDIR=$(HOME)/Qt/6.2.4/macos/bin/qmake
	OUTPUTFILE=Calculus.app
	OPEN=open
else
	CHECKFLAG=-L . -lcheck -lm -lsubunit -lrt -lpthread
	QMAKEDIR=/usr/lib/qt5/bin/qmake
	OUTPUTFILE=Calculus
	OPEN=xdg-open
endif

default: all

all: install test gcov_report

.o: $(SOURCE)
	$(CC) -c $(SOURCE) 

install: uninstall clean
	$(QMAKEDIR) -o makefile_qmake ./Calculus.pro
ifeq ($(OS), Darwin)
	make -f makefile_qmake qmake_all
	make -f makefile_qmake -j4
else
	make -f makefile_qmake all	
endif
	rm -f moc_*.* ui_*.* *.o makefile_qmake
	rm -rf build*
	if  [[ ! -d ./$(OUTPUTDIR) ]]; then mkdir $(OUTPUTDIR); fi;
ifeq ($(OS), Darwin)
	cp /$(OUTPUTFILE)/Contents/MacOS/$(PROJECT) /$(OUTPUTDIR)/$(PROJECT)
else
	cp ./Calculus Application/
endif
	rm -rf $(OUTPUTFILE)

uninstall: clean
	rm -f ./.qmake.stash ./makefile_qmake
	rm -f ./*.o ./*.a ./moc_* ./qrc_resource.cpp
	rm -f moc_*.* ui_*.* *.info *~ core *.core $(PROJECT) *.tar.gz
	rm -f *.gcda *.gcno *.info gcov_report
	rm -f *.out *.exe cpplint.py CPPLINT.cfg *.h.gch *.o *.a
	rm -rf build*
	rm -rf $(OUTPUTDIR)/ ./$(OUTPUTFILE)

dist: uninstall install
	tar -czf Calculus.install.tar.gz $(OUTPUTDIR)/$(PROJECT) 

dvi: documentation
	$(OPEN) ./Documentation/index.html	

documentation:
	makeinfo --html s21_smartcalc.texi

test: clean
	gcc $(GCOV) -c s21_smartcalc.c
	ar rcs s21_smartcalc.a s21_smartcalc.o
	gcc $(GCOV) -c s21_creditcalc.c
	ar rcs s21_creditcalc.a s21_creditcalc.o
	gcc $(GCOV) -c s21_depositcalc.c
	ar rcs s21_depositcalc.a s21_depositcalc.o
	gcc -c s21_test.c
	gcc $(GCOV) -o test s21_test.o s21_smartcalc.a s21_creditcalc.a s21_depositcalc.a $(CHECKFLAG)
	./test

clean:
	rm -f *.gcda *.gcno *.info gcov_report 
	rm -f s21_smartcalc_test *.out *.exe cpplint.py CPPLINT.cfg *.h.gch *.o *.a
	rm -rf build*
	rm -f *.autosave
	rm -rf makefile_qmake ./.qmake.stash ./makefile_qmake ./ui_*.*
	rm -rf Report/ Documentation/ build* ./test
	rm -f moc_*.* ui_*.* *.o *.info qrc_*.*
	rm -f *~ core *.core *.tar.gz
	

gcov_report: test
	lcov -t "gcov_report" -o s21_smartcalc_report.info -c -d .
	genhtml -o Report s21_smartcalc_report.info

linter:
	cp ../../materials/linters/CPPLINT.cfg /home/dtameka/C7_SmartCalc_v1.0-0/src/Calculus
	python3 ../../materials/linters/cpplint.py --extensions=c --repository=. $(SOURCE)
	rm -f /home/dtameka/C7_SmartCalc_v1.0-0/src/Calculus/*.cfg

cppcheck:
	cppcheck --enable=all --std=c11 --suppress=missingIncludeSystem --check-config $(SOURCE) $(HEADER)

valgrind: test
ifeq ($(OS), Darwin)
	leaks --atExit -- test
else
	valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all /home/dtameka/C7_SmartCalc_v1.0-0/src/Calculus/test
endif

