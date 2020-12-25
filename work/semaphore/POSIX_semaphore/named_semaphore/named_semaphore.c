#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <fcntl.h>           
#include <sys/stat.h>    
#include <semaphore.h>
#include <time.h>      


#define SEM_ID	"/test_sem"


void print_usage(char *name)
{
	printf("%s (show | wait | trywait | timedwait | release)\n", name);
}

sem_t *init_semaphore()
{
	sem_t *sem;

	sem = sem_open(SEM_ID, O_CREAT, 0644, 3);
	if(sem == SEM_FAILED){
		perror("sem_open()-error\n");
		return NULL;
	}

	return sem;
}


void do_show(sem_t *sem)
{
	int i;

	if(sem_getvalue(sem, &i) == -1){
		perror("sem_getvalue()-error\n");
		return;
	}

	printf("semaphore = %d\n", i);
}

void do_wait(sem_t *sem)
{
	sem_wait(sem);
}

void do_trywait(sem_t *sem)
{
	sem_trywait(sem);
}


void do_timedwait(sem_t *sem)
{
	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	ts.tv_sec += 3;

	sem_timedwait(sem, &ts);
}

void do_release(sem_t *sem)
{
	sem_post(sem);
}


int main(int argc, char **argv)
{
	sem_t *sem;

	if(argc < 2){
		print_usage(argv[0]);
		return 0;
	}

	sem = init_semaphore();
	if(sem == NULL){
		perror("init_semaphore()-error\n");
		return 0;
	}


	if(!strcmp(argv[1], "show")){

		do_show(sem);
	}

	else if(!strcmp(argv[1], "wait")){

		do_wait(sem);
	}

	else if(!strcmp(argv[1], "trywait")){

		do_trywait(sem);
	}
	
	else if(!strcmp(argv[1], "timedwait")){

		do_timedwait(sem);
	}

	else if(!strcmp(argv[1], "release")){

		do_release(sem);
	}
	
	else{
		print_usage(argv[0]);
	}

	return 0;
}
