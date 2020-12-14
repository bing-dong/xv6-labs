#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char**argv) {
    int ppipe[2];
    int cpipe[2];
    char buf[1] = {'c'};

    if (pipe(ppipe) != 0 || pipe(cpipe) != 0) {
        fprintf(2, "create pipe failed\n");
        exit(-1);
    }
    
    if (fork() == 0) {
        close(ppipe[1]);
        close(cpipe[0]);
        if (read(ppipe[0], buf, 1) == 1) {
            fprintf(1, "%d: received ping\n", getpid());
            write(cpipe[1], "c", 1);
        }
    }
    else {
        close(ppipe[0]);
        close(cpipe[1]);

        write(ppipe[1], "c", 1);
        if (read(cpipe[0], buf, 1) == 1) {
            fprintf(1, "%d: received pong\n", getpid());
        close(cpipe[0]);
        }
    }
    exit(0);
}

