//На вход программы подается путь до директории и имя директории, она создает папку копию с таким же путем, припиской _copy после имени и копией всех регулярных файлов из копируемой директории
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
		fprintf(stderr, "Usage: %s <path> <directory-name>\n", argv[0]);
		return 1;
	}

	//Открываем общую директорию, в которой лежит копируемая директория
	int dir_parent = open(argv[1], O_RDONLY, O_DIRECTORY);
	if (!dir_parent) {
		perror("Can't open parent directory");
		return 2;
	}
    
    
	//Открываем копируеммую директорию
	int dir_to_copy= openat(dir_parent, argv[2], O_RDONLY);
	if (dir_to_copy < 0) {
		perror("Can't open current directory");
		return 3;
	}

	struct stat sb_dir;
	if (fstatat(dir_parent, argv[2], &sb_dir, AT_SYMLINK_NOFOLLOW) < 0) printf("F to fstat");

    	//Создаем копию
	char copyname[512];
	snprintf(copyname, sizeof copyname, "%s_copy", argv[2]);
	if (mkdirat(dir_parent, copyname, sb_dir.st_mode) < 0) {
		perror("Failed to create directory");
		return 4;
	}


	//Открываем копию
	int dir_copy = openat(dir_parent, copyname, O_RDONLY);
	if (dir_copy < 0 ) {
		perror("Can't open copy directory");
		return 5;
	}
	
	DIR* dir = fdopendir(dir_to_copy);
	struct dirent* entry;

	//Начинаем копировать по файлам
	while ((entry = readdir(dir)) != NULL) {
		struct stat sb;
        	if(fstatat(dir_to_copy, entry->d_name, &sb, AT_SYMLINK_NOFOLLOW) < 0) printf("F to fstat");
        
		//Копируем только регулярные файлы
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
            
			//Копируем по 950
			char buffer[1000];
			size_t i = 950;
			while(i == 950){
				i = read(rf, buffer, 950);
				if (i<0) {
					printf("Read Error");
					return 4;
				}
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
	}
	//Закрываем все остальное
	closedir(dir);
	close(dir_copy);
	close(dir_to_copy);
	close(dir_parent);

	return 0;
}
