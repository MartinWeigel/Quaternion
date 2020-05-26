.DEFAULT_GOAL := all

MAJOR := 0
MINOR := 1
NAME := Quaternion
VERSION := $(MAJOR).$(MINOR)

IDIR =../include
CC=gcc
CFLAGS=-I$(IDIR) -L${LDIR} -Wall

ODIR=../obj
LDIR =../lib

LIBS=-lm
SHAREDLIBS = -l$(NAME)

MKDIR_P = mkdir -p
BINDIR =../bin

LDFLAGS= -shared

_DEPS = %.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = %.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

_TEST = TestQuaternion.o
TEST = $(patsubst %,$(ODIR)/%,$(_TEST))

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

lib$(NAME).so.$(VERSION): ${ODIR}/$(NAME).o
	$(CC) $^ -o ${LDIR}/$@ $(CFLAGS) $(LIBS) $(LDFLAGS) -Wl,-soname,lib$(NAME).so.$(MAJOR) 

lib$(NAME).so: lib$(NAME).so.$(VERSION)
	ldconfig -v -n ${LDIR}
	ln -s ${LDIR}/lib$(NAME).so.$(MAJOR) ${LDIR}/lib$(NAME).so

lib: lib$(NAME).so.$(VERSION)

example: $(ODIR)/example.o
	$(CC) $(CFLAGS) -Wl,-rpath=${LDIR} -o ${BINDIR}/$@ $^ $(LIBS) $(SHAREDLIBS) 

test: $(TEST)
	$(CC) $(CFLAGS) -Wl,-rpath=${LDIR} -o ${BINDIR}/$@ $^ $(LIBS) $(SHAREDLIBS)
	${BINDIR}/$@

all: directories lib$(NAME).so example test

.PHONY: clean directories

directories: ${BINDIR} ${ODIR} ${LDIR}

${BINDIR}:
	${MKDIR_P} ${BINDIR}

${ODIR}:
	${MKDIR_P} ${ODIR}

${LDIR}:
	${MKDIR_P} ${LDIR}

clean:
	rm -f *~ $(INCDIR)/*~
	rm -f $(ODIR)/*.o
	rm -f $(LDIR)/*
	rm -r ${BINDIR} $(ODIR)