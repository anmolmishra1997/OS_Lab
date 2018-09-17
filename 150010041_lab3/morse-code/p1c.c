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
  pid_t pid, ppid;

  pid = wait(NULL);
  ppid = getpid();

  printf("Parent %d: Child %d is exited\n", ppid, pid);
}

void handle_child_creation()
{
    int pid = getpid();
    int ppid = getppid();
    printf("Child %d is created by parent %d\n", pid, ppid);
}

int main()
{    
    signal(SIGCHLD, sig_handler);
    int parent_pid = getpid();

    int child_pid = fork();

    if(child_pid == 0)
    {   
        handle_child_creation(10);
        signal(SIGCHLD, sig_handler);

        int child_pid = fork();
        if (child_pid == 0)
        {
            handle_child_creation(1);
            sleep(10);
        }
        else    
        {
            sleep(2);
            sleep(2);
        }
    }
    
    int current_pid = getpid();
    if (current_pid == parent_pid)
    {
        while(1);
    }
}