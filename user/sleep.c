// Sleep

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

int main(int argc, char *argv[])
{
    // check for errors
    if (argc != 2) {
        const char *message = "Bad input. User should input the number of sleep seconds.\n";
        write(2, message, strlen(message));
        exit(1);
    }
    sleep(atoi(argv[1]));
    exit(0);
}