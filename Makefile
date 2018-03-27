TARGET=genmap
LIB=lib$(TARGET).a
TESTS=test

GSLIB=gslib
GSDIR ?= $(SRCROOT)/../gslib/src
MPI ?= 1
DEBUG ?= 1

CC=mpicc
CFLAGS=-std=c99 -O2 -Wall -Wextra -Wno-unused-function -Wno-unused-parameter
FC=mpif77
FFLAGS=
CXX=mpic++
CXXFLAGS=

SRCROOT =.
SRCDIR  =$(SRCROOT)/src
INCDIR  =$(SRCROOT)/inc
INCFLAGS=-I$(INCDIR) -I$(GSDIR)
TESTDIR =$(SRCROOT)/tests

CSRCS:=$(SRCDIR)/genmap.c $(SRCDIR)/genmap-vector.c $(SRCDIR)/genmap-algo.c

COBJS:=$(CSRCS:.c=.o)
FSRCS:=
FOBJS:=$(FSRCS:.f=.o)
LDFLAGS:=-lm -L$(GSDIR) -lgs

TESTCSRC:=$(TESTDIR)/vector-test.c $(TESTDIR)/algo-test.c $(TESTDIR)/genmap-test.c

TESTCOBJ:=$(TESTCSRC:.c=.o)
TESTFSRC:=
TESTFOBJ:=$(TESTFSRC:.f=.o)
TESTLDFLAGS:=-L. -lgenmap -Wl,-rpath=. -L$(GSDIR) -lgs -lm

SRCOBJS :=$(COBJS) $(FOBJS)
TESTOBJS:=$(TESTCOBJ) $(TESTFOBJ)

ifeq ($(MPI),1)
	CFLAGS+= -DMPI
endif
ifeq ($(DEBUG),1)
	CFLAGS+= -DDEBUG
	CFLAGS+= -g3
endif

.PHONY: all
all: $(TARGET) $(TESTS)

#.PHONY: $(GSLIB)
#$(GSLIB): $(GSDIR)
#    cd $(GSDIR) && $(MAKE) -j -B
#MPI=1
#CC=$(CC) CFLAGS=$(CFLAGS) ADDUS=0 lib

.PHONY: $(TARGET)
$(TARGET): $(COBJS) $(FOBJS)
	@$(AR) cr $(LIB) $(SRCOBJS)
	@ranlib $(LIB)

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

.PHONY: astyle
astyle:
	astyle --style=google --indent=spaces=2 --max-code-length=80 \
	    --keep-one-line-statements --keep-one-line-blocks --lineend=linux \
            --suffix=none --preserve-date --formatted --pad-oper \
	    --unpad-paren tests/*.[ch] src/*.[ch] inc/*.[ch]
