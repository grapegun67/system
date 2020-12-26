#include <stdio.h>
#include <string.h>

int write_file()
{
	FILE *fp;
	
	fp = fopen("data", "w");
	if(fp == NULL){
		perror("fopen()-error\n");
		return 0;
	}

	fputs("system programming\n" ,fp);
	

	fclose(fp);

}

int read_file()
{
	FILE *fp;
	char buf[1024];
	memset(buf, 0, sizeof(buf));

	fp = fopen("data", "r");
	if(fp == NULL){
		perror("fopen()-error\n");
		return 0;
	}

	fgets(buf, sizeof(buf), fp);
	printf("result: %s\n", buf);

	fclose(fp);
}


int main(int argc, char **argv)
{

	if(write_file()){
		perror("write_file()-error");
		return 0;
	}


	if(read_file()){
		perror("read_file()-error\n");
		return 0;
	}

	return 0;
}
