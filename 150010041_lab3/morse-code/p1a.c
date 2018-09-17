#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

void sig_handler(int signum)
{
	//SIGNINT is 2. SIGTERM is 15.
	if(signum == 2)
    	printf("\nInterrupt SIGINT received.\n");
    if(signum == 15)
    	printf("\nInterrupt SIGTERM received.\n");

    printf("Do you really want to quit (y|n)?\n");
    char input[2];
    scanf("%s", input);

    int ret = strcmp(input, "y");
    if(ret == 0)
    	exit(0);

}

int main()
{
    signal(SIGINT, sig_handler);
    signal(SIGTERM, sig_handler);
    while(1);

    return 0;
}