TARGET=genmap
TEST=test
GSLIB=gslib

LIB=lib$(TARGET).a

GSDIR ?= $(SRCROOT)/../$(GSLIB)
GSLIBDIR=$(GSDIR)/src

MPI ?= 1
VALGRIND ?= 0
DEBUG ?= 1

CC=mpicc
CFLAGS= -std=c99 -O2 -Wall -Wextra -Wno-unused-function -Wno-unused-parameter
FC=mpif77
FFLAGS=
CXX=mpic++
CXXFLAGS=

SRCROOT =.
SRCDIR  =$(SRCROOT)/src
INCDIR  =$(SRCROOT)/inc
READERSDIR=$(SRCROOT)/readers
BUILDDIR=$(SRCROOT)/build
TESTDIR =$(SRCROOT)/tests
DEFAULTDIR=$(READERSDIR)/default

CSRCS:= $(SRCDIR)/genmap-vector.c $(SRCDIR)/genmap-algo.c \
	$(SRCDIR)/genmap-io.c $(SRCDIR)/genmap-comm.c $(SRCDIR)/genmap.c
COBJS:=$(CSRCS:.c=.o)

FSRCS:=
FOBJS:=$(FSRCS:.f=.o)

DEFAULTSRCS = $(DEFAULTDIR)/default.c $(DEFAULTDIR)/default-comm.c \
	      $(DEFAULTDIR)/default-io.c
DEFAULTOBJS = $(DEFAULTSRCS:.c=.o)

TESTCSRC:= $(TESTDIR)/vector-test.c $(TESTDIR)/algo-test.c \
	   $(TESTDIR)/genmap-test.c $(TESTDIR)/io-test.c $(TESTDIR)/ax-test.c \
	   $(TESTDIR)/lanczos-test.c
TESTCOBJ:=$(TESTCSRC:.c=.o)
TESTFSRC:=
TESTFOBJ:=$(TESTFSRC:.f=.o)


SRCOBJS :=$(COBJS) $(FOBJS) $(DEFAULTOBJS)
TESTOBJS:=$(TESTCOBJ) $(TESTFOBJ)

INCFLAGS=-I$(INCDIR) -I$(GSLIBDIR) -I$(DEFAULTDIR)
LDFLAGS:=-L$(GSLIBDIR) -lgs
TESTLDFLAGS:=-L. -Wl,-rpath=. -l$(TARGET) -L$(GSLIBDIR) -lgs -lm

ifeq ($(MPI),1)
	CFLAGS+= -DMPI
endif
ifeq ($(DEBUG),1)
	CFLAGS+= -DDEBUG
	CFLAGS+= -g3
endif

.PHONY: all
all: $(GSLIB) $(TARGET) $(TEST)

.PHONY: $(TARGET)
$(TARGET): $(SRCOBJS)
#	$(CC) $(LDFLAGS) -shared -o $(LIB) $(SRCOBJS)
	@$(AR) cr $(LIB) $(SRCOBJS)
	@ranlib $(LIB)

$(COBJS): %.o: %.c
	$(CC) $(CFLAGS) $(INCFLAGS) -c $< -o $@

$(FOBJS): %.o: %.f
	$(FC) $(FFLAGS) $(INCFLAGS) -c $< -o $@

$(DEFAULTOBJS): %.o: %.c
	$(CC) $(CFLAGS) $(INCFLAGS) -c $< -o $@

.PHONY: $(TEST)
$(TEST): $(TESTCOBJ) $(TESTFOBJ)
	@cd $(TESTDIR) && ./run-tests.sh $(MPI) $(VALGRIND)

$(TESTCOBJ): %.o: %.c
	$(CC) $(CFLAGS) $(INCFLAGS) $< -o $@ $(TESTLDFLAGS)

$(TESTFOBJ): %.o: %.f
	$(FC) $(FFLAGS) $(INCFLAGS) $< -o $@ $(TESTLDFLAGS)

.PHONY: $(GSLIB)
$(GSLIB):
	make CC=$(CC) MPI=$(MPI) -C $(GSDIR)

.PHONY: clean
clean:
	@rm -f $(SRCOBJS) $(TESTOBJS) $(TARGET)

.PHONY: astyle
astyle:
	astyle --style=google --indent=spaces=2 --max-code-length=80 \
	    --keep-one-line-statements --keep-one-line-blocks --lineend=linux \
            --suffix=none --preserve-date --formatted --pad-oper \
	    --unpad-paren tests/*.[ch] src/*.[ch] inc/*.[ch]
