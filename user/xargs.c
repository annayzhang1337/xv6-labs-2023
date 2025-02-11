#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

int main(int argc, char *argv[])
{
    // Parse additional arguments
    char *additional_args[MAXARG];

    for (int i = 1; i < argc; i++) {
        additional_args[i-1] = argv[i];
    }

    char buf[200]; // arbitrary; defined as 200 to be safe to store each arg in a buffer
    int i = 0;
    int base_args = argc - 1;
    char c;
    int num_args = base_args;

    while(read(0, &c, 1) > 0) {
        if (c == '\n') {
            buf[i] = '\0';
            additional_args[num_args] = malloc(strlen(buf)+1);
            strcpy(additional_args[num_args], buf);
            num_args++;
            additional_args[num_args] = 0;

            // fork
            int pid = fork();
            if (pid < 0){
                printf("xargs: fork failed\n");
                exit(1);
            } else if (pid == 0) {
                // child
                if (exec(additional_args[0], additional_args) < 0) {
                    printf("xargs: exec failed\n");
                    exit(1);
                } 
            } else {
                wait(0);
                // reset
                for (int j = base_args; j < num_args; j++) {
                    free(additional_args[j]);
                    additional_args[j] = 0;
                }
                num_args = base_args;
                i = 0;
            }
    
        } else if (c == ' ') {
            buf[i] = '\0';
            additional_args[num_args] = malloc(strlen(buf)+1);
            strcpy(additional_args[num_args], buf);
            num_args++;
            i = 0;
        } else {
            buf[i] = c;
            i++;
        }
    }
    exit(0);
}