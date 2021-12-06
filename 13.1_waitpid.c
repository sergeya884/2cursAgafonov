// Программа создает child процесс, выводит информацию о нем и о parent, и работает пока процесс не завершится, выводя статус
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
	pid_t cpid, w;
	int wstatus;

        // создаем child процесс
	switch(cpid=fork()) {
		case -1:
			perror("fork"); 
			exit(1); 
		case 0:
			printf("\nChild\n");
			printf("PID %d,\n", getpid());
			printf("PPID %d,\n", getppid());
			printf("PGID %d,\n", getpgid(0));
			printf("SID %d\n\n", getsid(0));
			if (argc == 1) { //Можно отправить в argv[1] сигнал к завершению
				pause();
				_exit(atoi(argv[1]));
			}
		default:
			printf("\nParent\n");
			printf("PID %d,\n", getpid());
			printf("PPID %d,\n", getppid());
			printf("PGID %d,\n", getpgid(0));
			printf("SID %d\n\n", getsid(0));
        		do { //Пока child процесс не завершится успешно или по сигналу
				w = waitpid(cpid, &wstatus, WUNTRACED | WCONTINUED);
				if (w == -1) {
					perror("waitpid");
					exit(EXIT_FAILURE);
				}

				if (WIFEXITED(wstatus)) printf("the child process has been completed\nstatus = %d\n", WEXITSTATUS(wstatus));
				else if (WIFSIGNALED(wstatus)) printf("the child process has been killed by signal %d\n", WTERMSIG(wstatus));
				else if (WIFSTOPPED(wstatus)) printf("the child process has been stopped by signal %d\n", WSTOPSIG(wstatus));
				else if (WIFCONTINUED(wstatus)) printf("continued\n");

			} while (!WIFEXITED(wstatus) && !WIFSIGNALED(wstatus));

        		#ifdef WCOREDUMP
			if (WCOREDUMP(wstatus)) printf("child proc has cause a core dump\n");
        		#endif 

			exit(2);
	}
}
