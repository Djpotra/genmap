MPI ?=1
VALGRIND ?=0
DEBUG ?=1
ASAN ?=0
UNDERSCORE ?=1
CC ?=mpicc
FC ?=mpif77

SRCROOT =.
GSDIR ?= $(SRCROOT)/../gslib

GSLIBDIR=$(GSDIR)/src
SRCDIR  =$(SRCROOT)/src
INCDIR  =$(SRCROOT)/inc
BUILDDIR=$(SRCROOT)/build
TESTDIR =$(SRCROOT)/tests

IODIR=$(SRCROOT)/io
DEFAULTDIR=$(IODIR)/default
FORTRANDIR=$(IODIR)/fortran
GMSHDIR=$(IODIR)/gmsh

CFLAGS= -std=c99 -O2 -Wall -Wextra -Wno-unused-function -Wno-unused-parameter
FFLAGS= -O2 -Wall -Wextra -Wno-unused-function -Wno-unused-parameter -cpp

TARGET=genmap
TEST=test
GSLIB=gslib
LIB=lib$(TARGET).a

AFLAGS = -fsanitize=address
INCFLAGS=-I$(INCDIR) -I$(GSLIBDIR) -I$(IODIR)
LDFLAGS:=-L$(GSLIBDIR) -lgs
TESTLDFLAGS:=-L. -Wl,-rpath=. -l$(TARGET) -L$(GSLIBDIR) -lgs -lm


# Main source and io #
CSRCS:= $(SRCDIR)/genmap-vector.c $(SRCDIR)/genmap-algo.c \
	$(SRCDIR)/genmap-io.c $(SRCDIR)/genmap-comm.c $(SRCDIR)/genmap.c \
	$(SRCDIR)/genmap-fortran.c
COBJS:=$(CSRCS:.c=.o)

FSRCS:=
FOBJS:=$(FSRCS:.f=.o)

IOSRCS = $(DEFAULTDIR)/genmap-default.c \
         $(FORTRANDIR)/genmap-fortran.c \
         $(GMSHDIR)/genmap-gmsh.c
IOOBJS = $(IOSRCS:.c=.o)

# Tests #
TESTCSRC:= $(TESTDIR)/vector-test.c $(TESTDIR)/algo-test.c \
	   $(TESTDIR)/genmap-test.c $(TESTDIR)/default-io-test.c \
	   $(TESTDIR)/gmsh-io-test.c $(TESTDIR)/gmsh-io1-test.c \
	   $(TESTDIR)/gmsh-io2-test.c $(TESTDIR)/fiedler-test.c \
	   $(TESTDIR)/fiedler1-test.c \
	   $(TESTDIR)/comm-test.c $(TESTDIR)/lanczos-test.c \
	   $(TESTDIR)/rsb-test.c
TESTCOBJ:=$(TESTCSRC:.c=.o)

TESTFSRC:= $(TESTDIR)/fortran-test.f
TESTFOBJ:=$(TESTFSRC:.f=.o)

# Build #
SRCOBJS :=$(COBJS) $(FOBJS) $(IOOBJS)
TESTOBJS:=$(TESTCOBJ) $(TESTFOBJ)

ifneq ($(ASAN),0)
	CFLAGS+= $(AFLAGS)
	FFLAGS+= $(AFLAGS)
	LDFLAGS+= $(AFLAGS)
	TESTLDFLAGS+= $(AFLAGS)
endif
ifneq ($(MPI),0)
	CFLAGS+= -DGENMAP_MPI
endif
ifneq ($(DEBUG),0)
	CFLAGS+= -DGENMAP_DEBUG
	CFLAGS+= -g3
endif
ifneq ($(UNDERSCORE),0)
	CFLAGS+= -DGENMAP_UNDERSCORE
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

$(IOOBJS): %.o: %.c
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
	    --unpad-paren tests/*.[ch] src/*.[ch] inc/*.[ch] io/*/*.[ch]

print-%:
	$(info [ variable name]: $*)
	$(info [        origin]: $(origin $*))
	$(info [         value]: $(value $*))
	$(info [expanded value]: $($*))
	$(info)
	@true
