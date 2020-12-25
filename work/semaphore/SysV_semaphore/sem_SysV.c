#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
      


//key_t ftok(const char *pathname, int proj_id);
//int semget(key_t key, int nsems, int semflg);

#define	PATHNAME	"/tmp"
#define PROJ_ID		'd'
#define NUM_SEM		2

void print_usage(char *name)
{
	printf("%s (init | show | get SEM_IDX | release SEM_IDX | wait0 SEM_IDX))\n", name);
}

int init_semaphore()
{
	key_t key;
	int semid;	

	key = ftok(PATHNAME, PROJ_ID);	
	if(key == -1){
		perror("ftok()-error\n");
		return 0;
	}

	semid = semget(key, NUM_SEM, IPC_CREAT | 0644);
	if(semid == -1){
		perror("semget()-error\n");
		return 0;
	}

	return semid;
}


void do_init(int semid)
{
	semctl(semid, 0, SETVAL, 1);
	semctl(semid, 1, SETVAL, 3);

}

void do_show(int semid)
{
	for(int i=0; i<NUM_SEM; i++){
		printf("semaphore[%d] = %d\n",i, semctl(semid, i, GETVAL));
	}
}

void do_get(int semid, int sem_idx)
{
	struct sembuf buf;
	memset(&buf, 0, sizeof(buf));
	buf.sem_num = sem_idx;
	buf.sem_op = -1;

	semop(semid, &buf, 1);
}

void do_release(int semid, int sem_idx)
{

	struct sembuf buf;
	memset(&buf, 0, sizeof(buf));
	buf.sem_num = sem_idx;
	buf.sem_op = 1;

	semop(semid, &buf, 1);
}

void do_wait0(int semid, int sem_idx)
{


	struct sembuf buf;
	memset(&buf, 0, sizeof(buf));
	buf.sem_num = sem_idx;
	buf.sem_op = 0;

	semop(semid, &buf, 1);
}


int main(int argc, char **argv)
{
	int semid;
	int sem_idx;

	if(argc < 2){
		print_usage(argv[0]);
		return 0;
	}


	semid = init_semaphore();
	if(semid == -1){
		perror("init_semaphore()-error\n");
		return 0;
	}


	if(!strcmp(argv[1], "init")){
		do_init(semid);
	}

	else if(!strcmp(argv[1], "show")){
		do_show(semid);
	}

	else if(!strcmp(argv[1], "wait0")){
		
		if(argc < 3){
			print_usage(argv[0]);
			return 0;
		}

		sem_idx = atoi(argv[2]);

		do_wait0(semid, sem_idx);
	}

	else if(!strcmp(argv[1], "get")){
	
		if(argc < 3){
			print_usage(argv[0]);
			return 0;
		}

		sem_idx = atoi(argv[2]);
		
		do_get(semid, sem_idx);
	}

	else if(!strcmp(argv[1], "release")){

		if(argc < 3){
			print_usage(argv[0]);
			return 0;
		}

		sem_idx = atoi(argv[2]);

		do_release(semid, sem_idx);
	}
	
	else{
		print_usage(argv[0]);
	}

	return 0;
}
