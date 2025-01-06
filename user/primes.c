// Sieve

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

void sieve(int readfd) {
    int buffer[1];
    int ret = read(readfd, buffer, sizeof(int));
    // Base case
    if (ret <= 0) {
        return;
    }
    printf("prime %d\n", buffer[0]);
    int pipefd[2]; 
    // Create pipe
    if (pipe(pipefd) == -1) 
    {
        printf("Error creating pipe\n");
        exit(1);
    }
    // Fork
    int pid = fork();
    if (pid == 0) {
        // child
        close(pipefd[1]);
        sieve(pipefd[0]);
        close(pipefd[0]); 
    } else {
        // parent
       close(pipefd[0]);
       int readNum[1];
       while (read(readfd, readNum, sizeof(int)) > 0) {
        if (readNum[0] % buffer[0] != 0) {
            write(pipefd[1], readNum, sizeof(int));
        }
       }
       close(pipefd[1]);
       close(readfd);
       wait(0);
    }
}


int main(int argc, char *argv[])
{
    int END = 36;
    int pipefd[2];
    int buffer[1];

    // Create pipe
    if (pipe(pipefd) == -1) 
    {
        printf("Error creating pipe\n");
        exit(1);
    }

    // Fork first child
    int pid = fork();
    if (pid == 0) {
        // child
        close(pipefd[1]);
        sieve(pipefd[0]);
        close(pipefd[0]);
    }
    else if (pid > 0) {
        // parent
        close(pipefd[0]);
        for (int i = 2; i < END; i++) {
            buffer[0] = i;
            write(pipefd[1], buffer, sizeof(int));
        }
        close(pipefd[1]);
        wait(0);
        
    } else {
        printf("Crap. Something is wrong\n");
        exit(1);
    }
}
