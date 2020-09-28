#include "kernel/types.h"
#include "user/user.h"
#include "kernel/stat.h"
#include "kernel/fs.h"

void find(char *dir, char *filename) 
{
    int fd;
    char dirbuf[512];
    struct stat st;
    struct dirent diren;

    if ((fd = open(dir, 0)) < 0) {
        printf("open dir failed\n");
        exit(-1);
    }

    if (fstat(fd, &st) < 0) {
        printf("stat failed\n");
        exit(-1);
    }

    if (st.type != T_DIR) {
        close(fd);
        exit(1);
    }

    strcpy(dirbuf, dir);
    int buf_idx = strlen(dir);
    dirbuf[buf_idx++] = '/';

    //read目录，每次返回一个目录项
    while (read(fd, &diren, sizeof(diren)) == sizeof(diren)) {
        //diren.inum不可忽视
        if (diren.inum == 0 || strcmp(".", diren.name) == 0 || strcmp("..", diren.name) == 0) {
            continue;
        }
        memcpy(dirbuf+buf_idx, diren.name, DIRSIZ);
        buf_idx += DIRSIZ;
        dirbuf[buf_idx] = '\0';

        if (stat(dirbuf, &st) < 0) {
            printf("stat error\n");
            exit(-1);
        }

        if (st.type == T_FILE && strcmp(filename, diren.name) == 0) {
            printf("%s\n", dirbuf);
        }
        else if (st.type == T_DIR) {
            find(dirbuf, filename);
        }
        buf_idx -= DIRSIZ;
    }
    close(fd);
}

int main(int argc, char *argv[])
{
    if (argc != 3) {
        printf("usage: find dir filename\n");
        exit(-1);
    }

    find(argv[1], argv[2]);

    exit(0);
}
