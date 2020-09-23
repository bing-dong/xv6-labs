#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char**argv) {
    int wpipe[2];
    int rpipe[2];
    char buf[1] = {'c'};

    if (pipe(wpipe) != 0 || pipe(rpipe) != 0) {
        fprintf(2, "create pipe failed\n");
        exit(-1);
    }
    
    if (fork() == 0) {
        close(wpipe[1]);
        close(rpipe[0]);
        if (read(wpipe[0], buf, 1) == 1) {
            fprintf(1, "%d: received ping\n", getpid());
            write(rpipe[1], buf, 1);
        }
    }
    else {
        close(wpipe[0]);
        close(rpipe[1]);

        write(wpipe[1], buf, 1);
        if (read(rpipe[0], buf, 1) == 1) {
            fprintf(1, "%d: received pong\n", getpid());
        close(rpipe[0]);
        }
    }
    exit(0);
}

