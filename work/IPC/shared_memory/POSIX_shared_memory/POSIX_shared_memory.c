#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>	
#include <sys/mman.h>


#define SHM_NAME	"/shm"


struct login_info{

	int pid;
	int count;
};



void print_usage(char *name)
{
	printf("%s (monitor)\n", name);
}

int do_monitor()
{
	int fd,n=0;
	struct login_info *info;
	struct login_info local;

	fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0644);
	if(fd == -1){
		perror("shm_open()-error\n");
		return 0;
	}

	if(ftruncate(fd, sizeof(struct login_info)) == -1){
		perror("ftruncate()-error\n");
		close(fd);
		return 0;
	}
//       void *mmap(void *addr, size_t length, int prot, int flags,  int fd, off_t offset);
	


	info = mmap(NULL, sizeof(struct login_info), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

	if(info == MAP_FAILED){
		perror("mmap()-error\n");
		close(fd);
		return 0;
	}

	memset(info, 0, sizeof(struct login_info));
	memset(&local, 0, sizeof(local));

	while(1){
		
		if(memcmp(&local, info, sizeof(struct login_info ))){
			memcpy(&local, info, sizeof(struct login_info));
			printf("pid = %d, count = %d\n", info->pid, info->count);
			n++;
		}	
	
		if(n == 5) break;

		sleep(1);
	}
	

	munmap(info, sizeof(struct login_info));
	close(fd);
	return 0;
}

int do_login()
{
	
	int fd;
	struct login_info *info;

		
	fd = shm_open(SHM_NAME, O_RDWR, 0644);
	if(fd == -1){
		perror("open()-error\n");
		return 0;
	}


	info = mmap(NULL, sizeof(struct login_info), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if(info == MAP_FAILED){
		perror("mmap()-error\n");
		close(fd);
		return 0;
	}

	
	info->pid = getpid();
	info->count++;
	

	munmap(info, sizeof(struct login_info));

	close(fd);

	return 0;
}



int main(int argc, char **argv)
{
	int monitor=0;


	if(argc == 2){

		if(!strcmp(argv[1], "monitor")){
			monitor = 1;
		}
		
		else {
			print_usage(argv[0]);
			return 0;
		}
	}

	if(monitor){
		do_monitor();
	}
	
	else{
		do_login();
	}

	return 0;
}
