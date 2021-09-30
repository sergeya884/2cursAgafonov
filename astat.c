#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

int tipe(struct stat st)
{
  char t;
  switch (st.st_mode & S_IFMT) {
    case S_IFBLK:  t='b'; break;
    case S_IFCHR:  t='c'; break;
    case S_IFDIR:  t='d'; break;
    case S_IFIFO:  t='p'; break;
    case S_IFLNK:  t='l'; break;
    case S_IFREG:  t='-'; break;
    case S_IFSOCK: t='s'; break;
    default:       t='?'; break;
    }
  return(t);
}


int main(int argc, char *argv[])
{
    char t;
    char buf [80]; 
    struct stat st;
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <pathname>\n", argv[0]);//переделал под шаблон
        exit(EXIT_FAILURE);
    }
    if (lstat(argv[1], &st) == -1) {
        perror("F to stat");
        exit(EXIT_FAILURE);
    }
    t=tipe(st);
    printf("Тип файла:                %c\n", t);
    printf("номер inode:              %ld\n", (long) st.st_ino);
    long i = st.st_mode; //Права доступа в числах
    printf("Режим доступа:            %lo/%c", i, t);
    //Перевод прав доступа из чисел в буквы
    for(int j=512; j>1; j/=8) {
        i%=j;
        if (i/(j/8)==1) printf("--x");
        else if (i/(j/8)==2) printf("-w-");
        else if (i/(j/8)==3) printf("-wx");
        else if (i/(j/8)==4) printf("r--");
        else if (i/(j/8)==5) printf("r-x");
        else if (i/(j/8)==6) printf("rw-");
        else if (i/(j/8)==7) printf("rwx");
    }

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

