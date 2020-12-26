#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>


#define SOCK_PATH	"datagram"


void print_usage(char * name)
{

	printf("%s (server | client)\n", name);
}


int do_server()
{
	int sock,ret;
	char buf[1024];

	sock = socket(AF_UNIX, SOCK_DGRAM, 0);

	if(sock == -1){
		perror("socket()-error\n");
		return 0;	
	}

	struct sockaddr_un addr;
	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, SOCK_PATH, sizeof(addr.sun_path)-1);

	if(bind(sock,(struct sockaddr *)&addr, sizeof(struct sockaddr_un)) == -1){
		perror("bind()-error\n");
		close(sock);
		return 0;
	}

	memset(buf, 0, sizeof(buf));

	ret = recvfrom(sock, buf, sizeof(buf), 0, NULL, NULL);

	if(ret < 0){
		perror("recvfrom()-error\n");	
		return 0;
	}

	printf("%s\n", buf);
	close(sock);

	return 0;
}

int do_client()
{
	int sock,ret;
	char buf[1024];

	sock = socket(AF_UNIX, SOCK_DGRAM, 0);
	if(sock == -1){
		perror("socket()-error\n");
		return 0;
	}

	struct sockaddr_un addr;
	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, SOCK_PATH, sizeof(addr.sun_path)-1 );
	
	memset(buf, 0, sizeof(buf));
	snprintf(buf, sizeof(buf), "linux system");

	ret = sendto(sock, buf, sizeof(buf), 0, (struct sockaddr *)&addr, sizeof(struct sockaddr_un));

	if(ret < 0){
		perror("sendto()-error\n");
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
	}

	else if(!strcmp(argv[1], "server")){
		ret = do_server();
	}

	else if(!strcmp(argv[1], "client")){
		ret = do_client();
	}

	else{
		print_usage(argv[0]);
	}


	return 0;
}
