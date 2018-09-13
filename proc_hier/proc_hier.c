#include    <errno.h>
#include    <sys/types.h>
#include    <sys/wait.h>
#include    <stdio.h>
#include    <stdlib.h>
#include    <string.h>
#include    <unistd.h>

#define oops(m) {perror(m); exit(EXIT_FAILURE);}

int main()
{
    pid_t pid;

    // fork a child process
    pid = fork();

    if (pid < 0) // error occurred
    oops("Fork Failed!");

    if (pid == 0) // child
    {
        printf("I am the child %d\n", getpid());
        if (execlp("./iam", "iam", "Hello Parent! Please do not kill me!", NULL) < 0)
        oops("Execlp Failed!");
    }

    // pid > 0 ==> must be parent
    printf("I am the parent %d\n", getpid());
    /* parent will wait for the child to complete */
    if (wait(NULL) < 0)
        printf("-1 from wait(NULL) with errno = %d\n", errno);

    printf("Child Complete\n");
    exit(EXIT_SUCCESS);
}
