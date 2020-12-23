#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>	
#include <sys/mman.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/procfs.h>


#define SHMKEY_FILEPATH		"/tmp"
#define SHMKEY_PROJID		'r'

#define ROUNDUP(x) 	((x) + (PAGE_SIZE -1)) & -(PAGE_SIZE -1)


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
	int n=0,shmid;
	key_t key;
	size_t size;
	struct login_info *info;
	struct login_info local;


	key = ftok(SHMKEY_FILEPATH, SHMKEY_PROJID);
	if(key == -1){
		perror("ftok()-error\n");
		return 0;
	}


	size = ROUNDUP(sizeof(struct login_info));
	shmid = shmget(key, size, IPC_CREAT | 0644);
	if(shmid == -1){
		perror("shmget()-error\n");
		return 0;
	}


	info = shmat(shmid, NULL, 0);
	if(info == (void *)-1){
		perror("shmat()-error\n");
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

	shmdt(info);	
	return 0;
}

int do_login()
{
	key_t key;
	int shmid;
	size_t size;
	struct login_info *info;

	key = ftok(SHMKEY_FILEPATH, SHMKEY_PROJID);
	if(key == -1){
		perror("ftok()-error\n");
		return 0;
	}


	size = ROUNDUP(sizeof(struct login_info));
	shmid = shmget(key, size,  0644);
	if(shmid == -1){
		perror("shmget()-error\n");
		return 0;
	}


	info = shmat(shmid, NULL, 0);
	if(info == (void *)-1){
		perror("shmat()-error\n");
		return 0;
	}	

	info->pid = getpid();
	info->count++;


	shmdt(info);
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
