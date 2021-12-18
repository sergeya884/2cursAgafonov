#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h> 
#include <stdlib.h>


int main(int argc, char *argv[])
{
	if (argc != 3) {
		fprintf(stderr, "Usage: %s read-filename write-filename\n", argv[0]);
		return 1;
	}
	//rf файл для чтения, wf - для записи
	int rf=open(argv[1], O_RDONLY);
	char buffer[500];
	if(rf <= 0) {
		printf("Cannot open file for reading");
		return 2;
	}

        int wf = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if(wf<0){
		perror("Failed to open file for writing");
		return 3;
	}

	//циклически переписываем по 500
	off_t offset=0;
	ssize_t i;
	while(1==1){
		i = pread(rf, buffer, sizeof(buffer), offset);
		if (i<0) {
			printf("Read Error");
			return 4;
		}
		if (i==0) break;
	        if (pwrite(wf,buffer,i, offset)<=0){
			printf("Write Error");	
			return 5;
		}
		offset+=sizeof(buffer);
	}

	//закрываем файлы
	if (close(rf)<0){
		perror("Failure write closing rf");
		return 6;
	}
	if (close(wf)<0){
		perror("Failure write closing wf");
		return 7;
	}
	return 0;

}
