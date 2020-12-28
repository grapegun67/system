#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>



void sigterm_handler(int signum)
{
	printf("fixed handler\n");

}


int main(int argc, char **argv)
{

	sigset_t set;

	
	sigemptyset(&set);
	sigaddset(&set, SIGINT);
	sigprocmask(SIG_SETMASK, &set, NULL);


	printf("start\n");

	while(1){
		sleep(1);
	
		signal(SIGTERM, sigterm_handler);


	}
	


	return 0;
}
