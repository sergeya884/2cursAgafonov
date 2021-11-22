//Прогррамма выводит права доступа и тип всех файлов в текущем каталоге
#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <assert.h>

//Перевод прав доступа из чисел в буквы
void file_access(long i) {
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
}

//Нахождение типа файлов
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

//Тип директории
char dtype(unsigned char dtype) {

	switch (dtype) {
		case DT_BLK:      return 'b'; 
		case DT_CHR:      return 'c'; 
		case DT_DIR:      return 'd';
		case DT_FIFO:     return 'f';
		case DT_LNK:      return 'l'; 
		case DT_REG:      return 'r';
		case DT_SOCK:     return 's';
		case DT_UNKNOWN:  return '?';
	}

	return '?';
}


int main() {
	DIR *dir = opendir(".");

	if (!dir) {
		perror("Can't open current directory");
        return 1;
	}

	struct dirent* entry;

	while ((entry = readdir(dir)) != NULL) {

		char entry_type = dtype(entry->d_type);

		struct stat sb;
		assert((lstat(entry->d_name, &sb)) == 0);

		file_access(sb.st_mode);
        
		if (entry_type == '?') entry_type = tipe(sb.st_mode);

		printf(" %c %s\n", entry_type, entry->d_name);
	}

	closedir(dir);
	return 0;
}
