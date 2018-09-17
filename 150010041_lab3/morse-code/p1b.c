#include <stdio.h>
#include <time.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

void sig_handler(int signum)
{
  pid_t pid;

  
  pid = wait(NULL);

  printf("Child %d is exited.\n", pid);
}

int main(int argc, char** argv)
{    
    signal(SIGCHLD, sig_handler);
    int n = atoi(argv[1]);

    int parent_pid = getpid();
    
    for(int i = 0; i < n; i++)
    {
        int child_pid = fork();
        if (child_pid == 0)
        {
            printf("Child %d is created ", getpid());
            srand(time(NULL) ^ (getpid()<<16));
            int sleep_time = rand() % n + 1;
            printf("(sleeps for %d seconds)\n", sleep_time);
            sleep(sleep_time);
            break;
        }
        else
        {
            sleep(0.01/n);
        }
    }

    int current_pid = getpid();
    if (current_pid == parent_pid)
    {
        while(1);
    }

}