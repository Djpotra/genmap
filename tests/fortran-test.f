      program testapi
#ifdef MPI
      implicit none

      include 'mpif.h'

      integer ierror,np,nid
      integer vertices(8)
      integer h

      call mpi_init(ierror)
      call mpi_comm_size(mpi_comm_world,np ,ierror)
      call mpi_comm_rank(mpi_comm_world,nid,ierror)

      call genmapinit(h,mpi_comm_world,0,ierror)
      call genmapset(h,2,3,4,4,vertices,ierror)
      call genmapfinalize(h,ierror)

      call mpi_finalize(ierror)
#endif
      end
