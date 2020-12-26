#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>



#define PATHNAME	"data"

int write_file()
{
	int fd;
	char buf[1024];
	memset(buf, 0, sizeof(buf));


	fd = open(PATHNAME, O_CREAT | O_WRONLY ,0644);
	if(fd == -1){
		perror("open()-error\n");	
		return 0;
	}	

	
	strncpy(buf,  "system programming\n", sizeof(buf));

	write(fd, buf, sizeof(buf));


	close(fd);
	return 0;
}

int read_file()
{
	int fd;
	char buf[1024];
	memset(buf, 0, sizeof(buf));
	
	fd = open(PATHNAME, O_RDONLY);
	if(fd == -1){
		perror("open()-error\n");
		return 0;
	}
	
	read(fd, buf, sizeof(buf));


	return 0;
}





int main(int argc, char **argv)
{
	int fd;

	write_file();
	read_file();


	return 0;
}
