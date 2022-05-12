#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void
find(char *path, char *fileName) {
//	printf("path: %s, fileName: %s\n", path, fileName);
    char buf[128], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if ((fd = open(path, 0)) < 0) {
        fprintf(2, "path error\n");
        return;
    }

    if (fstat(fd, &st) < 0) {
        fprintf(2, "path stat failed\n");
        close(fd);
        return;
    }

    switch (st.type) {
        case T_FILE:
            fprintf(2, "path error\n");
            return;                     //以上部分判断输入路径是否正确
        case T_DIR:
            strcpy(buf, path);
            p = buf + strlen(buf);
            *p++ = '/';                 //保存当前正在搜索目录的完整路径，便于输出
            while (read(fd, &de, sizeof(de)) == sizeof(de)) {   //遍历搜索目录
                if (de.inum == 0)
                    continue;
                char bufFile[128], *pFile;
                strcpy(bufFile, path);
                pFile = bufFile + strlen(bufFile);
                *pFile++ = '/';
                memmove(pFile, de.name, DIRSIZ);    //保存带完整路径的文件名，便于正常打开
//                printf("%s\n", bufFile);
		int fd1 = -2;
                struct stat st1;
                if(strcmp(de.name, ".") && strcmp(de.name, "..")){  //若不是'.'或'..'目录，则打开
                    fd1 = open(bufFile, 0);
                    fstat(fd1, &st1);
//		    printf("fd1: %d\n", fd1);
                }
                if (fd1 >= 0) {
//			printf("open scuccess!\n");
                    if (fstat(fd1, &st1) >= 0) {
                        char bufPath[128], *pPath;
                        switch (st1.type) {
                            case T_FILE:
                                if (!strcmp(de.name, fileName)) {
                                    memmove(p, de.name, DIRSIZ);
                                    p[DIRSIZ] = 0;
                                    printf("%s\n", buf);    //若文件名与目标文件名一致，则输出其完整路径
                                }
                                close(fd1);
                                break;
                            case T_DIR:
//				printf("dir\n");
                                strcpy(bufPath, path);
                                pPath = bufPath + strlen(bufPath);
                                *pPath++ = '/';
                                memmove(pPath, de.name, DIRSIZ);    //保存下一个目录的完整路径名，传递给下一个递归程序
//                                printf("%s\n", bufPath);
                                close(fd1);
//				printf("before\n");
                                find(bufPath, fileName);
                                break;
                            case T_DEVICE:
                                close(fd1);
                                break;
                        }
                    }
                }
            }
            break;
    }
    close(fd);
}

int
main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(2, "Usage:find path fileName\n");
        exit(0);
    }
    find(argv[1], argv[2]);
    exit(0);
}
