#include    <errno.h>
#include    <sys/types.h>
#include    <sys/wait.h>
#include    <stdio.h>
#include    <stdlib.h>
#include    <string.h>
#include    <unistd.h>

#define oops(m) {perror(m); exit(EXIT_FAILURE);}
#define maxChild 2
#define maxLevels 3


void goFork()
{


}



int main()
{
	pid_t pid;
	
	int levels = 1;
	int childCount = 0;

    	// fork a child process
	pid = fork();

    	if (pid < 0) // error occurred
    	{
    		oops("Fork Failed!");
    	}
    	else if(pid == 0) // child
    	{
        	printf("I am the child %d\n", getpid());
    		// fork a child process
    		pid = fork();
	}
    	else
    	{
    		// pid > 0 ==> must be parent
    		printf("I am the parent %d\n", getpid());
    		/* parent will wait for the child to complete */		
		
		if (wait(NULL) < 0)
			printf("-1 from wait(NULL) with errno = %d\n", errno);

    		printf("Child Complete\n");
    		exit(EXIT_SUCCESS);
    	}
}
