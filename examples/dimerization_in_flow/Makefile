# Path to the epdm directory - change this if your model folder is not at examples/
epdmDir         = ../..

# Name of the executable
epdmExecName   = epdm

# List of objects specific to this model - append your objects here
modelObjects    = model.o main.o

# Loading list of library objects and prepending the epdm directory to each entry
include ${epdmDir}/Makefile.lib
libraryObjects  = $(foreach object, ${localLibraryObjects}, ${epdmDir}/${object})

.SUFFIXES       = .cpp .o
CC              = g++
LDFLAGS         = -g -ggdb -Wall
CFLAGS          = -std=c++11 -O2 -g -ggdb -Wall -I"." -I"${epdmDir}"
CPPFLAGS        = ${CFLAGS}

.cpp.o:
	${CC} -o $@ -c ${CPPFLAGS} $<

${epdmExecName}: ${libraryObjects} ${modelObjects}
	${CC} ${LDFLAGS} -o $@ $^

clean:
	${RM} ${libraryObjects} ${modelObjects} ${epdmExecName}

all: ${epdmExecName}
