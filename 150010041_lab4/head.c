#include "types.h"
#include "stat.h"
#include "user.h"

char buf[1];

void
cat(int fd, int num_lines)
{
  int n;
  int count = 0;

  while((n = read(fd, buf, sizeof(buf))) > 0 && count < num_lines) {
    if (write(1, buf, n) != n) {
      printf(1, "cat: write error\n");
      exit();
    }
    if(*buf == '\n')
      count++;
  }
  if(n < 0){
    printf(1, "cat: read error\n");
    exit();
  }
}

int
main(int argc, char *argv[])
{
  int fd, i;

  // if(argc <= 1){
  //   cat(0);
  //   exit();
  // }

  int num_lines = atoi(argv[1]);

  for(i = 2; i < argc; i++){
    if((fd = open(argv[i], 0)) < 0){
      printf(1, "cat: cannot open %s\n", argv[i]);
      exit();
    }
    printf(1, "--%s--\n", argv[i]);
    cat(fd, num_lines);
    close(fd);
  }
  exit();
}
