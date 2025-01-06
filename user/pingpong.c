// Ping

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

int main(int argc, char *argv[])
{
    char dummy = 'h';
    char buffer[1];
    int pipefd[2];
    if (pipe(pipefd) == -1) 
    {
        printf("Error creating pipe\n");
        exit(1);
    }
    int pid = fork();
    if (pid == 0) 
    {
        
        // child
        read(pipefd[0], buffer, sizeof(buffer));
        close(pipefd[0]);
        printf("%d: received ping\n", getpid());
        write(pipefd[1], &dummy, sizeof(dummy));
        close(pipefd[1]);
        exit(0);

    } else 
    {
        // parent
        // Write to the pipe
        write(pipefd[1], &dummy, sizeof(dummy));
        close(pipefd[1]);
        read(pipefd[0], buffer, sizeof(buffer));
        printf("%d: received pong\n", getpid()); 
        close(pipefd[0]);
        exit(0);
    }

}