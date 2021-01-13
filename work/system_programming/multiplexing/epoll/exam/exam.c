#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/epoll.h>


#define LENGTH	1024

int main(int argc, char **argv)
{
	int epfd,num,tmp;
	char buf[LENGTH];
	struct epoll_event event;

	epfd = epoll_create1(0);
	if(epfd == -1){
		perror("epoll_create1()-error\n");
		return 0;
	}


	memset(&event, 0, sizeof(event));
	event.events = EPOLLIN;
	event.data.fd = STDIN_FILENO;

	if(epoll_ctl(epfd, EPOLL_CTL_ADD, STDIN_FILENO, &event) == -1){
		perror("epoll_ctl()-error\n");
		return 0;
	}

	while(1){
		
		printf("epoll_wait.....\n");
		tmp = epoll_wait(epfd, &event, 1, 5000);

		if(tmp == -1){
			perror("epoll_wait()-error\n");
			return 0;
		}
	
		else if(tmp == 0){
			printf("timeout!\n");
		}

		else if(tmp > 0){
			memset(buf, 0, sizeof(buf));
			if(read(STDIN_FILENO, buf, sizeof(buf)) == -1){
				perror("read()-error\n");
				return 0;
			}

			printf("input: %s\n", buf);
		}
		
	}

	return 0;
}
