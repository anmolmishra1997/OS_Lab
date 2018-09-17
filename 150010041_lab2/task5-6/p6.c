#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>


int main()
{
	int parent_pid = getpid();
	int child_pid = fork();

	if (child_pid == 0)
	{
		printf("Child : The child's process ID is : %d\n", getpid());
		printf("Child : The parent process ID is : %d\n", getppid());
		getchar();
	}
	else
	{
		printf("Parent : The child's process ID is: %d\n", child_pid);
		printf("Parent : The process ID is : %d\n", getpid());
		sleep(60);

		int status = 0;
		wait(&status);

		printf("Parent : The parent has terminated.\n");
	}
}