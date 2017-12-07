#include "mpiwrapper.h"
#include "io.h"

//------------------------------------------------------------------------------
int32 main(int32 argc, char** argv)
{
  struct comm global;

  init_genmap(&global, argc, argv);

  char *name = "nbrhd/nbrhd.map.bin";
  int32 *header, *glo_num, *elem_id;

  readmap(&global, &header, &glo_num, &elem_id, name);

#ifdef DEBUG
  for (int32 i = 0; i < HEADER_SIZE; i++)
  {
    printf("%d ", header[i]);
  }
  printf("\n");

  int32 i = 0;
  while (i < header[NC]*header[MYCHUNK])
  {
    printf("%d ", glo_num[i]);
    i++;
    if (i%header[NC] == 0) printf("\n");
  }
#endif

  finalize_genmap(&global);
   
  return 0;
}
