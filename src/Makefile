IDIR =../include
CC=gcc
CFLAGS=-I$(IDIR)

ODIR=obj
LDIR =../lib

LIBS=-lm

_DEPS = example.h Quaternion.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = example.o Quaternion.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

_TEST = TestQuaternion.o Quaternion.o
TEST = $(patsubst %,$(ODIR)/%,$(_TEST))

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

example: $(OBJ)
	$(CC) -o ./bin/$@ $^ $(CFLAGS) $(LIBS)

unittest: $(TEST)
	$(CC) -o ./bin/$@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ ./bin/example ./bin/unittest $(INCDIR)/*~ 