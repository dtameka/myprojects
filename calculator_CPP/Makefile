.PHONY: install test
CFLAGS=-Wall -Werror -Wextra -std=c11
GCOV=-fprofile-arcs -ftest-coverage
PROJECT=Calculus
SOURCE=s21_test.cpp s21_smartcalc.cpp s21_creditcalc.cpp s21_depositcalc.cpp
HEADER=s21_smartcalc.h s21_creditcalc.h s21_depositcalc.h
OBJECT=s21_smartcalc.o s21_creditcalc.o s21_depositcalc.o
LIB=s21_smartcalc.a s21_depositcalc.a s21_creditcalc.a
BUILD=Calculus
OUTPUTDIR=Application
ADDITIONAL=mainwindow.cpp s21_controller.cpp s21_graphics.cpp mainwindow.h s21_controller.h s21_graphics.h

OS=$(shell uname -s)
SHELL:=/bin/bash
ifeq ($(OS), Darwin)
	CHECKFLAG=-lgtest
	QMAKEDIR=$(HOME)/Qt/6.2.4/macos/bin/qmake
	OUTPUTFILE=Calculus.app
	OPEN=open
else
	CHECKFLAG=-L . -lgtest -lm -lsubunit -lrt -lpthread
#	QMAKEDIR=$(HOME)/Qt/6.3.0/gcc_64/bin/qmake
#	QMAKEDIR=$(shell which qmake)
#	QMAKEDIR=$(HOME)/Qt/6.4.0/gcc_64/bin/qmake
	QMAKEDIR=/usr/lib/qt5/bin/qmake
	OUTPUTFILE=Calculus
	OPEN=xdg-open
endif

default: all

all: clean install dvi

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
	rm -rf Dist/

dist: uninstall install
	tar -czf Calculus.install.tar.gz $(OUTPUTDIR)/$(PROJECT)
	mkdir Dist
	cp Calculus.install.tar.gz Dist

dvi:
	$(OPEN) ./Documentation/index.html	

test: clean
	g++ $(GCOV) -c s21_smartcalc.cpp
	ar rcs s21_smartcalc.a s21_smartcalc.o
	g++ $(GCOV) -c s21_creditcalc.cpp
	ar rcs s21_creditcalc.a s21_creditcalc.o
	g++ $(GCOV) -c s21_depositcalc.cpp
	ar rcs s21_depositcalc.a s21_depositcalc.o
	g++ -c s21_test.cpp
	g++ $(GCOV) s21_test.o s21_smartcalc.a s21_creditcalc.a s21_depositcalc.a -o test $(CHECKFLAG)
	./test

clean:
	rm -f *.gcda *.gcno *.info gcov_report 
	rm -f s21_smartcalc_test *.out *.exe cpplint.py CPPLINT.cfg *.h.gch *.o *.a
	rm -rf build*
	rm -f *.autosave
	rm -rf makefile_qmake ./.qmake.stash ./makefile_qmake ./ui_*.*
	rm -rf Report/ build* ./test .clang-format
	rm -f moc_*.* ui_*.* *.o *.info qrc_*.*
	rm -f *~ core *.core *.tar.gz

style:
	@cp ../../materials/linters/.clang-format .clang-format
	@sleep 1
	clang-format -n $(SOURCE) $(HEADER) $(ADDITIONAL)
	@rm -f .clang-format

valgrind: test
ifeq ($(OS), Darwin)
	leaks --atExit -- test
else
	valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all ./test
endif

