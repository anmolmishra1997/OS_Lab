#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>

#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64

char **tokenize(char *line)
{
    char **tokens = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
    char *token = (char *)malloc(MAX_TOKEN_SIZE * sizeof(char));
    int i, tokenIndex = 0, tokenNo = 0;

    for(i =0; i < strlen(line); i++){

        char readChar = line[i];

        if (readChar == ' ' || readChar == '\n' || readChar == '\t') {
            token[tokenIndex] = '\0';
            if (tokenIndex != 0){
                tokens[tokenNo] = (char*)malloc(MAX_TOKEN_SIZE*sizeof(char));
                strcpy(tokens[tokenNo++], token);
                tokenIndex = 0; 
            }
        } 
        else {
            token[tokenIndex++] = readChar;
        }
    }
 
    free(token);
    tokens[tokenNo] = NULL ;
    return tokens;
}

char ***command_tokenize(char* line)
{
    char ***command_tokens = (char ***)malloc(MAX_NUM_TOKENS * sizeof(char **));
    char **single_command = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
    int tokenIndex = 0;

    // Returns first token 
    char *token = strtok(line, ";;");
  
    // Keep printing tokens while one of the
    // delimiters present in str[].
    while (token != NULL)
    {
        single_command = tokenize(token);
        if(single_command[0] != NULL)
        	command_tokens[tokenIndex++] = single_command;
        token = strtok(NULL, ";;");
    }

    command_tokens[tokenIndex] = NULL;

    return command_tokens;
}

void free_memory(char** tokens)
{
    // Freeing the allocated memory 
    for(int i=0;tokens[i]!=NULL;i++){
        free(tokens[i]);
    }
    free(tokens);
}

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
    	kill(getpid(), 9);

}

void main(void)
{
    char  line[MAX_INPUT_SIZE];
    char ***command_tokens;            
    char  **tokens;              
    int i;

    signal(SIGINT, SIG_IGN);
    int parent_id = getpid();

    while (1) {           
       
        printf("turtle>");     
        bzero(line, MAX_INPUT_SIZE);
        gets(line);           
        // printf("Got command %s\n", line);
        line[strlen(line)] = '\n'; //terminate with new line
        command_tokens = command_tokenize(line);

        for(int tokenid = 0; command_tokens[tokenid]!=NULL; tokenid++)
        {
            tokens = command_tokens[tokenid];
	        //do whatever you want with the commands, here we just print them
	        //cd, ls, cat, echo, sleep
	        int count = 0;

	        for(i=0; tokens[i]!=NULL; i++)
	            count++;

	        char* command = tokens[0];
	        if(strcmp(command, "cd") == 0)
	        {
	            if(count!=2)
	            {
	                printf("Wrong number of arguments\n");
	                continue;
	            }
	            
	            int error = chdir(tokens[1]);
	            if(error == -1)
	                printf("Incorrect Command\n");

	            free_memory(tokens);
	            continue;
	        }

	        if(strcmp(command, "exit") == 0)
	        {
	        	free_memory(tokens);
	            exit(0);
	        }

	        int child_id = fork();

	        if(child_id == 0)
	        {
	        	int error;
	        	if (count > 2 && strcmp(tokens[count-2], ">") == 0)
	        	{
	        		close(1);
	        		int fd = open(tokens[count-1], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

	        		tokens[count-2] = tokens[count-1] = NULL;
	        		error = execvp(command, tokens);
	        		close(1);
	        	}
	        	else
	            	error = execvp(command, tokens);
	            if (error == -1)
	            	printf("Incorrect Command\n");
	            break;
	        }
	        else
	        {
	        	free_memory(tokens);
	            wait(NULL);
	        }
	    }

	    int current_pid = getpid();
	    if(current_pid != parent_id)
	    	break;
    }
     

}

                
