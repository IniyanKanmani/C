#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>


int main(){

	
	int fd;
	char buffer[100];

	fd = open("/dev/chardev", O_RDONLY, 0777);
	if(fd < 0){
		printf("Error!\n");
		exit(1);
	}

	int readCount;
	readCount = read(fd, buffer, 1);

	while(readCount > 0){

		printf("%.*s",readCount,buffer);
		readCount = read(fd, buffer, 1);

	}

	printf("\n");

	if(close(fd) < 0){
		printf("Error!\n");
		exit(1);
	}

	return(0);
}

