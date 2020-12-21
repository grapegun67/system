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

	printf("%s (send|recv)\n", name);
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

int do_recv()
{
	int msgq,ret;
	struct msgbuf mbuf;
	
	msgq=init_msgq();
	if(msgq == -1){
		perror("init_msgq()-error\n");
		return -1;
	}

	memset(&mbuf, 0, sizeof(mbuf));
	ret = msgrcv(msgq, &mbuf, sizeof(mbuf.string), 0, 0);
	if(ret == -1){
		perror("msgrcv()-error\n");
		return -1;
	}
	printf("recv msg: mtype: %ld, msg [%s]\n", mbuf.mtype, mbuf.string);
}


int do_send()
{
	int msgq;
	msgq = init_msgq();
	if(msgq == -1){
		perror("init_msgq()-error\n");
		return -1;
	}
	struct msgbuf mbuf;

	memset(&mbuf, 0, sizeof(mbuf));
	mbuf.mtype=1;
	strncpy(mbuf.string, "system programming", sizeof(mbuf.string)-1);
	if(msgsnd(msgq, &mbuf, sizeof(mbuf.string), 0) == -1 ){

		perror("msgnd()-error\n");
		return -1;
	}
	
	return 0;
}

int main(int argc, char **argv)
{
	int ret;

	if(argc < 2){

		print_usage(argv[0]);
		return -1;
	}

	if(!strcmp(argv[1], "send")){
		ret=do_send();

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
