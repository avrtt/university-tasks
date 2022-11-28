#include <signal.h>
#include <wait.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>

int fd[2], PID1, PID2, PID_CUR, p_number, ret_code, w;
char str_cur[40];

void start(int s)
{
	printf("Process P1 (%d) received the signal\n", getpid());
}

int main()
{
	if (pipe(fd) == 0)
		printf("K1 was created\n");
	else
	{
		exit(1);
		printf("Error: Unable to create a channel!\n");
	}
	
    signal(SIGUSR1, start);
	
	PID1 = fork();
	
	if (PID1 == 0)
    {
		printf("P1 was created\n");
		pause();
		PID_CUR = getpid();
		char str_cur_1[] = "Hello from P1";
		write(fd[1], &PID_CUR, sizeof(int));
		write(fd[1], &str_cur_1, sizeof(str_cur_1));
		printf("Data sent from P1 to K1\n");
		exit(0);
    }
	
	if (PID1 == -1)
	{
		exit(1);
		printf("Error: Unable to create P1!\n");
	}
	
	PID2 = fork();
	
	if (PID2 == 0)
	{ 
		printf("P2 was created\n");
		PID_CUR = getpid();
		char str_cur_2[] = "Hello from P2"; 
		write(fd[1], &PID_CUR, sizeof(int));
		write(fd[1], &str_cur_2, sizeof(str_cur_2));
		kill(PID1, SIGUSR1);
		printf("Data sent from P2 to K1\n");
		printf("A signal is sent from P2 to P1\n");
		exit(0);
    }
	
	if (PID2 == -1)
	{
		exit(1);
		printf("Error: Unable to create P2!\n");
	}
		
	if (PID1 && PID2)
	{			
		read(fd[0], &p_number, sizeof(int));
		read(fd[0], &str_cur, sizeof(str_cur));
		printf("%s (%d)\n", str_cur, p_number);
		wait(&ret_code);
		if (WIFEXITED(ret_code) != 0)
			printf("P2 is finished\n");
			
		read(fd[0], &p_number, sizeof(int));
		read(fd[0], &str_cur, sizeof(str_cur));
		printf("%s (%d)\n", str_cur, p_number);	
		wait(&ret_code);
		if (WIFEXITED(ret_code) != 0)
			printf("P1 is finished\n");
	}	
	
	printf("Parent process finished\n");
	return 0;
}
