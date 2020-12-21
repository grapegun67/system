#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define FIFO_FILENAME "./testfifo"

int do_reader(void)
{
	
	int fd;
	char buf[128];

	printf("call open().......\n");
	fd = open(FIFO_FILENAME, O_RDONLY);
	if( fd <0 ){
		perror("open()-error");
		return -1;
	}
	read(fd, buf, sizeof(buf));
	printf("answer: %s\n", buf);
	close(fd);	

	return 0;
}

int do_writer(void)
{

	int fd;
	char buf[128];

	unlink(FIFO_FILENAME);
	if(mkfifo(FIFO_FILENAME, 0644)){
		perror("mkfifo()-error");
		return -1;
	}

	printf("call open()......\n");
	fd = open(FIFO_FILENAME, O_WRONLY);
	if( fd <0 ){
		perror("open()-error");
		return -1;
	}
	
	strncpy(buf, "Linux", sizeof(buf));
	write(fd ,buf, strlen(buf));
	close(fd);

	return 0;
}

void print_usage(char *name){

	printf("%s (w|r) \n", name);
       return;	
}


int main(int argc, char **argv)
{
	if(argc < 2){
		print_usage(argv[0]);
		return -1;
	}

	
	if(!strcmp(argv[1], "r")){
		//reader
		do_reader();

	}
	else if(!strcmp(argv[1], "w")){
		//writer
		do_writer();

	}
	else{
		print_usage(argv[0]);
		return -1;
	}


	return -1;
}
