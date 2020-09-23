#include "kernel/types.h"
#include "user/user.h"

void sieve(int pd[2]) {
    int p;
    //先判断有没有数据，有再创建子进程进行进一步筛选
    if (read(pd[0], &p, sizeof(p)) != 0) {
        printf("prime %d\n", p);
    }
    else {
        close(pd[0]);
        exit(0);
    }

    int pd2[2];
    if (pipe(pd2) != 0) {
        printf("pipe created failed\n");
        printf("fd: %d\n", dup(0));
    }

    if (fork() == 0) {
        close(pd[0]);
        close(pd2[1]);
        sieve(pd2);
    }
    else {
        close(pd2[0]);
        //read(pd[0], &p, sizeof(p));
        //printf("prime %d\n", p);
        int i;
        while (read(pd[0], &i, sizeof(i)) != 0) {
            if (i % p) {
                write(pd2[1], &i, sizeof(i));
            }
        }
        close(pd[0]);
        close(pd2[1]);

        wait(0);
    }
}

int main()
{
    int pd[2];
    if (pipe(pd) != 0) {
        printf("pipe created failed\n");
        printf("fd: %d\n", dup(0));
    }

    if (fork() == 0) {
        close(pd[1]);
        sieve(pd);
    }
    else {
        close(pd[0]);
        for (int i = 2; i <= 35; i++) {
            write(pd[1], &i, sizeof(i));
        }
        //虽然exit时会自动关闭，但不关闭子进程就会阻塞在read不会退出
        //所以在这里需要主动关闭
        //ps:pipe关闭写端，读端不影响，读完全部数据再读时返回0，若不关闭则阻塞
        close(pd[1]);
        wait(0);
    }

    exit(0);
}
