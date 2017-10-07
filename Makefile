TARGET=libgenmap.so
TESTS=tests
GSLIB=gslib

CC=mpicc
CFLAGS=-std=c99 -O2 -Wall -Wextra -g -fPIC
#TESTCFLAGS=-std=c99 -O2 -Wall -Wextra -g -DMPI
FC=mpif77
FFLAGS=
CXX=mpic++
CXXFLAGS=

SRCROOT =.
SRCDIR  =$(SRCROOT)/src
INCDIR  =$(SRCROOT)/inc
INCFLAGS=-I$(INCDIR) -I$(GSDIR)
TESTDIR =$(SRCROOT)/tests

GSDIR ?= $(SRCROOT)/../gslib/3rd_party/gslib/src

CSRCS:=$(SRCDIR)/io.c $(SRCDIR)/lanczos.c $(SRCDIR)/linalg.c $(SRCDIR)/csr.c $(TESTDIR)/test.c
COBJS:=$(CSRCS:.c=.o)
FSRCS:=
FOBJS:=$(FSRCS:.f=.o)
LDFLAGS:=-shared -lm -L$(GSDIR) -l:libgs.a

TESTCSRC:=$(TESTDIR)/readmap_test.c $(TESTDIR)/ax_test.c $(TESTDIR)/vector_test.c $(TESTDIR)/lanczos_test.c $(TESTDIR)/gs_test.c
TESTCOBJ:=$(TESTCSRC:.c=.o)
TESTFSRC:=
TESTFOBJ:=$(TESTFSRC:.f=.o)
TESTLDFLAGS:=-L. -lgenmap -Wl,-rpath=. -L$(GSDIR) -l:libgs.a

SRCOBJS :=$(COBJS) $(FOBJS)
TESTOBJS:=$(TESTCOBJ) $(TESTFOBJ)

.PHONY: all
all: $(GSLIB) $(TARGET) $(TESTS)

.PHONY: $(GSLIB)
$(GSLIB):

    $(MAKE) -j -B -C $(GSDIR) MPI=1 CC=$(CC) CFLAGS=$(CFLAGS) ADDUS=0 lib

.PHONY: $(TARGET)
$(TARGET): $(COBJS) $(FOBJS)
	$(CC) $(SRCOBJS) -o $@ $(LDFLAGS)

$(COBJS): %.o: %.c
	$(CC) $(CFLAGS) $(INCFLAGS) -c $< -o $@

$(FOBJS): %.o: %.f
	$(FC) $(FFLAGS) $(INCFLAGS) -c $< -o $@

.PHONY: $(TESTS)
$(TESTS): $(TESTCOBJ) $(TESTFOBJ)

$(TESTCOBJ): %.o: %.c
	$(CC) $(CFLAGS) $(INCFLAGS) $< -o $@ $(TESTLDFLAGS)

$(TESTFOBJ): %.o: %.f
	$(FC) $(FFLAGS) $(INCFLAGS) $< -o $@ $(TESTLDFLAGS)

.PHONY: clean
clean:
	rm -f $(SRCOBJS) $(TESTOBJS) $(TARGET)
