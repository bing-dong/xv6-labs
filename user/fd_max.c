#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    while (1) {
        int fd = dup(1);
        printf("%d\n", fd);
        if (fd < 0) break;
    }
    exit(0);
}
