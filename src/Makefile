CC=gcc
CXX=g++
MAKE=make

CFLAGS = -std=c++11 -g -O0 -Wno-sign-compare
CFLAGS += -I "../../fmt"
CFLAGS += -I "../.."
CFLAGS += -I "../include"


CLIBS =
OBJS = flag.o log.o conf.o

all: ${OBJS}
	ar ru cjet.a ${OBJS} ${CLIBS}
	ranlib cjet.a

flag.o:
	${CXX} ${CFLAGS} -o flag.o -c flag.cc

log.o:
	${CXX} ${CFLAGS} -o log.o -c log.cc

conf.o:
	${CXX} ${CFLAGS} -o conf.o -c conf.cc

clean:
	rm -f ${EXE_SERVER} *.o *.a

