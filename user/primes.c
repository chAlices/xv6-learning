#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
generate_natural()
{
  int p[2];
  int start = 2;
  int end = 36;
  int i = 0;

  pipe(p);

  for (i = start; i < end; i++)
    write(p[1], &i, sizeof(int));

  close(p[1]);

  return p[0];
}

int
prime_filter(int in_fd, int prime)
{
  int i = 0;
  int p[2];

  pipe(p);
  if (fork() == 0) {
    close(p[0]);

    while (read(in_fd, &i, sizeof(int))) {
      if (i % prime != 0)
        write(p[1], &i, sizeof(int));
    }

    close(in_fd);
    close(p[1]);

    exit(0);
  }

  wait(0);
  close(in_fd);
  close(p[1]);

  return p[0];
}

int
main(int argc, char *argv[])
{
  int prime;

  int pipe_in = generate_natural();
  while (read(pipe_in, &prime, sizeof(int))) {
    printf("prime %d\n", prime);
    pipe_in = prime_filter(pipe_in, prime);
  }

  exit(0);
}
