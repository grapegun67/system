#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>




#define PATHNAME	"test"
#define MMAPSIZE	1024


void print_usage(char *name)
{
	printf("%s (write sentence| read)\n", name);
}

void do_write(char *input)
{
	int fd;
	char *add;
	char buf[MMAPSIZE];
	memset(buf, 0, sizeof(buf));

	fd = open(PATHNAME, O_CREAT | O_RDWR, 0644);

	add = mmap(NULL, MMAPSIZE, PROT_WRITE, MAP_SHARED, fd, 0);
	if(add == (void *)-1){
		perror("mmap()-error\n");
		return;
	}
	
	strncpy(buf, input, sizeof(buf));
	write(fd, buf, sizeof(buf));

	munmap(add, sizeof(buf));
}


void do_read()
{

	int fd;
	char *add;
	char buf[MMAPSIZE];
	memset(buf, 0, sizeof(buf));

	fd = open(PATHNAME, O_CREAT | O_RDWR, 0644);

	add = mmap(NULL, MMAPSIZE, PROT_WRITE, MAP_SHARED, fd, 0);
	if(add == (void *)-1){
		perror("mmap()-error\n");
		return;
	}

	read(fd ,buf, sizeof(buf));

	printf("result: %s\n", buf);
}



int main(int argc, char **argv)
{

	if(!strcmp(argv[1], "read")){
		do_read();
		return 0;
	}

	if(argc < 3){
		print_usage(argv[0]);
		return 0;
	}

	if(!strcmp(argv[1], "write")){
		do_write(argv[2]);
	}

	else{
		print_usage(argv[0]);
	}



	return 0;
}

