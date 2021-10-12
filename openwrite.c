#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h> 



int main(int argc, char *argv[])
{
	if (argc != 3) {
		fprintf(stderr, "Usage: %s filename text-to-write\n", argv[0]);
		return 1;
	}
	int fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if(fd<0){
		perror("Failed to open file for writing");
		return 2;
	}
        size_t written = 0;
	size_t len = strlen(argv[2]);
	while(written<len){
		size_t path = write(fd,argv[2]+written,len-written);
		if(path <0) {
			break;
			printf("Write Error");	
		}
		written+=path;
	}

	if (close(fd)<0){
		perror("Failure write closing fd");
		return 4;
	}
	return 0;
}
