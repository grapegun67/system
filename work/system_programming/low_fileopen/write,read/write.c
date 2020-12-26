#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#define PATHNAME	"exam_test"


void print_usage(char *name)
{
	printf("%s (write | read)\n", name);
}


void do_write()
{
	int fd;
	char buf[1024];
	memset(buf, 0, sizeof(buf));	


	fd = open(PATHNAME, O_CREAT | O_RDWR, 0644);
	if(fd == -1){
		perror("open()-error\n");
		return;
	}


	strncpy(buf, "Linux system\n", sizeof(buf));
	if(write(fd, buf, sizeof(buf)) == -1){
		perror("write()-error\n");
		return;
	}

	close(fd);
}


void do_read()
{
	int fd;
	char buf[1024];
	memset(buf, 0, sizeof(buf));


	fd = open(PATHNAME, O_RDONLY);
	if(fd == -1){
		perror("open()-error\n");
		return;
	}
	

	read(fd, buf, sizeof(buf));
	
	printf("result: %s", buf);

	close(fd);
}


int main(int argc, char **argv)
{

	if(argc < 2){
		print_usage(argv[0]);
	}

	else if(!strcmp(argv[1], "write")){
		do_write();
	}

	else if(!strcmp(argv[1], "read")){
		do_read();
	}

	else{
		print_usage(argv[0]);
	}


	return 0;
}
