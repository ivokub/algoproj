BINDIR=../bin
IDIR =../include
CC=gcc
CFLAGS=-I$(IDIR)
FLAGS=-g -ggdb

ODIR=../build

LIBS=-lm

_DEPS = struct.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = coo.o csr.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
		$(CC) $(FLAGS) -c -o $@ $< $(CFLAGS)

sparse: $(OBJ)
		gcc $(FLAGS) -o $(BINDIR)/$@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
		rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 
