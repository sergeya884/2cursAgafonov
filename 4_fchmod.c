//Программа копирует содержимое одного файла в другой блоками, так же копирует права доступа
#include <unistd.h> 
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <fcntl.h> 
#include <sys/stat.h>

int main(int argc, char *argv[])
{
	if (argc != 3) {
		fprintf(stderr, "Usage: %s <read-filename> <write-filename>\n", argv[0]);
		exit(1);
	}

	int rf, wf;
	char buffer[100];
	struct stat sb;

	if ((rf=open(argv[1], O_RDONLY, 0644<0))<0) {	
		perror("Cannot open file for reading");
		exit(2);
	}
	
	if ((wf = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644))<0) {
		perror("Failed to open file for writing");
		exit(3);
	}

	if(fstat(rf, &sb)<0) {
		perror("F to fstat");
		exit(4);
		}
 
	if(fchmod(wf, sb.st_mode)<0){
		perror("F to fchmod");
		exit(5);
		}

	size_t i = 50;
	while(i == 50){
		i = read(rf, buffer, 50);
		if (i<0) {
			perror("Read Error");
			exit(6);
		}
	        if (write(wf,buffer,i)<=0){
			perror("Write Error");	
			exit(7);
		}
	}

	if (close(rf)<0){
		perror("Failure write closing rf");
		exit(8);
	}
	if (close(wf)<0){
		perror("Failure write closing wf");
		exit(9);
	}

	return 0;
}

