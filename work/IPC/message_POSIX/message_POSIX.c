#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <fcntl.h>           
#include <sys/stat.h>        
#include <mqueue.h>

#define POSIX_MQ_NAME		"/test"
#define MSGBUF_STR_SIZE 	8192

void print_usage(char *name)
{
	printf("%s (send priority |recv)\n", name);
}

int init_msgq(void)
{
	mqd_t mqd;
	struct mq_attr attr;
	mqd = mq_open(POSIX_MQ_NAME, O_RDWR | O_CREAT , 0644, NULL);

	if(mqd == -1){
		perror("mq_open()-error\n");
		return 0;
	}

	memset(&attr, 0, sizeof(attr));
	if(mq_getattr(mqd, &attr) == -1){
		close(mqd);
		return 0;
	}

	printf("mq_flag		= %ld\n", attr.mq_flags);
	printf("mq_maxmsg	= %ld\n", attr.mq_maxmsg);
	printf("mq_msgsize	= %ld\n", attr.mq_msgsize);
	printf("mq_curmsgs	= %ld\n", attr.mq_curmsgs);

	return mqd;
}

int do_recv()
{
	int mqd;
	char buf[MSGBUF_STR_SIZE];
	unsigned priority;

	mqd = init_msgq();
	if(mqd == -1){
		perror("init_msgq()-error\n");
		return -1;
	}
	
	memset(buf, 0, sizeof(buf));
	if(mq_receive(mqd, buf, sizeof(buf), &priority) == -1){
		perror("mq_receive()-error\n");
		close(mqd);
		return-1;
	}

	printf("priority: %u, msg [%s]\n", priority,  buf);
	
	return 0;
}


int do_send(unsigned int priority)
{
	int mqd;
	char buf[MSGBUF_STR_SIZE];
	mqd = init_msgq();
	if(mqd == -1){
		perror("init_msgq()-error\n");
		return -1;
	}
	
	memset(buf, 0, sizeof(buf));
	snprintf(buf, sizeof(buf), "hello from pid: %d", getpid());
	if(mq_send(mqd, buf, sizeof(buf), priority) == -1){
		perror("mq_send()-error\n");
		close(mqd);
		return-1;
	}

	return 0;
}

int main(int argc, char **argv)
{
	int ret;
	long mtype;

	if(argc < 2){

		print_usage(argv[0]);
		return -1;
	}

	if(!strcmp(argv[1], "send")){
		unsigned int priority;
		
		if(argc < 3){
			print_usage(argv[0]);
			return 0;	
		}

		priority = (unsigned int)strtoul(argv[2], NULL, 10);
		ret = do_send(priority);
		
	}
	
	else if(!strcmp(argv[1], "recv")){
		ret=do_recv();
	}
	
	else{
		print_usage(argv[0]);
		return -1;
	}

	return 0;
}
