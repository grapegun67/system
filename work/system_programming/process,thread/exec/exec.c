#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


void print_test(void)
{
	printf("test_fucntion from pid: %d\n", getpid());
}



int main(int argc, char **argv)
{
	pid_t pid;
	int exit_status;

	printf("origin pid: %d\n", getpid());

	pid = fork();

	if(pid > 0){
		printf("parent pid: %d, child pid: %d\n", getpid(), pid);
	}

	else if(pid == 0){
		printf("child pid: %d\n", getpid());
		if(execl("/bin/ls", "ls", "-al", NULL) == -1){
			perror("execl()-error\n");
			return 0;
		}
	}

	else{
		perror("fork()-error\n");
		return 0;
	}


	print_test();


	pid = wait(&exit_status);
	if(WIFEXITED(exit_status)){
		printf("child %d returns %d\n", pid, WEXITSTATUS(exit_status));
	}
	
	else{
		printf("child %d is not exited\n", pid);
	}

	return 0;
}
