GCC=g++
STANDART=-std=c++11
CFLAGS=-Wall -Werror -Wextra -std=c++11
LFLAGS=-L . -lm -lpthread
PROGRAM_ONE_SRC=program1/main.cc program1/client/client.cc program1/encryptor/data_grabber.cc program1/encryptor/encrypter.cc
PROGRAM_ONE_HEADERS=program1/client/headers/client.h program1/encryptor/headers/data_grabber.h program1/encryptor/headers/encrypter.h \
program1/facade/programFacade.h
PROGRAM_TWO_HEADERS=program2/server/headers/server.h
PROGRAM_TWO_SRC=program2/main.cc program2/server/server.cc
OUTPUT_NAME_CLIENT=program_one
OUTPUT_NAME_SERVER=program_two
OUTPUTDIR=Applications
OS=$(shell uname -s)

all: clean uninstall install

install: program_one program_two
	rm -rf ../$(OUTPUTDIR)
	mkdir ../$(OUTPUTDIR)
	cp program_one ../$(OUTPUTDIR)
	cp program_two ../$(OUTPUTDIR)
	rm program_two program_one

program_one:
	$(GCC) $(CFLAGS) $(STANDART) $(PROGRAM_ONE_SRC) $(LFLAGS) -o $(OUTPUT_NAME_CLIENT)

program_two:
	$(GCC) $(STANDART) $(PROGRAM_TWO_SRC) $(LFLAGS) -o $(OUTPUT_NAME_SERVER)

clean:
	rm -rf *.o

uninstall:
	rm -rf ../$(OUTPUTDIR)/$(OUTPUT_NAME_CLIENT) ../$(OUTPUTDIR)/$(OUTPUT_NAME_SERVER)

style:
	@cp ../linter/.clang-format .clang-format
	@sleep 1
	clang-format -n $(PROGRAM_ONE_SRC) $(PROGRAM_TWO_SRC) $(PROGRAM_ONE_HEADERS) $(PROGRAM_TWO_HEADERS)
	@rm -f .clang-format
