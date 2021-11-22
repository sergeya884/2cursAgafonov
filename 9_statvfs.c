#include <stdio.h>
#include <sys/statvfs.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

//На вход подается файловая система(папака) и файл, выводится информация о файловой системе и поместится ли файл в файловую систему 

int main(int argc, char* argv[]) {

	struct statvfs fs;

	struct stat sb;

	if (argc != 3) {
		fprintf(stderr, "Usage: %s file-system filename\n", argv[0]);
		return 1;
	}


	if (lstat(argv[1], &sb) < 0) {
		perror("F to lstat");
		return 2;
	}

	//Проверка соответствия файловой системы
	if ((sb.st_mode & S_IFMT) != S_IFDIR) {
		fprintf(stderr, "[err] %s isn't a file system\n", argv[1]);
		return 3;
	}

	if (statvfs(argv[1], &fs) < 0) {
		perror("F to statvfs");
		return 4;
	}

	if (lstat(argv[2], &sb) < 0) {
		perror("F to lstat");
		return 5;
	}

	//Вывод информации о файловой системе
	printf("Filesystem block size:                          %lu\n", fs.f_bsize);
	printf("Fragment size:                                  %lu\n", fs.f_frsize);
	printf("Size of filesystem in fragment size units:      %lu\n", fs.f_blocks);
	printf("Number of free blocks:                          %lu\n", fs.f_bfree);
	printf("Number of free blocks for unprivileged users:   %lu\n", fs.f_bavail);
	printf("Number of inodes:                               %lu\n", fs.f_files);
	printf("Number of free inodes:                          %lu\n", fs.f_ffree);
	printf("Number of free inodes for unprivileged users:   %lu\n", fs.f_favail);
	printf("Filesystem ID:                                  %lu\n", fs.f_fsid);
	printf("Mount flags:                                    %lu\n", fs.f_flag);
	printf("Maximum filename length:                        %lu\n", fs.f_namemax);

	//Проверка влезет ли файл
	if ((fs.f_favail < 1) || ((long int) (fs.f_bavail * fs.f_bsize) < sb.st_size)) {
		printf("\nThe file will not fit into the folder\n");
	}
	else {
		printf("\nThe file will fit into the folder\n");
	}

	return 0;
}
