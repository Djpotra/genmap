TARGET=genmap

CC=gcc
CFLAGS=-std=c99 -O2 -Wall -Wextra -g
FC=gfortran
FFLAGS=
CXX=g++
CXXFLAGS=

SRCROOT =.
SRCDIR  =$(SRCROOT)/src
INCDIR  =$(SRCROOT)/inc
INCFLAGS=-I$(INCDIR)
LDFLAGS =

FSRCS =
FOBJS:=$(FSRCS:.f=.o)
CSRCS =$(SRCDIR)/rsb.c
COBJS:=$(CSRCS:.c=.o)
OBJS :=$(COBJS) $(FOBJS)

.PHONY: all
all: $(TARGET)

$(TARGET): $(COBJS) $(FOBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

$(COBJS): %.o: %.c
	$(CC) $(CFLAGS) $(INCFLAGS) -c $< -o $@

$(FOBJS): %.o: %.f
	$(FC) $(FFLAGS) $(INCFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJS) $(TRAGET)
