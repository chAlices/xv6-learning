#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int i = 0;
  int sleep_time = 0;
  
  for (i = 1; i < argc; i++)
    sleep_time += atoi(argv[i]);

  sleep(sleep_time);

  exit(0);
}
