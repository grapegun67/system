#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#define PATHNAME "lock_test"


void print_usage(char *name)
{
	printf("%s (exclusive|shared)\n", name);
}



int main(int argc, char **argv)
{
	int fd;
	int ops;

	if(argc < 2){
		print_usage(argv[0]);
		return 0;
	}

	else if(!strcmp(argv[1], "exclusive")){
		ops = LOCK_EX;
	}

	else if(!strcmp(argv[1], "shared")){
		ops = LOCK_SH;
	}

	else{
		print_usage(argv[0]);
		return 0;
	}
	



	fd = open(PATHNAME, O_CREAT | O_RDWR, 0644);
	if(fd == -1){
		perror("open()-error\n");
		return 0;
	}



	printf("before-lock\n");

	if(flock(fd, ops) == -1){
		perror("flock()-error\n");
		close(fd);
		return 0;
	}

	printf("get-lock\n");

	getc(stdin);	

	if(flock(fd, LOCK_UN) == -1){
		perror("flock()-error\n");
		close(fd);
		return 0;
	}

	printf("after-lock\n");


	close(fd);
	return 0;
}
