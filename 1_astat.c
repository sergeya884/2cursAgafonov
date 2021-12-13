#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
//#include "functions.h"

const char tipe(unsigned mod)
{
  switch (mod & S_IFMT) {
    case S_IFBLK:  return 'b';
    case S_IFCHR:  return 'c';
    case S_IFDIR:  return 'd';
    case S_IFIFO:  return 'p';
    case S_IFLNK:  return 'l';
    case S_IFREG:  return '-';
    case S_IFSOCK: return 's';
    default:       return '?';
    }
}

void file_access(int i) {
	for(int j=512; j>1; j/=8) {
        	i%=j;
		switch (i/(j/8)) {
	        	case 1: printf("--x"); break;
			case 2: printf("-w-"); break;
			case 3: printf("-wx"); break;
			case 4: printf("r--"); break;
			case 5: printf("r-x"); break;
			case 6: printf("rw-"); break;
			case 7: printf("rwx"); break;
		}
	}
}

int main(int argc, char *argv[])
{
    char buf [80]; 
    struct stat st;
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <pathname>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    if (lstat(argv[1], &st) == -1) {
        perror("F to stat");
        exit(EXIT_FAILURE);
    }

    printf("Тип файла:                %c\n", tipe(st.st_mode));
    printf("номер inode:              %ld\n", (long) st.st_ino);
    int i = st.st_mode; //Права доступа в числах
    printf("Режим доступа:            %o/%c", i, tipe(st. st_mode));
    //Перевод прав доступа из чисел в буквы
    file_access(i);   
    printf("\nКол-во ссылок:            %ld\n", (long) st.st_nlink);
    printf("Владелец:                 UID=%ld   GID=%ld\n", (long) st.st_uid, (long) st.st_gid);
    printf("Предпоч. размер бл. в/в:  %ld байт\n", (long) st.st_blksize);
    printf("Размер файла:             %lld байт\n", (long long) st.st_size);
    printf("Выделено блоков:          %lld\n", (long long) st.st_blocks);
    // переделал под strftime
    strftime (buf,80,"%x %X %Z",localtime(&st.st_ctime));
    printf("Посл. изм. состояния:     %s\n", buf);
    strftime (buf,80,"%x %X %Z",localtime(&st.st_atime));
    printf("Посл. доступ к файлу:     %s\n", buf);
    strftime (buf,80,"%x %X %Z",localtime(&st.st_mtime));
    printf("Посл. изм. файла:         %s\n", buf);

    exit(EXIT_SUCCESS);
    return 1;
}

