#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  int child_id = fork();

  if (child_id == 0)
  {
    char **array;
    array = malloc(sizeof(char*)*(argc));

    int index;
    for (index = 0; index < argc - 1; ++index)
    {
      ++argv;
      array[index] = malloc( strlen(*argv)+1 ); // add one for the \0
      strcpy(array[index], *argv);
    }
    array[index] = '\0';
    exec(array[0], array);
  }
  else
  {
    wait();
  }
  exit();
}
