#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int g_exit_code = 0;

int ft_strlen(const char *str)
{
	int i;

	i = 0;
	while (str[i] != '\0')
		i++;
	return (i);
}


int get_next_cmd(char ***cmd, char *end_tok, int start_pos, char **av)
{
    int i = 0;

    av = av + start_pos;
    while (av[i] && strcmp(av[i], ";") && strcmp(av[i], "|"))
		i++;
    *end_tok = (av[i] ? *av[i] : '\0');
    av[i] = 0;
    *cmd = av;
    return (i);
}

void connect_to_pps(int *prev_pipe, int *next_pipe)
{
	if(prev_pipe[0] != -1)
	{
		dup2(prev_pipe[0], STDIN_FILENO);
		close(prev_pipe[0]);
		close(prev_pipe[1]);
	}
	if(next_pipe[1] != -1)
	{
		dup2(next_pipe[1], STDOUT_FILENO);
		close(next_pipe[0]);
		close(next_pipe[1]);
	}
}

int ft_cd(char **cmd)
{
	int i;
	int ret;

	i = 0;
	while (cmd[i])
		i++;
	if (i != 2)
	{
		write(2, "error: cd: bad arguments\n", ft_strlen("error: cd: bad arguments\n"));
		return  (1);
	}
	if ((ret = chdir(cmd[1])) < 0)
	{
		write(2, "cannot change directory to ", ft_strlen("cannot change directory to "));
		write(2, cmd[1], ft_strlen(cmd[1]));
		write(2, "\n", 1);
		return (ret);
	}
	return (ret);
}



int main(int ac, char **av, char **env)
{
    char **cmd;
    char end_tok;
    int prev_pipe[2];
    int next_pipe[2];
    int cmd_pos;
    int res;

    (void)ac;
    end_tok = ';';
    cmd_pos = 1;
    next_pipe[0] = -1;
    next_pipe[1] = -1;

    while (end_tok != '\0' || (end_tok == ';' && av[cmd_pos] != 0))
    {
        cmd_pos = cmd_pos + get_next_cmd(&cmd, &end_tok, cmd_pos, av) + 1;
        if (!cmd[0])
			continue;

        prev_pipe[0] = next_pipe[0];
        prev_pipe[1] = next_pipe[1];
        if (end_tok == '|')
        	pipe(next_pipe);
        else
		{
        	next_pipe[0] = -1;
        	next_pipe[1] = -1;
		}
        close(prev_pipe[1]);
        if (!(strcmp(cmd[0], "cd")))
        	g_exit_code = ft_cd(cmd);
        else
		{
        	if (fork() == 0)
			{
				connect_to_pps(prev_pipe, next_pipe);
				res = execve(cmd[0], cmd, env);
				write(2, "error: cannot execute ", ft_strlen("error: cannot execute "));
				write(2, cmd[0], ft_strlen(cmd[0]));
				write(2, "\n", 1);
				exit(res);
			}
        	waitpid(-1, &res, 0);
        	if (WIFEXITED(res))
        		g_exit_code = WEXITSTATUS(res);
		}
        close(prev_pipe[0]);
    }
    return (g_exit_code);
}