#include    <errno.h>
#include    <sys/types.h>
#include    <sys/wait.h>
#include    <stdio.h>
#include    <stdlib.h>
#include    <string.h>
#include    <unistd.h>

#define oops(m) {perror(m); exit(EXIT_FAILURE);}
#define maxFork 2
#define maxHier 6

pid_t pid;
char name[maxHier] = "1";

void goFork(int forkVal)
{
	forkVal++;
	if(forkVal < maxFork)
	{
		pid = fork();
		if(pid < 0) //error
		{
			oops("Fork Failed!")
		}
		else if(pid == 0)
		{
			setName(".1", forkVal);
		}
		else
		{
			setName(".2", forkVal);
		}
	}

}

void setName(char endNum[maxFork], int forkVal)
{
	strcat(name, endNum);
	pid = fork();
	if(pid < 0)
	{
		oops("Fork Failed!");
	}
	else if(pid == 0)
	{
		goFork(forkVal);
	}
	else
	{
		if(execlp("./iam", "iam", name, NULL) < 0 )
			oops("Execlp Failed!");
	}

}

int main()
{
    int currFork = 0; 

    // fork a child process
    pid = fork();

    if (pid < 0) // error occurred
    {
	    oops("Fork Failed!");
    }
    else if(pid == 0) // child
    {
	    goFork(currFork);
    }
    else
    {
    	    /* parent will wait for the child to complete */
    	    if (wait(NULL) < 0)
		    printf("-1 from wait(NULL) with errno = %d\n", errno);

    	    printf("Child Complete\n");
    	    exit(EXIT_SUCCESS);
    }
}
