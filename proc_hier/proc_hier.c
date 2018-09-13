#include    <errno.h>
#include    <sys/types.h>
#include    <sys/wait.h>
#include    <stdio.h>
#include    <stdlib.h>
#include    <string.h>
#include    <unistd.h>

#define oops(m) {perror(m); exit(EXIT_FAILURE);}
#define maxHier 3

pid_t pid;
char name[maxHier] = "1";

void goFork(int level)
{
	level++;
	if(level < maxHier)
	{
		pid = fork();

		if(pid < 0) //error
		{
			oops("Fork Failed!")
		}
		else if(pid == 0)
		{
			strcat(name, ".1");
			printf("I am the child. Name: %s\n", name);
			exit(EXIT_SUCCESS);
		}
		else
		{
			pid = fork();

			if(pid < 0)
			{
				oops("Fork Failed!");
			}
			else if(pid == 0)
			{
				strcat(name, ".2");
				printf("I am the child. Name: %s\n", name);
				exit(EXIT_SUCCESS);
			}
			else
			{
				printf("I am the parent. Name: %s\n", name);
				if (wait(NULL) < 0)
					printf("-1 from wait(NULL) with errno = %d\n", errno);
 	    			printf("Child Complete\n");
    	    			exit(EXIT_SUCCESS);
			}
		}
	}

}

int main()
{
    int currLevel = 0; 

    // fork a child process
    pid = fork();

    if (pid < 0) // error occurred
    {
	    oops("Fork Failed!");
    }
    else if(pid == 0) // child
    {
	    strcat(name, ".1");
	    printf("I am the child. Name: %s\n", name);
	    goFork(currLevel);
    }
    else
    {
	    pid = fork();

	    if(pid < 0)
	    {
		    oops("Fork Failed!");
	    }
	    else if(pid == 0)
	    {
		    strcat(name, ".2");
		    printf("I am the child. Name: %s\n", name);
		    goFork(currLevel);
	    }
	    else
	    {
		printf("I am the parent. Name: %s\n", name);
		/* parent will wait for the child to complete */
    	    	if (wait(NULL) < 0)
			printf("-1 from wait(NULL) with errno = %d\n", errno);
 	    	printf("Child Complete\n");
    	    	exit(EXIT_SUCCESS);
	    }
    }
}
