.DEFAULT_GOAL := all

MAJOR := 0
MINOR := 1
NAME := Quaternion
VERSION := $(MAJOR).$(MINOR)

BDIR = build
SDIR = src
IDIR = include
CC = gcc
CFLAGS = -I$(IDIR) -L${LDIR}/ -Wall -Werror -fpic

ODIR = $(BDIR)/obj
LDIR = $(BDIR)/lib

LIBS=-lm
SHAREDLIBS = -l$(NAME)

MKDIR_P = mkdir -p
BINDIR = $(BDIR)/bin

LDFLAGS= -shared -Wl,-soname,lib$(NAME).so.$(MAJOR)

_DEPS = %.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = example.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

_TEST = TestQuaternion.o
TEST = $(patsubst %,$(ODIR)/%,$(_TEST))

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

lib$(NAME).so.$(VERSION): ${ODIR}/$(NAME).o
	$(CC) $^ -o ${LDIR}/$@ $(CFLAGS) $(LIBS) $(LDFLAGS) 

lib$(NAME).so: lib$(NAME).so.$(VERSION)
	ldconfig -n ${LDIR} && \
	ln -s ./${build}/lib$(NAME).so.$(MAJOR) ${LDIR}/lib$(NAME).so

lib: lib$(NAME).so.$(VERSION)

example: $(OBJ)
	$(CC) $(CFLAGS) -o ${BINDIR}/$@ $^ $(LIBS) $(SHAREDLIBS) 

test: $(TEST)
	$(CC) $(CFLAGS) -o ${BINDIR}/$@ $^ $(LIBS) $(SHAREDLIBS) && \
	LD_LIBRARY_PATH=${LDIR}:$LD_LIBRARY_PATH ${BINDIR}/$@

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
	rm -f $(INCDIR)/*~ $(SDIR)/*~  && \
	rm -f $(ODIR)/*.o $(LDIR)/*.so.* ${BINDIR}/* && \
	rm -r ${BINDIR} $(ODIR) $(LDIR) $(BDIR)