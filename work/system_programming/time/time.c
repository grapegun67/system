#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <sys/time.h>



void print_cur_time()
{
	time_t now;
	struct tm *now_tm;


	now = time(NULL);

        printf("current time: %ld\n", now);

        now_tm = localtime(&now);
        if(!now_tm){
                printf("localtime()-error\n");
                return; 
        }


	printf("%d/%d/%d %d:%d:%d\n", now_tm->tm_year+1900, now_tm->tm_mon+1, now_tm->tm_mday, now_tm->tm_hour, now_tm->tm_min, now_tm->tm_sec);

}

void signalarm_handler(int signum)
{
	printf("\ngot SIGALRM\n");
	print_cur_time();
}


int main(int argc, char **argv)
{
	struct itimerval itimer;
	itimer.it_interval.tv_sec = 1;
	itimer.it_interval.tv_usec = 0;
	itimer.it_value.tv_sec = 5;	
	itimer.it_value.tv_usec = 0;	

	signal(SIGALRM, signalarm_handler);


	print_cur_time();


	if(setitimer(ITIMER_REAL, &itimer, NULL) != 0){
		printf("settimer()-error\n");	
		return 0;
	}


	while(1){
		sleep(1);
	}


	return 0;
}



