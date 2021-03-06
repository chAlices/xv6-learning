#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

int
main(int argc, char *argv[])
{
  if(argc < 2) {
    printf("usage: xargs cmd [...]\n");
    exit(1);
  }

  int flag = 1;
  char **newargv = malloc(sizeof(char *)*(argc + 2));
  memcpy(newargv, argv, sizeof(char *)*(argc + 1));
  newargv[argc + 1] = 0;

  while (1) {
    char buf[1024], *p = buf;
    while ((flag = read(0, p, sizeof(char))) && *p != '\n')
      p++;
    if (flag == 0)
      break;
    *p = 0;
    newargv[argc] = buf;
    if (fork() == 0) {
      exec(argv[1], newargv + 1);
      exit(0);
    }
    wait(0);
  }

  free(newargv);
  exit(0);
}
