#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char* argv[]) {
    if (argc <= 1) {
        fprintf(2, "usage: sleep [sec]\n");
        exit(-1);
    }

    int sec = atoi(argv[1]);
    if (sleep(sec) != 0) {
        fprintf(2, "sleep error\n");
        exit(-1);
    }

    exit(0);
}
