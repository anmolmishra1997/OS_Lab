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
	int final_child_pid = getpid();

	printf("Parent is : %d\n", parent_pid);
	printf("Number of children: %d\n", n);
	
	for(int i = 0; i < n; i++)
	{
		int child_pid = fork();
		if (child_pid == 0)
		{
			int current_pid = getpid();

			printf("Child %d is created\n", current_pid);
			srand(time(NULL) ^ (current_pid<<16));
			sleep(rand() % n + 1);
			break;
		}
		else
		{
			final_child_pid = child_pid;
			sleep(0.01/n);
		}
	}
	
	int current_pid = getpid();

	if (current_pid == parent_pid)
	{
		for(int i=0; i<n; i++)
		{
			int status = 0;
			waitpid(final_child_pid, &status, 0);
			printf("Child %d exited \n", final_child_pid);
			final_child_pid--;
		}
		printf("Parent exited\n");
	}

}