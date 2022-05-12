#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void repeat(int p[2], int SIZE) {
    int first[1];
    int buf[--SIZE];
    //关闭不必要的文件描述符
    close(0);
    dup(p[0]);
    close(p[0]);
    close(p[1]);

    if (read(0, first, 4)) {
        printf("prime %d\n", first[0]);
    }//print
    
    if(SIZE){
        int size = read(0, buf, sizeof buf) / 4;//read
        if(size){
            int p1[2];
            pipe(p1);

            for (int i = 0; i < size; ++i) {
                if (buf[i] % first[0] != 0) {
                    write(p1[1], buf + i, 4);
                } else {
                    --SIZE;
                }
            }//write

            if(fork() == 0){
                repeat(p1, SIZE);
                exit(0);
            } else {
                wait(0);
            }
        }
    }

    exit(0);
}

int main() {
    const int SIZE = 34;
    int p[2];
    pipe(p);
    int buf[SIZE];
    for(int i = 0; i < SIZE; ++i){
        buf[i] = i + 2;
    }

    write(p[1], buf, SIZE * 4);
    if(fork() == 0) {
        repeat(p, SIZE);
        exit(0);
    } else {
        wait(0);
    }
    exit(0);
}

