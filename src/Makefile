.DEFAULT_GOAL := all

IDIR =../include
CC=gcc
CFLAGS=-I$(IDIR)

ODIR=obj
LDIR =../lib

LIBS=-lm

MKDIR_P = mkdir -p
BINDIR = bin

_DEPS = example.h Quaternion.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = example.o Quaternion.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

_TEST = TestQuaternion.o Quaternion.o
TEST = $(patsubst %,$(ODIR)/%,$(_TEST))

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

example: $(OBJ)
	$(CC) -o ${BINDIR}/$@ $^ $(CFLAGS) $(LIBS)

test: $(TEST)
	$(CC) -o ${BINDIR}/$@ $^ $(CFLAGS) $(LIBS)
	${BINDIR}/$@

all: directories example test

.PHONY: clean directories

directories: ${BINDIR} ${ODIR}

${BINDIR}:
	${MKDIR_P} ${BINDIR}

${ODIR}:
	${MKDIR_P} ${ODIR}

clean:
	rm -f *~ $(INCDIR)/*~
	rm -f $(ODIR)/*.o
	rm -r ${BINDIR} $(ODIR)