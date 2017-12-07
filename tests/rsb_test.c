#include "mpiwrapper.h"

//------------------------------------------------------------------------------
int32 main(int32 argc, char** argv)
{
  struct comm global;

  init_genmap(&global, argc, argv);

  finalize_genmap(&global);
   
  return 0;
}
