#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>


void createChildProcess(int n)
{
	if (n==0)
		return;
	int child_pid = fork();
	if (child_pid == 0)
	{
		printf("Child %d is created\n", getpid());
		createChildProcess(n-1);
	}
	else
	{
		int status = 0;
		wait(&status);
	}
}

int main(int argc, char** argv)
{
	int n = atoi(argv[1]);

	int parent_pid = getpid();

	printf("Parent is : %d\n", parent_pid);
	printf("Number of children: %d\n", n);
	
	createChildProcess(n);
	int current_pid = getpid();
	if (current_pid != parent_pid)
		printf("Child %d exited \n", current_pid);
	else
		printf("Parent exited\n");

}