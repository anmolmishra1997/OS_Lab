#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv)
{
	char* filename = argv[1];
	
	close(0);
	int fd = open(filename, O_RDONLY);
	printf("Parent: File opened. fd=%d.\n", fd);
	
	int child_pid = fork();

	if(child_pid == 0)
	{
		execl("./mycat", "mycat", (char *) NULL );
	}
	else
	{
		int status = 0;
		wait(&status);

		printf("Parent : The child has terminated.\n");
	}
}