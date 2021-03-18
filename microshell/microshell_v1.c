/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   microshell_v1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fflores <fflores@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/17 17:20:44 by fflores           #+#    #+#             */
/*   Updated: 2021/03/18 13:55:22 by fflores          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "microshell_v1.h"


void type_prompt()
{
	static int first_time = 1;
	if (first_time)
	{
		char *CLEAR_SCREEN_ANSI = " \e[1;1H\e[2J"; // move cursor up (1;1) and then clean the screen
		write(STDOUT_FILENO, CLEAR_SCREEN_ANSI, 12);
		first_time = 0;
	}
	write(1, "$", 1);
}

void read_command ( char *command, char **parameters)
{
	char line[1024];
	int count = 0;
	int i = 0;
	int j = 0;
	char *array[100];
	char *buf;
	char *pch;
	
	while (1)
	{
		int c = fgetc (stdin);
		line[count++] = (char)c;
		if (c == '\n')
			break;
	}
	if (count == 1)
		return ;
	pch = strtok(line, " \n");	// ' ' or '\n'
	while (pch != NULL)			// get words (command and parameters) from the input line
	{
		array[i++] = strdup(pch);
		pch = strtok(NULL, " \n");
	}
	strcpy(command, array[0]); //command
	while (j < i)
	{
		parameters[j] = array[j];		// copying parameters
		j++;
	}
	parameters[j] = NULL; // terminating parameters array with NULL
}


int main(int argc, char **argv, char **envp)
{
	char cmd[100];
	char command[100];
	char *parameters[20];
	while (1)
	{
		type_prompt();
		read_command( command, parameters );
		if (fork() != 0)
			wait(NULL);
		else
		{
			strcpy( cmd, "/bin/" );
			strcat( cmd, command );
			execve( cmd, parameters, envp );
		}
		if ( strcmp( command, "exit" ) == 0)
			break;
	}
	return (0);
}