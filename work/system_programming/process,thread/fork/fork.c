#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>


void print_test(void)
{
	printf("test_fucntion from pid: %d\n", getpid());
}


int main(int argc, char **argv)
{
	pid_t pid;


	printf("origin pid: %d\n", getpid());

	pid = fork();

	if(pid > 0){
		printf("parent pid: %d, child pid: %d\n", getpid(), pid);
	}

	else if(pid == 0){
		printf("child pid: %d\n", getpid());
	}

	else{
		perror("fork()-error\n");
		return 0;
	}


	print_test();

	return 0;
}
