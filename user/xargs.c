#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"

int main(int argc, char *argv[])
{
    char argbuf[MAXARG];
    char *args[MAXARG];
    int bufcnt = 0;

    while (read(0, &argbuf[bufcnt], 1) == 1) {
        if (argbuf[bufcnt] != '\n') {
            bufcnt++;
            continue;
        }
        argbuf[bufcnt] = '\0';

        int argcnt = 0;
        args[argcnt++] = argv[1];
        while (argcnt < argc-1) {
            args[argcnt] = argv[argcnt+1];
            argcnt++;
        }
        args[argcnt++] = argbuf;
        args[argcnt] = '\0';

        if (fork() == 0) {
            exec(argv[1], args);
            exit(0);
        }
        else {
            wait(0);
            bufcnt = 0;
        }
    }

    exit(0);
}
