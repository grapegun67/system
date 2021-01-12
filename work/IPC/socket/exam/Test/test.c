#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <pthread.h>



#define PATHNAME        "tty"
#define LENGTH          1024


struct client{
        int client_sockfd;
        pthread_t pt;
};

struct client client[3];
int c_num = 0;


void *client_func(void *args)
{
        int *sockfd = (int *)args;
        char buf[LENGTH];

        memset(buf, 0, sizeof(buf));
        strncpy(buf, "good", sizeof(buf));

        while(1){
                sleep(2);
                send(*sockfd, buf, sizeof(buf),0);
        }
}


void do_read()
{
        int sockfd, client_sockfd;
        struct sockaddr_un sockaddr;
        struct sockaddr_un client_sockaddr;
        char buf[LENGTH];

        memset(buf, 0, sizeof(buf));
	memset(&sockaddr, 0, sizeof(sockaddr));
        memset(&client_sockaddr, 0, sizeof(client_sockaddr));
        sockaddr.sun_family = AF_UNIX;
        strncpy(sockaddr.sun_path, PATHNAME, sizeof(sockaddr.sun_path)-1);


        sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
        if(sockfd == -1){
                perror("socket()-error\n");
                return;
        }

        if(bind(sockfd, (struct sockaddr *)&sockaddr, sizeof(struct sockaddr_un)) == -1){
                perror("bind()-error\n");
                return;
        }


        listen(sockfd, 3);

        printf("waiting connecting....\n");

        while(1){
		client[c_num].client_sockfd = accept(sockfd, NULL, NULL);
                if(client[c_num].client_sockfd < 1){
                        printf("[%d]socket exit connect\n", client[c_num].client_sockfd);
                        return;
                }
                printf("[%d]socket success connect\n", client[c_num].client_sockfd);

                pthread_create(&client[c_num].pt, NULL, client_func, (void *)&client[c_num].client_sockfd);

                c_num++;
        }
}

void do_write()
{
        int sockfd;
        char buf[LENGTH];
        struct sockaddr_un sockaddr;


        memset(&sockaddr, 0, sizeof(sockaddr));
        memset(buf, 0, sizeof(buf));
        sockaddr.sun_family = AF_UNIX;
        strncpy(sockaddr.sun_path, PATHNAME, sizeof(sockaddr.sun_path)-1);

        sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
        if(sockfd == -1){
                perror("socket()-error\n");
                return;
        }

        if(connect(sockfd, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) == -1){
                perror("connect()-error\n");
                return;
        }


        printf("connect success\n");

	while(1){
                memset(buf, 0, sizeof(buf));
                recv(sockfd, buf, sizeof(buf), 0);
                printf("result: %s\n", buf);
        }

}

void print_usage(char *name)
{
        printf("%s (read | write word)\n", name);
}

int main(int argc, char **argv)
{

        if(argc == 2 && !strcmp(argv[1], "read")){
                do_read();
        }

        else if(argc == 2 && !strcmp(argv[1], "write")){
                do_write();
        }

        else{
                print_usage(argv[0]);
        }


        return 0;
}

