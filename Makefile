TARGET=genmap.so
TESTS=tests

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
TESTDIR =$(SRCROOT)/tests

CSRCS:=$(SRCDIR)/rsb.c
COBJS:=$(CSRCS:.c=.o)
FSRCS:=
FOBJS:=$(FSRCS:.f=.o)
LDFLAGS:=-shared

TESTCSRC:=$(TESTDIR)/ax.c
TESTCOBJ:=$(TESTCSRC:.c=.o)
TESTFSRC:=
TESTFOBJ:=$(TESTFSRC:.f=.o)
TESTLDFLAGS:=-L . -lgenmap

SRCOBJS :=$(COBJS) $(FOBJS)
TESTOBJS:=$(TESTCOBJ) $(TESTFOBJ)

.PHONY: all
all: $(TARGET) $(TESTS)

$(TARGET): $(COBJS) $(FOBJS)
	$(CC) $(SRCOBJS) -o $@ $(LDFLAGS)

$(COBJS): %.o: %.c
	$(CC) $(CFLAGS) $(INCFLAGS) -c $< -o $@

$(FOBJS): %.o: %.f
	$(FC) $(FFLAGS) $(INCFLAGS) -c $< -o $@

.PHONY: tests
tests: $(TESTCOBJ) $(TESTFOBJ)

$(TESTCOBJ): %.o: %.c
	$(CC) $(CFLAGS) $(INCFLAGS) -c $< -o $@

$(TESTFOBJ): %.o: %.f
	$(FC) $(FFLAGS) $(INCFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(SRCOBJS) $(TESTOBJS) $(TARGET)
