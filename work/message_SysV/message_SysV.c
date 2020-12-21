#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define IPC_KEY_FILENAME "/proc"
#define IPC_KEY_PROJ_ID 'a'
#define MSGBUF_STR_SIZE 64

struct msgbuf{

	long mtype;
	char string[MSGBUF_STR_SIZE];
};



void print_usage(char *name)
{

	printf("%s (send|recv) mtype \n", name);
}

int init_msgq(void)
{
	int msgq;
	key_t key;

	key = ftok(IPC_KEY_FILENAME, IPC_KEY_PROJ_ID);
	if(key == -1){
		perror("ftok()-error\n");
		return 0;
	}
	
	msgq = msgget(key, 0644 | IPC_CREAT);
	if(msgq == -1){
		perror("msgget()-error\n");
		return 0;
	}
	
	return msgq;
}

static int do_recv(long mtype)
{
	int msgq,ret;
	struct msgbuf mbuf;
	
	msgq=init_msgq();
	if(msgq == -1){
		perror("init_msgq()-error\n");
		return -1;
	}

	memset(&mbuf, 0, sizeof(mbuf));
	ret = msgrcv(msgq, &mbuf, sizeof(mbuf.string), mtype, 0);
	if(ret == -1){
		perror("msgrcv()-error\n");
		return -1;
	}
	printf("recv msg: mtype: %ld, msg [%s]\n", mbuf.mtype, mbuf.string);
}


int do_send(long mtype)
{
	int msgq;
	msgq = init_msgq();
	if(msgq == -1){
		perror("init_msgq()-error\n");
		return -1;
	}
	struct msgbuf mbuf;

	memset(&mbuf, 0, sizeof(mbuf));
	mbuf.mtype=mtype;
	snprintf(mbuf.string, sizeof(mbuf.string), "hello world mtype %ld",mtype);
	if(msgsnd(msgq, &mbuf, sizeof(mbuf.string), 0) == -1 ){

		perror("msgnd()-error\n");
		return -1;
	}
	
	return 0;
}

int main(int argc, char **argv)
{
	int ret;
	long mtype;

	if(argc < 3){

		print_usage(argv[0]);
		return -1;
	}

	mtype = strtol(argv[2], NULL, 10);
	
	if(!strcmp(argv[1], "send")){
		if(mtype <= 0){
			print_usage(argv[0]);
			return 0;
		}	
		ret = do_send(mtype);
	}
	
	else if(!strcmp(argv[1], "recv")){
		ret=do_recv(mtype);
		
	}
	
	else{
		print_usage(argv[0]);
		return -1;
	}




	return 0;
}
