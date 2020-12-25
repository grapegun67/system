#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>	
#include <sys/mman.h>
#include <semaphore.h>


#define SHM_NAME	"/tmp"


struct login_info{

	sem_t sem;
	int pid;
	int count;	
};



void print_usage(char *name)
{
	printf("%s (monitor | get | release)\n", name);
}

int do_monitor()
{
	int fd,n=0,value;
	struct login_info *info;
	struct login_info local;

	fd = shm_open(SHM_NAME, O_RDWR|O_CREAT, 0644);
	if(fd == -1){
		perror("shm_open()-error\n");
		return 0;
	}

	if(ftruncate(fd, sizeof(struct login_info)) == -1){
		perror("ftruncate()-error\n");
		close(fd);
		return 0;
	}

	info = mmap(NULL, sizeof(struct login_info), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

	if(info == MAP_FAILED){
		perror("mmap()-error\n");
		close(fd);
		return 0;
	}

	memset(info, 0, sizeof(struct login_info));
	memset(&local, 0, sizeof(local));


	if(sem_init(&info->sem, 1, 3) == -1){
		perror("sem_init()-error\n");
		close(fd);
		return 0;
	}
	

	while(1){
		
		if(memcmp(&local, info, sizeof(struct login_info ))){
			
			sem_getvalue(&info->sem, &value);
			memcpy(&local, info, sizeof(struct login_info));
			printf("pid = %d, count = %d, semaphore = %d \n", info->pid, info->count, value);
			n++;
		}	
	
		if(n == 15) break;

		sleep(1);
	}
	
	sem_destroy(&info->sem);
	munmap(info, sizeof(struct login_info));
	close(fd);
	return 0;
}

int do_login()
{
	
	int fd;
	struct login_info *info;

	printf("%d\n", getpid());		

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

	
	sem_wait(&info->sem);	

	info->pid = getpid();
	info->count++;


	munmap(info, sizeof(struct login_info));

	close(fd);

	return 0;
}


int do_logout()
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

	
	sem_post(&info->sem);	

	munmap(info, sizeof(struct login_info));

	close(fd);
	return 0;
}


int main(int argc, char **argv)
{
	int monitor=0;
	int get;	

	if(argc < 2){
		print_usage(argv[0]);
		return 0;
	}

	if(argc == 2){

		if(!strcmp(argv[1], "monitor")){
			monitor = 1;
		}
		
		else if(!strcmp(argv[1], "get")){
			get=1;
		}

		else if(!strcmp(argv[1], "release")){
			get=0;
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
		if(get){

			do_login();
		}

		else{
			do_logout();
		}
	
	}

	return 0;
}
