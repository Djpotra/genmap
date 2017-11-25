TARGET=libgenmap.a
TESTS=tests
GSLIB=gslib
GSDIR ?= $(SRCROOT)/../gslib/src

CC=mpicc
CFLAGS=-std=c99 -O2 -Wall -Wextra -g
FC=mpif77
FFLAGS=
CXX=mpic++
CXXFLAGS=

SRCROOT =.
SRCDIR  =$(SRCROOT)/src
INCDIR  =$(SRCROOT)/inc
INCFLAGS=-I$(INCDIR) -I$(GSDIR)
TESTDIR =$(SRCROOT)/tests

CSRCS:=$(SRCDIR)/io.c $(SRCDIR)/lanczos.c $(SRCDIR)/linalg.c \
    $(SRCDIR)/csr.c $(TESTDIR)/test.c $(SRCDIR)/laplacian.c  \
    $(SRCDIR)/gswrapper.c

COBJS:=$(CSRCS:.c=.o)
FSRCS:=
FOBJS:=$(FSRCS:.f=.o)
LDFLAGS:=-lm -L$(GSDIR) -lgs

TESTCSRC:=$(TESTDIR)/readmap_test.c $(TESTDIR)/csr_test.c \
    $(TESTDIR)/vector_test.c $(TESTDIR)/lanczos_serial_test.c   \
    $(TESTDIR)/gs_test.c $(TESTDIR)/laplacian_test.c     \
    $(TESTDIR)/gop_test.c $(TESTDIR)/lanczos_parallel_test.c
TESTCOBJ:=$(TESTCSRC:.c=.o)
TESTFSRC:=
TESTFOBJ:=$(TESTFSRC:.f=.o)
TESTLDFLAGS:=-L. -lgenmap -Wl,-rpath=. -L$(GSDIR) -lgs -lm

SRCOBJS :=$(COBJS) $(FOBJS)
TESTOBJS:=$(TESTCOBJ) $(TESTFOBJ)

.PHONY: all
all: $(TARGET) $(TESTS)

#.PHONY: $(GSLIB)
#$(GSLIB): $(GSDIR)
#    cd $(GSDIR) && $(MAKE) -j -B
#MPI=1
#CC=$(CC) CFLAGS=$(CFLAGS) ADDUS=0 lib

.PHONY: $(TARGET)
$(TARGET): $(COBJS) $(FOBJS)
	@$(AR) cr $(TARGET) $(SRCOBJS)
	@ranlib $(TARGET)

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
