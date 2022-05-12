#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]){
	int p1[2], p2[2];
	pipe(p1), pipe(p2);
	if(fork() == 0){
		close(0);
		dup(p1[0]);
		close(p1[0]);
		close(p1[1]);
		
		char buf[5];
		int size = read(0, buf, sizeof buf);	
		printf("%d: received ", getpid());
		write(1, buf, size);

		close(1);
		dup(p2[1]);
		close(p2[1]);
		close(p2[0]);

		write(1, "pong\n", 5);

		exit(0);
	}else{	
		close(p1[0]);
		write(p1[1], "ping\n", 5);
		close(p1[1]);

		wait(0);

		close(0);
		dup(p2[0]);
		close(p2[0]);
		close(p2[1]);
		
		char buf[5];
		int size = read(0, buf, sizeof buf);
		printf("%d: received ", getpid());
		write(1, buf, size);
	}
	exit(0);
}

