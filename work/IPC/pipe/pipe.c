#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char ** argv)
{


	int pipe_fds[2];
	pid_t pid;
	char buf[1024];

	memset(buf, 0, sizeof(buf));

	if(pipe(pipe_fds)){
		perror("pipe()-error");
		return -1;
	}

	pid=fork();

	if(pid == 0){
		close(pipe_fds[1]);
		read(pipe_fds[0], buf, sizeof(buf));
		printf("%s\n", buf);
		close(pipe_fds[0]);
		
	}
	else if(pid > 0){
		close(pipe_fds[0]);
		strncpy(buf, "Linux system programming", sizeof(buf)-1);
		write(pipe_fds[1], buf, strlen(buf));
		close(pipe_fds[1]);

	}

	else {
		perror("fork()-error");
		goto err;

	}

err:
	close(pipe_fds[0]);
	close(pipe_fds[1]);
	return -1;
}

