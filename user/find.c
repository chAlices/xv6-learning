#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char*
fmtname(char *path)
{
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  return p;
}

void
find(char *path, char*name)
{
  char buf[1024], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, 0)) < 0) { // 1. open this file, we get fd
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0) { // 2. use fstat to determine this path is file or directory
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type){
  case T_FILE:
    if (strcmp(fmtname(path), name) == 0) // back up from the path end to look for the file name
      printf("%s\n", path);
    break;

  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf)) {
      printf("find: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)) { // because it's directory file, so we can use
                                                     // dirent to get all the file massage under 
                                                     // this directory
      if(de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ); // be careful for the different between memmove and memcpy.
      p[DIRSIZ] = 0;
      if (!strcmp(de.name, ".") || !strcmp(de.name, ".."))
        continue;
      find(buf, name);
    }
    break;
  }
  close(fd);
}

int
main(int argc, char *argv[])
{
  if(argc == 2) {
    find(".", argv[1]);
    exit(0);
  } else if (argc == 3) {
    find(argv[1], argv[2]);
    exit(0);
  } else {
    printf("usage: 'find [Dir] [File Name]' or 'find [File Name]'\n");
    exit(0);
  }
}
