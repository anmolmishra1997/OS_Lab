#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
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

	return 0;
}