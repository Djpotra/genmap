TARGET=libgenmap.so
TESTS=tests

CC=gcc
CFLAGS=-std=c99 -O2 -Wall -Wextra -g -fPIC
FC=gfortran
FFLAGS=
CXX=g++
CXXFLAGS=
GSDIR=${HOME}/Nek5000/3rd_party/gslib/src

SRCROOT =.
SRCDIR  =$(SRCROOT)/src
INCDIR  =$(SRCROOT)/inc
INCFLAGS=-I$(INCDIR) -I$(GSDIR)
TESTDIR =$(SRCROOT)/tests

CSRCS:=$(SRCDIR)/lanczos.c $(SRCDIR)/linalg.c $(SRCDIR)/csr.c $(TESTDIR)/test.c
COBJS:=$(CSRCS:.c=.o)
FSRCS:=
FOBJS:=$(FSRCS:.f=.o)
LDFLAGS:=-shared -lm -L${GSDIR} -lgs

TESTCSRC:=$(TESTDIR)/ax_test.c $(TESTDIR)/vector_test.c $(TESTDIR)/lanczos_test.c $(TESTDIR)/gs_test.c
TESTCOBJ:=$(TESTCSRC:.c=.o)
TESTFSRC:=
TESTFOBJ:=$(TESTFSRC:.f=.o)
TESTLDFLAGS:=-L. -lgenmap -Wl,-R.

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
	$(CC) $(CFLAGS) $(INCFLAGS) $< -o $@ $(TESTLDFLAGS)

$(TESTFOBJ): %.o: %.f
	$(FC) $(FFLAGS) $(INCFLAGS) $< -o $@ $(TESTLDFLAGS)

.PHONY: clean
clean:
	rm -f $(SRCOBJS) $(TESTOBJS) $(TARGET)
