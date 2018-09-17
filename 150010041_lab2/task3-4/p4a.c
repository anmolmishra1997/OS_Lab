#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

void writetofile(char* buf, int fd)
{
	int length = strlen(buf);

	if(write(fd, buf, length) != length)
   	{ 
       fprintf(stdout, "write error\n"); 
	   exit(-1); 
   	}  	
}

int main(int argc, char** argv)
{
	char* filename = argv[1];

	int fd = open(filename, O_WRONLY | O_CREAT);
	printf("Parent: File opened. fd=%d.\n", fd);

	int child_pid = fork();

	if(child_pid == 0)
	{
		int self_pid = getpid();
		printf("Child %d: writing to file %d\n", self_pid, fd);
		
		char* buf = "hello world! I am the child\n";
		writetofile(buf, fd);
	}
	else
	{
		int self_pid = getpid();
		printf("Parent %d: writing to file %d\n", self_pid, fd);

		char* buf = "hello world! I am the parent\n";
		writetofile(buf, fd);

		int status = 0;
		wait(&status);

		printf("Parent : The child has terminated.\n");

	}
}