#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>

#define TYPE2STR(x) \
     ((x) == DT_BLK	? "block device":\
      (x) == DT_CHR	? "char device":\
      (x) == DT_DIR	? "directory":\
      (x) == DT_FIFO	? "fifo":\
      (x) == DT_LNK	? "symlink":\
      (x) == DT_REG	? "regular":\
      (x) == DT_SOCK	? "socket":\
      "unknown")




int main(int argc, char **argv)
{

	DIR *dp;
	struct dirent *entry;

	dp = opendir(".");
	if(dp == NULL){
		perror("opendir()-error\n");
		return 0;
	}


	while((entry = readdir(dp))){

		printf("name: %s ", entry->d_name);
		printf("type: %s\n", TYPE2STR(entry->d_type));
	}	

	closedir(dp);
	return 0;
}
