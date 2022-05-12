#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

int main(int argc, char *argv[]) {
    //从标准输入读取数据
    char stdIn[512];
    int size = read(0, stdIn, sizeof stdIn);
    //将数据分行存储
    int line = 0;
    for (int k = 0; k < size; ++k) {
        if (stdIn[k] == '\n') {
            ++line;
        }
    }
    char output[line][64];
    int i = 0, j = 0;
    for (int k = 0; k < size; ++k) {
        output[i][j++] = stdIn[k];
        if (stdIn[k] == '\n') {
            output[i][j - 1] = 0;
            ++i;
            j = 0;
        }
    }
    //将数据分行拼接到argv[2]后，并运行
    char *arguments[MAXARG];
    for(j = 0; j < argc - 1; ++j){
        arguments[j] = argv[1 + j];
    }
    line = 0;
    while (line < i) {
        arguments[j] = output[line++];
        if (fork() == 0) {
            exec(argv[1], arguments);
            exit(0);
        }
        wait(0);
    }
    exit(0);
}

