#include <stdio.h>
#include <string.h>


int create_file()
{
	FILE *fp;
	
	fp = fopen("datafile", "w");
	if(fp == NULL){
		printf("fopen()-error\n");
		return 0;
	}



	printf("write offset: %ld\n", ftell(fp));
	fputs("system programming\n",fp);
	fputs("system programming\n",fp);
	fputs("system programming\n",fp);
	fputs("system programming\n",fp);
	printf("write offset: %ld\n\n", ftell(fp));


	fclose(fp);
	return 0;
}


int read_file()
{
	FILE *fp;
	char buf[1024];
	memset(buf,0, sizeof(buf));
	
	
	fp = fopen("datafile", "r+");
	if(fp == NULL){
		printf("fopen()-error\n");
		return 0;
	}

	printf("read offset: %ld\n", ftell(fp));
	fgets(buf, sizeof(buf), fp);
	printf("read offset: %ld\n", ftell(fp));
	printf("\nresult: %s\n", buf);

	fseek(fp, 0, SEEK_END);
	printf("end offset: %ld\n", ftell(fp));

	fputs("end", fp);
	printf("final offset: %ld\n", ftell(fp));


	fclose(fp);
	return 0;
}


int main(int argc, char **argv)
{
	create_file();
	read_file();	

	return 0;
}
