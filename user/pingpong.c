#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int p[2];
  char buf[512];
  int pid;

  pipe(p);
  if (fork() == 0) {  // child
    read(p[0], buf, sizeof(buf));
    close(p[0]);

    pid = getpid();
    printf("%d: received %s\n", pid, buf);

    write(p[1], "pong", 4);
    close(p[1]);

    exit(0);
  } else {
    write(p[1], "ping", 4);
    close(p[1]);

    wait(0); //wait child for exiting

    read(p[0], buf, sizeof(buf));
    close(p[0]);

    pid = getpid();
    printf("%d: received %s\n", pid, buf);
  }
  exit(0);
}
