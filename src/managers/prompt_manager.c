/*
** EPITECH PROJECT, 2018
** PSU_minishell1_2017
** File description:
** prompt_manager functions
*/

# include "minishell.h"

char *parse_cwd(char *buffer, shell_t *shell)
{
	char *dup = my_strdup(buffer);
	char *home = env_get_variable("HOME", shell);
	int loop = 0;
	char *str = NULL;

	if (my_strstartswith(dup, home)) {
		str = my_strconfigure(1 + (my_strlen(buffer) - my_strlen(home)));
		if (str == NULL)
			return (NULL);
		str[0] = '~';
		while (*buffer) {
			if (loop++ >= my_strlen(home)) {
				str[(loop - my_strlen(home))] = *buffer;
			}
			buffer++;
		}
		free(dup);
	}
	return ((str == NULL ? dup : str));
}

void display_prompt(shell_t *shell)
{
	char buffer[4097];
	char *cwd = getcwd(buffer, 4096);
	char *pcwd = parse_cwd(cwd, shell);

	my_putstr("\033[1m");
	my_putstr(pcwd);
	free(pcwd);
	my_putstr("\033[0m");
	my_putstr(" \033[31m︻\033[0m\033[32m┳\033[0m\033[33mデ");
	my_putstr("\033[0m\033[34m═\033[0m\033[35m—\033[0m$ ");
}
