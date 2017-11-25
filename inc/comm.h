#ifndef _COMM_H_
#define _COMM_H_

/* requires:
     <stddef.h>            for size_t
     <stdlib.h>            for exit
*/

/*
  When the preprocessor macro MPI is defined, defines (very) thin wrappers
  for the handful of used MPI routines. Alternatively, when MPI is not defined,
  these wrappers become dummy routines suitable for a single process run.
  No code outside of "comm.h" and "comm.c" makes use of MPI at all.

  Basic usage:
  
    struct comm c;
  
    comm_init(&c, MPI_COMM_WORLD);  // initializes c using MPI_Comm_dup

    comm_free(&c);
*/

#ifdef MPI
#include <mpi.h>
typedef MPI_Comm comm_ext;
typedef MPI_Request comm_req;
#else
typedef int comm_ext;
typedef int comm_req;
typedef int MPI_Fint;
#endif

/* global id, np vars strictly for diagnostic messages (fail.c) */
#ifndef comm_gbl_id
#define comm_gbl_id PREFIXED_NAME(comm_gbl_id)
#define comm_gbl_np PREFIXED_NAME(comm_gbl_np)
extern uint comm_gbl_id, comm_gbl_np;
#endif

struct comm {
  uint id, np;
  comm_ext c;
};

static void comm_init(struct comm *c, comm_ext ce);
/* (macro) static void comm_init_check(struct comm *c, MPI_Fint ce, uint np); */
/* (macro) static void comm_dup(struct comm *d, const struct comm *s); */
static void comm_free(struct comm *c);
static double comm_time(void);
static void comm_barrier(const struct comm *c);

/*----------------------------------------------------------------------------
  Code for static (inline) functions
  ----------------------------------------------------------------------------*/

static void comm_init(struct comm *c, comm_ext ce)
{
#ifdef MPI
  int i;
  MPI_Comm_dup(ce, &c->c);
  MPI_Comm_rank(c->c,&i), comm_gbl_id=c->id=i;
  MPI_Comm_size(c->c,&i), comm_gbl_np=c->np=i;
#else
  c->id = 0, c->np = 1;
#endif
}

static void comm_init_check_(struct comm *c, MPI_Fint ce, uint np,
                             const char *file, unsigned line)
{
#ifdef MPI
  comm_init(c,MPI_Comm_f2c(ce));
  if(c->np != np)
    fail(1,file,line,"comm_init_check: passed P=%u, "
                     "but MPI_Comm_size gives P=%u",
                     (unsigned)np,(unsigned)c->np);
#else
  comm_init(c,0);
  if(np != 1)
    fail(1,file,line,"comm_init_check: passed P=%u, "
                     "but not compiled with -DMPI",(unsigned)np);
#endif
}
#define comm_init_check(c,ce,np) comm_init_check_(c,ce,np,__FILE__,__LINE__)


static void comm_dup_(struct comm *d, const struct comm *s,
                      const char *file, unsigned line)
{
  d->id = s->id, d->np = s->np;
#ifdef MPI
  MPI_Comm_dup(s->c,&d->c);
#else
  if(s->np!=1) fail(1,file,line,"%s not compiled with -DMPI\n",file);
#endif
}
#define comm_dup(d,s) comm_dup_(d,s,__FILE__,__LINE__)

static void comm_free(struct comm *c)
{
#ifdef MPI
  MPI_Comm_free(&c->c);
#endif
}

static double comm_time(void)
{
#ifdef MPI
  return MPI_Wtime();
#else
  return 0;
#endif
}

static void comm_barrier(const struct comm *c)
{
#ifdef MPI
  MPI_Barrier(c->c);
#endif
}

#endif
