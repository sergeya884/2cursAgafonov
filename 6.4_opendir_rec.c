//Программа получает на вход абсолютный путь до директории и рекурсивно выводит /*права доступа и*/ тип всех файлов в ней и во всех директориях внутри нее
#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
/*
//Перевод прав доступа из чисел в буквы
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
*/
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



//рекурсивный вывод всего в папке
int rec_chek(char *name, int tire, char *path){
	if (chdir(name)<0) return 1;

	DIR *dir = opendir(".");
	if (!dir) {
		perror("Can't open current directory");
        	return 2;
	}

	struct dirent* entry;
	//Пока есть файлы
	while((entry = readdir(dir)) != NULL) {
		for(int j=0; j<tire; ++j) printf("----"); //Выводим необходимое количество тире для читаемости структуры 
		printf("/ ");

		//Определяем тип файла 
		char entry_type = dtype(entry->d_type);
		//lstat вызывыется только если dtype не опознал
		if (entry_type == '?') {
			struct stat sb;
			lstat(entry->d_name, &sb);
			entry_type = tipe(sb.st_mode);
		}
		printf(" %c %s\n", entry_type, entry->d_name);

		//Если это дирректория и не "." или "..", то переходим в нее
		if ((entry_type == 'd') && (strcmp(entry->d_name,"..") != 0) && (strcmp(entry->d_name,".") != 0)){			
			rec_chek(entry->d_name, tire+1, path);
		}

				
	}
	closedir(dir);
	if (chdir(path)<0) return 1;
	}

int main(int argc, char *argv[]) {
	if (argc != 2) {
		fprintf(stderr, "Usage: %s directorypath\n", argv[0]);
		return 1;
	}

	//Переходим в нужную директорию
	if (chdir(argv[1])<0) {
		perror("Can't found directory");
        	return 3;
	}

	rec_chek(".", 0,argv[1]);
	
	return 0;
}
