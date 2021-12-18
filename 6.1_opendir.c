//Прогррамма выводит тип всех файлов в текущем каталоге
#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

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
		
		//lstat вызывыется только если dtype не опознал
		if (entry_type == '?') {
			struct stat sb;
			lstat(entry->d_name, &sb);
			entry_type = tipe(sb.st_mode);
		}
		printf(" %c %s\n", entry_type, entry->d_name);
	}

	closedir(dir);
	return 0;
}
