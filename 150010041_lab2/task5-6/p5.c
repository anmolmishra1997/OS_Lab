#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
	int parent_pid = getpid();
	int child_pid = fork();

	if (child_pid == 0)
	{
		printf("Child : My process ID is: %d\n", getpid());
		printf("Child : The parent process ID is: %d\n", getppid());
		sleep(2);
		printf("Child : My process ID is: %d\n", getpid());		
		printf("Child : The parent process ID is: %d\n", getppid());
	}
	else
	{
		printf("Parent : My process ID is: %d\n", getpid());
		printf("Parent : The child process ID is: %d\n", child_pid);
		sleep(1);
		printf("Parent : The parent has terminated.\n");
	}
}