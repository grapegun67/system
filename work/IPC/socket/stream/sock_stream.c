#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>         
#include <sys/socket.h>
#include <sys/un.h>

#define SOCK_PATH	"sock_stream_un"

void print_usage(const char *name)
{
	printf("%s (sever | client)\n", name);
}

int stream_send(int sock, void *buf, size_t buflen, int flag)
{
	int written = 0;
	int ret;

	while(written < buflen){	
	
		ret =  send(sock, (char *)buf + written, buflen + written, flag);

		if(ret == -1){
			return ret;
		}
		
		written += ret;
	}

	return 0;
}

int stream_recv(int sock, void *buf, size_t buflen, int flag)
{
	int written = 0;
	int ret;

	while(written < buflen){	
	
		ret =  recv(sock, (char *)buf + written, buflen + written, flag);

		if(ret == -1){
			return ret;
		}
		
		written += ret;
	}

	return 0;
}



int do_server()
{
	int sock,peer,ret;
	char buf[1024];
	sock = socket(AF_UNIX, SOCK_STREAM, 0);
	if(sock == -1){
		perror("socket() error\n");
		return 0;
	}

	struct sockaddr_un addr;
	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, SOCK_PATH, sizeof(addr.sun_path)-1);

	if(bind(sock, (struct sockaddr *)&addr, sizeof(struct sockaddr_un)) == -1){
		perror("bind()-error\n");
		close(sock);
		return 0;
	}

	listen(sock, 5);

	peer = accept(sock, NULL, NULL);
	if(peer == -1){
		perror("accept()-error\n");
		close(sock);
		return 0;
	}

	memset(buf, 0, sizeof(buf));


	//ret = recv(peer, buf, sizeof(buf), 0);
	
	ret = stream_recv(peer, buf, sizeof(buf), 0);

	if(ret < 0){
		perror("recv()- error\n");
		close(sock);
		return 0;
	}

	printf("clientmessage: %s\n", buf);
	
	close(peer);
	close(sock);

	return 0;
}


int do_client()
{

	int sock,ret;
	char buf[1024];
	struct sockaddr_un addr;
	sock = socket(AF_UNIX, SOCK_STREAM, 0);
	if(sock < 0){
		perror("socket()-error\n");
		return 0;
	}

	memset(&addr, 0 , sizeof(addr));
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, SOCK_PATH, sizeof(addr.sun_path)-1);

	if(connect(sock, (struct sockaddr *)&addr, sizeof(struct sockaddr_un)) == -1){
		perror("connect()-error\n");
		close(sock);
		return 0;
	}

	memset(buf,0,sizeof(buf));
	snprintf(buf, sizeof(buf), "system programming\n");

	//ret = send(sock, buf, sizeof(buf), 0);
	
	
	ret = stream_send(sock, buf, sizeof(buf), 0);

	if(ret < 0){
		error("send()-error\n");
		close(sock);
		return 0;
	}

	close(sock);

	return 0;
}


int main(int argc, char **argv)
{
	int ret;

	if(argc < 2){
		print_usage(argv[0]);
		return 0;
	}

	else if (!strcmp(argv[1], "server")){
		ret=do_server();
	}
	
	else if (!strcmp(argv[1], "client")){
		ret=do_client();
	}

	else{
		print_usage(argv[0]);
		return 0;
	}


	return ret;

}
