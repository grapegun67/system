#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


#define TARGET_FILE "test"


int main(int argc, char **argv)
{
	if(link(TARGET_FILE, "hardlinked_test")){
		perror("link() error\n");
		return 0;
	}


	if(symlink(TARGET_FILE, "symlinked_test")){
		perror("link() error\n");
		return 0;
	}

	return 0;
}
