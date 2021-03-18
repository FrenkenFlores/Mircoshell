/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   microshell_v2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fflores <fflores@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/18 16:40:47 by fflores           #+#    #+#             */
/*   Updated: 2021/03/18 16:50:19 by fflores          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
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
		write(1, "\e[3m\e[32mFFLORES\n", strlen("\e[3m\e[32mFFLORES\n"));
		write(1, "\e[0m", strlen("\e[0m"));
		first_time = 0;
	}
	write(1, "$", 1);
}

void take_input(char *command, char **parameters)
{
	int i;
	int j;
	char c;

	i = 0;
	j = 0;
	while (read(1, &c, 1))
	{
		if (c == '\n')
		{
			command[i] = '\0';
			break;
		}
		command[i++] = c;
	}
	i = 0;
	while (command[i] != ' ' && command[i] != '\0')
		i++;
	if (command[i] == ' ')
		command[i++] = '\0';
	if (command[i] != '\0')
		parameters[j++] = &command[0];
	parameters[j++] = &command[i];
	while (command[i] != '\0')
	{
		if (command[i] == ' ')
		{
			command[i++] = '\0';
			parameters[j++] = &command[i];
		}
		i++;
	}
	parameters[j] = NULL;
}

void print_input(char *command, char **parameters)
{
	int j = 0;

	write(1, command, strlen(command));
	write(1, "\n", 1);
	while (parameters[j] != NULL)
	{
		write(1, parameters[j], strlen(parameters[j]));
		write(1, "\n", 1);
		j++;
	}
}

char *concatenate_bin(char *command)
{
	int i;
	int j;
	char *s = "/bin/";
	char *ptr = (char *)malloc(sizeof(char) * (strlen(command) + strlen(s) + 1));

	i = -1;
	j = -1;
	while (s[++i] != '\0')
		ptr[i] = s[i];
	while (command[++j] != '\0')
		ptr[i++] = command[j];
	ptr[i] = '\0';
	return ptr;
}

int main(int argc, char **argv, char **envp)
{
	int *status;
	char command[100];
	char *parameters[24];
	char *bash_command;
	int exec_ret;
	int fork_ret;

	while (1)
	{
		type_prompt();
		take_input(command, parameters);
		if (strcmp(command, "exit") == 0)
			exit(0);
		bash_command = concatenate_bin(command);
		// print_input(bash_command, parameters);
		if ((fork_ret = fork()) != 0)
			waitpid(-1, status, 0);
		else
			exec_ret = execve(bash_command, parameters, envp);
		if (exec_ret == -1)
		{
			write(1, "\e[31mCommand not found\n", strlen("\e[31mcommand not found\n"));
			exec_ret = 0;
			write(1, "\e[0m", strlen("\e[0m"));
			break ;
		}
	}
	return (0);
}