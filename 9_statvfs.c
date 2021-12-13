#include <stdio.h>
#include <sys/statvfs.h>
//На вход подается файловая система(папака), выводится информация о файловой системе

int main(int argc, char* argv[]) {

	struct statvfs fs;

	if (argc != 2) {
		fprintf(stderr, "Usage: %s <file-system>\n", argv[0]);
		return 1;
	}

	if (statvfs(argv[1], &fs) < 0) {
		perror("F to statvfs");
		return 2;
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

	return 0;
}
