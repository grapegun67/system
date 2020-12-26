#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>	
#include <sys/mman.h>
#include <sys/wait.h>


struct login_info{

	int pid;
	int count;
};

struct login_info *login_info_init()
{
	struct login_info *info;

	info = mmap(NULL, sizeof(struct login_info), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

	if(info == MAP_FAILED){
		perror("mmap()-error\n");
		return NULL;
	}

	memset(info, 0, sizeof(struct login_info));

	return info;
}


int do_monitor(struct login_info *info)
{
	int n=0;
	struct login_info local;

	memset(&local, 0, sizeof(local));

	while(1){
		
		if(memcmp(&local, info, sizeof(struct login_info ))){
			memcpy(&local, info, sizeof(struct login_info));
			printf("pid = %d, count = %d\n", info->pid, info->count);
			n++;
		}	
	
		if(n == 5) break;

		usleep(100000);
	}
	

	munmap(info, sizeof(struct login_info));
	return 0;
}

int do_login(int index, struct login_info *info)
{
	sleep(index + 1);

	int fd;
	
	info->pid = getpid();
	info->count++;

	return 0;
}



int main(int argc, char **argv)
{
	int pid,i;
	struct login_info *info;

	info = login_info_init();

	if(!info){
		perror("login_info_init()-error\n");
		return 0;
	}	


#define NUM_FORK	5


	for(i=0; i<NUM_FORK; i++){

		pid = fork();

		if(pid > 0){
		}

		else if(pid == 0){
			do_login(i, info);
			munmap(info, sizeof(struct login_info));
			return 0;
		}
	
		else{
			perror("fork()-error\n");
			return 0;
		}
	
	}

	do_monitor(info);

	for(i=0; i<NUM_FORK; i++){

		pid = wait(NULL);
		printf("pid %d is end\n", pid);
	}	
	

	return 0;
}
