
#define _GNU_SOURCE        

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sched.h>
#include <sys/types.h>
#include <unistd.h>


#define MAXCPU	2


void print_cpuset(cpu_set_t *mask)
{	
	for(int i=0; i<MAXCPU; i++){

		if(CPU_ISSET(i, mask)){
			printf("cpu%d ", i);		
		}
	}
	
	printf("\n");
}


int main(int argc, char **argv)
{
	int cur_cpu;
	cpu_set_t mask;	


	if(sched_getaffinity(getpid(), sizeof(cpu_set_t), &mask) == -1){
		perror("sched_getaffinity()-error\n");
		return 0;
	}
	print_cpuset(&mask);
	
	cur_cpu = sched_getcpu();
	printf("run on CPU%d\n", cur_cpu);


	CPU_ZERO(&mask);
	CPU_SET(cur_cpu ^ 1, &mask);


	if(sched_setaffinity(getpid(), sizeof(cpu_set_t), &mask) == -1){
		perror("sched_setaffinity()-error\n");
		return 0;
	}


	cur_cpu = sched_getcpu();
	printf("run on CPU%d\n", cur_cpu);

	return 0;
}






