MPI ?=1
VALGRIND ?=1
DEBUG ?=0
ASAN ?=0

SRCROOT =.
GSDIR ?= $(SRCROOT)/../../gslib/gslib

GSLIBDIR=$(GSDIR)/src
SRCDIR  =$(SRCROOT)/src
INCDIR  =$(SRCROOT)/inc
BUILDDIR=$(SRCROOT)/build
TESTDIR =$(SRCROOT)/tests

READERDIR=$(SRCROOT)/readers
DEFAULTDIR=$(READERDIR)/default
FORTRANDIR=$(READERDIR)/fortran

ifeq ($(MPI),0)
CC=gcc
endif
CFLAGS= -std=c99 -O2 -Wall -Wextra -Wno-unused-function -Wno-unused-parameter

ifeq ($(MPI),0)
FC=gfortran
endif
FFLAGS=

ifeq ($(MPI),0)
CXX=g++
endif
CXXFLAGS=

TARGET=genmap
TEST=test
GSLIB=gslib
LIB=lib$(TARGET).a

AFLAGS = -fsanitize=address
INCFLAGS=-I$(INCDIR) -I$(GSLIBDIR) -I$(READERDIR)
LDFLAGS:=-L$(GSLIBDIR) -lgs
TESTLDFLAGS:=-L. -Wl,-rpath=. -l$(TARGET) -L$(GSLIBDIR) -lgs -lm


# Main source and readers #
CSRCS:= $(SRCDIR)/genmap-vector.c $(SRCDIR)/genmap-algo.c \
	$(SRCDIR)/genmap-io.c $(SRCDIR)/genmap-comm.c $(SRCDIR)/genmap.c \
	$(SRCDIR)/genmap-fortran.c
COBJS:=$(CSRCS:.c=.o)

FSRCS:=
FOBJS:=$(FSRCS:.f=.o)

READERSRCS = $(DEFAULTDIR)/genmap-default.c $(DEFAULTDIR)/genmap-default-io.c \
             $(FORTRANDIR)/genmap-fortran.c $(FORTRANDIR)/genmap-fortran-io.c
READEROBJS = $(READERSRCS:.c=.o)

# Tests #
TESTCSRC:= $(TESTDIR)/vector-test.c $(TESTDIR)/algo-test.c \
	   $(TESTDIR)/genmap-test.c $(TESTDIR)/io-test.c \
	   $(TESTDIR)/comm-test.c $(TESTDIR)/lanczos-test.c \
	   $(TESTDIR)/rsb-test.c
TESTCOBJ:=$(TESTCSRC:.c=.o)

TESTFSRC:=
TESTFOBJ:=$(TESTFSRC:.f=.o)

# Build #
SRCOBJS :=$(COBJS) $(FOBJS) $(READEROBJS)
TESTOBJS:=$(TESTCOBJ) $(TESTFOBJ)

ifeq ($(ASAN),1)
	CFLAGS+= $(AFLAGS)
	FFLAGS+= $(AFLAGS)
	LDFLAGS+= $(AFLAGS)
	TESTLDFLAGS+= $(AFLAGS)
endif
ifeq ($(MPI),1)
	CFLAGS+= -DMPI
endif
ifeq ($(DEBUG),1)
	CFLAGS+= -DDEBUG
	CFLAGS+= -g3
endif

.PHONY: all
all: $(TARGET) $(TEST)

.PHONY: $(TARGET)
$(TARGET): $(SRCOBJS)
#	$(CC) $(LDFLAGS) -shared -o $(LIB) $(SRCOBJS)
	@$(AR) cr $(LIB) $(SRCOBJS)
	@ranlib $(LIB)

$(COBJS): %.o: %.c
	$(CC) $(CFLAGS) $(INCFLAGS) -c $< -o $@

$(FOBJS): %.o: %.f
	$(FC) $(FFLAGS) $(INCFLAGS) -c $< -o $@

$(READEROBJS): %.o: %.c
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
	make clean -C $(GSDIR)
	export CC=$(CC) && make MPI=$(MPI) -C $(GSDIR)

.PHONY: clean
clean:
	@rm -f $(SRCOBJS) $(TESTOBJS) $(TARGET)

.PHONY: astyle
astyle:
	astyle --style=google --indent=spaces=2 --max-code-length=72 \
	    --keep-one-line-statements --keep-one-line-blocks --lineend=linux \
            --suffix=none --preserve-date --formatted --pad-oper \
	    --unpad-paren tests/*.[ch] src/*.[ch] inc/*.[ch] readers/*/*.[ch]
