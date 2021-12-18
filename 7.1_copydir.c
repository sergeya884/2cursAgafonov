//На вход программы подается директория-источник и директории-приемник, она копирует все регулярные файлы и директории(пустыми, чтобы использовать mkdir) из источника в приемник
#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/sysmacros.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char* argv[]) {

	if (argc != 3) {
		fprintf(stderr, "Usage: %s <directory-for-copy> <directory-for-write>\n", argv[0]);
		return 1;
	}

	//Открываем директорию-источник
	int dir_to_copy= open(argv[1], O_RDONLY);
	if (dir_to_copy < 0) {
		perror("Can't open current directory");
		return 3;
	}

	//Открываем директорию-приемник
	int dir_copy = open(argv[2], O_RDONLY);
	if (dir_copy < 0 ) {
		perror("Can't open copy directory");
		return 5;
	}
	
	DIR* dir = fdopendir(dir_to_copy);
	struct dirent* entry;

	//Начинаем копировать
	while ((entry = readdir(dir)) != NULL) {
		struct stat sb;
        	if(fstatat(dir_to_copy, entry->d_name, &sb, AT_SYMLINK_NOFOLLOW) < 0) printf("F to fstat");
        
		//Копируем регулярные файлы
		if ((sb.st_mode & S_IFMT) == S_IFREG) {
			
			int rf = openat(dir_to_copy, entry->d_name, O_RDONLY); //файл для чтения
			if (rf < 0) {
				perror("Cannot open file for reading");
				return 6;
			}
            		
            		int wf = openat(dir_copy, entry->d_name, O_WRONLY | O_CREAT | O_TRUNC, 0644); //файл для записи
            		if (wf < 0) {
                		perror("Failed to open file for writing");
				return 7;
			}
            
			//Переписываем блоками по 500
			char buffer[500];
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
		
			//Закрываем файлы
			if (close(rf)<0){
				perror("Failure write closing rf");
				return 8;
			}
			if (close(wf)<0){
				perror("Failure write closing wf");
				return 9;
			}
	
		}

		//Копируем директории
		if ((sb.st_mode & S_IFMT) == S_IFDIR) mkdirat(dir_copy, entry->d_name, sb.st_mode);
	}
	//Закрываем все остальное
	closedir(dir);
	close(dir_copy);
	close(dir_to_copy);

	return 0;
}
