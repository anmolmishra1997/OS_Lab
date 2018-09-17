#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
	int parent_pid = getpid();
	int child_pid = fork();

	if(child_pid == 0)
	{
		printf("Child : My process ID is: %d\n", getpid());
		printf("Child : The parent process ID is: %d\n", parent_pid);
	}
	else
	{
		printf("Parent : My process ID is: %d\n", parent_pid);
		printf("Parent : The child process ID is: %d\n", child_pid);
	}

	return 0;
}