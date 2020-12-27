#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/inotify.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>





int main(int argc, char **argv)
{

	int fd,ret;
	int wd1;
	int wd2;
	char buf[1024];
	fd_set fds;
	struct timeval timeout;
	memset(buf, 0, sizeof(buf));
	struct inotify_event *event;



	fd = inotify_init();
	if(fd == -1){
		perror("inotify_init()-error\n");
		return 0;
	}



	wd1 = inotify_add_watch(fd, ".", IN_CREATE | IN_DELETE);
	if( wd1 == -1){
		perror("inotify_add_watch()-error\n");
		close(fd);
		return 0;
	}	
	

	wd2 = inotify_add_watch(fd, "../", IN_CREATE | IN_DELETE);
	if( wd2 == -1){
		perror("inotify_add_watch()-error\n");
		close(fd);
		return 0;
	}	


	STDIN_FILENO


	while(1){

		ret = read(fd, buf, sizeof(buf));
		if(ret == -1){
			perror("read()-error\n");
			return 0;
			
		}	

		FD_ZERO(&fds);
		FD_SET(fd, &fds); 
		FD_SET(STDIN_FILENO, &fds);
		timeout.tv_sec = 5;
		timeout.tv_usec = 0;	


		ret = select(fd+1, &fds, NULL, NULL, timeout);
		if(ret == -1){
			perror("select()-error\n");
			return 0;
		}

		else if(ret == 0){
			printf("select()-timeout\n');
		}	

		else if(ret > 0){
			
				if(FD_ISSET(fd, &fds)){
					event = (struct inotify_event *)buf;
      
     	 	        	        while(ret > 0){
      
                	        	     if(event->mask & IN_CREATE){
                                		      printf("file %s is created\n", event->name);
                        	      	     }
      
     
		                             if(event->mask & IN_DELETE){
        		                             printf("file %s is deleted\n", event->name);
                		             }
     
                        		     event = (struct inotify_event *)((char *)event + sizeof(struct inotify_event) + event->len);
	                        	     ret -= (sizeof(struct inotify_event) + event->len);
     
					}

				}

				else if(FD_ISSET(STDIN_FILENO, &fds)){

				}
	
		}


	close(wd1);
	close(wd2);
	close(fd);
	return 0;
}
