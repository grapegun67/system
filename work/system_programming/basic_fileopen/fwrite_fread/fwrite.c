#include <stdio.h>
#include <string.h>
#include <stdlib.h>


struct Person{

	char name[20];
	int age;
};



int write_file()
{
	FILE *fp;
	struct Person kim ={
		.name = "kim",
		.age = 30
	};	

	struct Person lee ={
		.name = "Lee",
		.age = 24
	};

	fp = fopen("person", "w");
	if(fp == NULL){
		perror("fopen()-error\n");
		return 0;
	}

	if(fwrite(&kim, sizeof(struct Person), 1, fp) != 1){
		perror("fwrite()-error\n");
		return 0;
	}

	if(fwrite(&lee, sizeof(struct Person), 1, fp) != 1){
		perror("fwrite()-error\n");
		return 0;
	}

	fclose(fp);
	return 0;
}


int read_file()
{
	FILE *fp;
	struct Person person[2];

	fp = fopen("person", "r");
	if(fp == NULL){
		perror("fopen()-error\n");
		return 0;
	}

	if(fread(person, sizeof(struct Person), 2, fp) != 2){
		perror("fread()-error\n");
		return 0;
	}

	for(int i=0; i<2; i++){
		printf("name: %s, age: %d\n", person[i].name, person[i].age);
	}

	
	fclose(fp);
	return 0;
}





int main(int argc, char **argv)
{

	write_file();
	read_file();	

	return 0;
}

