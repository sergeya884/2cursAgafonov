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
	char buffer[500];
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

	//Переписываем по 500
	ssize_t i;
	while(1==1){
		i = read(rf, buffer, sizeof(buffer));
		if (i<0) {
			printf("Read Error");
			return 4;
		}
		if (i==0) break;
	        if (write(wf,buffer,i)<=0){
			printf("Write Error");	
			return 5;
		}
	}

	//Копируем права
	if(fchmod(wf, sb.st_mode)<0){
		perror("F to fchmod");
		exit(5);
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

