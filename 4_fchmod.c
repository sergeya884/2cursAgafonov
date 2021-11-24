#include <unistd.h> // for read
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <fcntl.h> /* Definition of AT_* constants */
#include <sys/stat.h>

//

// read()
// ssize_t read(int fd, void *buf, size_t count);
// returns number of bites(file posision) on success
// and -1 if smth wrong
// read() attempts to read up to count bytes from file descriptor fd into the buffer starting at buf.

int main(int argc, char *argv[])
{
	//Int fd1, fd2, metadata, f;
	char buffer[100000] = "";
	unsigned int bytes;
	struct stat sb;

	scanf("%u", &bytes);
	int fd1 = open(argv[1], O_RDONLY, 0644);
	int fd2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	bytes = read(fd1, buffer, bytes);
	int f = fstat(fd1, &sb); 
	int metadata = fchmod(fd2, sb.st_mode);
	unsigned int bytes_out = write(fd2, buffer, bytes);
	
	if(fd1 == -1) {
	perror("Ошибка открытия файла");
	exit(1);
	}
	if(fd2 == -1) {
	perror("Ошибка открытия файла");
	exit(1);
	}
	if(metadata == -1) {
	printf("Ошибка копирования метаданных");
	exit(1);
	}
	if (bytes == -1){
	perror ("Possible read error.");
	exit(1);
	}
	if(bytes_out == -1){
	perror("Write Error");
	exit(1);
	}

	close(fd1);
	close(fd2);

	return 0;
}

