#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>




void print_usage(char *name)
{
	printf("%s (filename)\n", name);
}


int main(int argc, char **argv)
{

	struct stat statbuf;

	if(argc < 2){
		print_usage(argv[0]);
		return 0;
	}

	if(stat(argv[1], &statbuf)){
		perror("stat()-error\n");
		return 0;
	}	

	printf("filename: %s\n", argv[1]);
	printf("size: %d\n", statbuf.st_size);


	if(S_ISDIR(statbuf.st_mode)){
		printf("mode: directory");

	}
	
	else if(S_ISREG(statbuf.st_mode)){
		printf("mode: regular");
	}

	else printf("???\n");

	return 0;
}
