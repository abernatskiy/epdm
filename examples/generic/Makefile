# List of objects specific to this model - append your objects here
modelObjects    = model.o main.o

# Path to the epdm directory - change this if your model is outside of the examples folder
epdmDir    = ../..

# Loading list of library objects and prepending the epdm directory to each entry
include ${epdmDir}/Makefile.lib
libraryObjects = $(foreach object, ${localLibraryObjects}, ${epdmDir}/${object})

.SUFFIXES       = .cpp .o
CC              = g++
LDFLAGS         = -g -ggdb -Wall
CFLAGS          = -std=c++11 -O2 -g -ggdb -Wall -I"." -I"${epdmDir}"
CPPFLAGS        = ${CFLAGS}

.cpp.o:
	${CC} -o $@ -c ${CPPFLAGS} $<

pdmmod: ${libraryObjects} ${modelObjects}
	${CC} ${LDFLAGS} -o $@ $^

clean:
	${RM} ${libraryObjects} ${modelObjects} pdmmod

all: pdmmod
