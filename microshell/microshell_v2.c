#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>


void    type_prompt()
{
	static int first_time = 1;

	if (first_time == 1)
	{
		char *ANSI_MOVE_CURSOR_ESCAPE_CODE = "\e[1;1H";
		char *ANSI_CLEAR_SCREEN_ESCAPE_CODE = "\e[2J";
		write(1, ANSI_MOVE_CURSOR_ESCAPE_CODE, 6);
		write(1, ANSI_CLEAR_SCREEN_ESCAPE_CODE, 4);
		first_time = 0;
	}
	write(1, "$", 1);
}

void take_input(char *command, char *parameters)
{
	int i;
	char c;

	i = 0;
	while (read(1, &c, 1))
	{
		if (c == '\n')
		{
			command[i] = '\0';
			break;
		}
		command[i++] = c;
	}
}

void print_input(char *command, char *parameters)
{
	write(1, command, strlen(command));
	write(1, "\n", 1);
}


int main(int argc, char **argv, char **envp)
{
	int *status;
	char command[100];
	char parameters[100];

	while (1)
	{
		type_prompt();
		take_input(command, parameters);
		print_input(command, parameters);
		// if (fork() != 0)
		// 	waitpid(-1, status, 0);
		// else
		// 	execve("/bin/ls", argv, envp);
	}
	
}