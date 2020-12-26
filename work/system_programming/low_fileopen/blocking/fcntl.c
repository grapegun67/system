#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>



int main(int argc, char **argv)
{
	int ret,flag;
	char buf[1024];
	
	memset(buf, 0, sizeof(buf));



	printf("trying...\n");
	ret = read(STDIN_FILENO, buf, sizeof(buf));
	
	if(ret == -1){
		perror("read()-error\n");
		return 0;
	}

	printf("byte: %d, read: %s\n", ret, buf);


	flag = fcntl(STDIN_FILENO, F_GETFL);
	if(flag == -1){
		perror("fcntl()-error\n");
		return 0;
	}


	ret = fcntl(STDIN_FILENO, F_SETFL, flag | O_NONBLOCK);
	if(ret == -1){
		perror("fcntl()-error\n");
		return 0;
	}

	
	printf("trying...\n");
	ret = read(STDIN_FILENO, buf, sizeof(buf));
	if(ret == -1){
		printf("errno:%d, %s\n", errno, strerror(errno));
	}

	printf("byte: %d, read: %s\n", ret, buf);


	return 0;
}
