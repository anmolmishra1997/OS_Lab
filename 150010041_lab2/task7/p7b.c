#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char** argv)
{

	int n = atoi(argv[1]);

	int parent_pid = getpid();

	printf("Parent is : %d\n", parent_pid);
	printf("Number of children: %d\n", n);
	
	for(int i = 0; i < n; i++)
	{
		int child_pid = fork();
		if (child_pid == 0)
		{
			printf("Child %d is created\n", getpid());
			srand(time(NULL) ^ (getpid()<<16));
			sleep(rand() % n + 1);
			break;
		}
		else
		{
			sleep(0.01/n);
		}
	}
	int current_pid = getpid();
	if (current_pid != parent_pid)
	{
		printf("Child %d exited \n", current_pid);
	}
	else
	{
		for(int i=0; i<n; i++)
		{
			int status = 0;
			wait(&status);
		}
		printf("Parent exited\n");
	}

}