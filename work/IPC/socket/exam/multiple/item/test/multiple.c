#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>          
#include <sys/socket.h>
#include <sys/un.h>
#include <pthread.h>


#define PATHNAME	"abcd"
#define	LENGTH		1024

typedef struct Client_info{

	int client_sockfd;
	pthread_t pt;

}Client_info;


typedef struct Client_node{

	Client_info data;	
	struct client_node *next;

}Client_node;

/* head_list */
Client_node *client_head_list; 

void insert_node(Client_node *head, int client_sockfd)
{
	Client_node *new_node = (Client_node *)malloc(sizeof(Client_node));
	
	new_node->data.client_sockfd = client_sockfd;
	new_node->next = head->next;
	head->next = new_node;
}


void print_node_list(Client_node *head)
{
	Client_node *tmp_node = head->next;

	printf("client list: ");

	while(1){

		if(tmp_node != NULL){
			printf("%d, ", tmp_node->data.client_sockfd);
			tmp_node = tmp_node->next;
		}

		else if(tmp_node == NULL){
			break;
		}
	}
	printf("\n");
}


int explore_node(int num)
{
	Client_node *node = client_head_list->next;	

	while(1){

		if(node->data.client_sockfd == num){
			return num;
		}

		else if(node->next == NULL){
			return -1;
		}
	}
}


void *fight_pthread(void *args)
{
	int tmp, num, fight_sockfd;
	char buf[LENGTH];
	char person1[LENGTH];
	char person2[LENGTH];
	int *tmp_sockfd = (int *)args;


	/* initialize connect */
	memset(buf, 0, sizeof(buf));
	strncpy(buf, "choice person: ", sizeof(buf));	
	if(send(*tmp_sockfd, buf, sizeof(buf), 0) == -1){
		perror("send()-error\n");
		return NULL;
	}


	memset(buf, 0, sizeof(buf));
	if(recv(*tmp_sockfd, buf, sizeof(buf), 0) == -1){
		perror("recv()-error\n");
		return NULL;
	}


	num = atoi(buf);
	fight_sockfd = explore_node(num);
	if(fight_sockfd == -1){
		printf("%d do not existed\n", num);
		return NULL;
	}

	/* fight loop */

	memset(buf, 0, sizeof(buf));
	strncpy(buf, "2*3 = ?", sizeof(buf));

	send(*tmp_sockfd , buf, sizeof(buf), 0);
	send(fight_sockfd, buf, sizeof(buf), 0);

	recv(*tmp_sockfd ,person1, sizeof(person1), 0);
	recv(fight_sockfd ,person2, sizeof(person2), 0);

	/* person1 */
	if(2*3 == atoi(person1)){
		
		memset(buf, 0, sizeof(buf));
		strncpy(buf, "correct", sizeof(buf));
		send(*tmp_sockfd, buf, sizeof(buf), 0);
	}
	else{
		memset(buf, 0, sizeof(buf));
		strncpy(buf, "incorrect", sizeof(buf));
		send(*tmp_sockfd, buf, sizeof(buf), 0);
	}

	/* person2 */
	if(2*3 == atoi(person2)){
		
		memset(buf, 0, sizeof(buf));
		strncpy(buf, "correct", sizeof(buf));
		send(fight_sockfd, buf, sizeof(buf), 0);
	}

	else{
		memset(buf, 0, sizeof(buf));
		strncpy(buf, "incorrect", sizeof(buf));
		send(fight_sockfd, buf, sizeof(buf), 0);
	}

}


int main(int argc, char **argv)
{
	/* head_list initialize */

	client_head_list = (Client_node *)malloc(sizeof(Client_node));
	client_head_list->next = NULL;

	/* initiallize variable  */

	int sockfd, client_sockfd;
	struct sockaddr_un sockaddr;

	/* memory setting */

	memset(&sockaddr, 0, sizeof(sockaddr));
	sockaddr.sun_family = AF_UNIX;
	strncpy(sockaddr.sun_path, PATHNAME, sizeof(sockaddr.sun_path)-1);


	sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
	if(sockfd == -1){
		perror("socket()-error\n");
		return 0;
	}


	if(bind(sockfd, (struct sockaddr *)&sockaddr, sizeof(struct sockaddr_un)) == -1){
		perror("bind()-error\n");	
	}

	listen(sockfd, 5);

	
	while(1){
		
		printf("wait connecting...\n");

		client_sockfd = accept(sockfd, NULL, NULL);
		if(client_sockfd == -1){
			perror("accept()-error\n");
			return 0;
		}

		printf("client connected\n");

		insert_node(client_head_list, client_sockfd);
		print_node_list(client_head_list);


		Client_node *tmp_node = client_head_list->next;
		pthread_create(&(tmp_node->data.pt), NULL, fight_pthread, (void *)&client_sockfd); 
	}


	return 0;
}
