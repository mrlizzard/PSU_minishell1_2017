/*
** EPITECH PROJECT, 2018
** minishell
** File description:
** commands manager functions
*/

# include "my.h"
# include "minishell.h"

int executable(char *bin, stat_t info, char **c, param_t *p)
{
	if (info.st_mode & S_IXUSR) {
		if (info.st_mode & S_IXUSR) {
			return (run_command(bin, c, p));
		} else {
			my_printf("Error: permission denied for '%s'.\n", bin);
		}

		free(bin);
		return (1);
	}

	free(bin);
	return (0);
}

int check_binaries(char **command, param_t *param)
{
	int i = 0;
	char *bin_path;
	char **path = my_strtok(env_get_var("PATH", param->env), ':');
	stat_t info;

	exit(0);

	while (path && path[i]) {
		if (my_str_startswith(command[0], path[i]))
			bin_path = my_strdup(command[0]);
		else
			bin_path = my_pathjoin(path[i], command[0]);

		printf("bin_path = %s\n", bin_path);

		if (lstat(bin_path, &info) == -1) {
			free(bin_path);
		} else {
			my_free_array(path);
			return (executable(bin_path, info, command, param));
		}
		i++;
	}

	my_free_array(path);
	return (0);
}

int check_command(char *command, param_t *param)
{
	int ret = 0;

	if (my_strcmp(command, "exit") == 0) {
		ret = exit_command(command, param);
	} else if (my_strcmp(command, "env") == 0) {
		ret = env_command(command, param);
	}

	return (ret);
}

int run_command(char *path, char **args, param_t *param)
{
	pid_t pid = fork();
	char **arr = my_strtok(path, ' ');

	signal(SIGINT, proc_signal_handler);

	if (pid == 0) {
		printf("%s args = {", path);

		for (int i = 0; arr[i]; i++)
			printf("%s,", arr[i]);
		printf("}\n");

		execve(path, arr, param->env);
	} else if (pid < 0) {
		free(path);
		my_free_array(arr);
		write(2, "Fork failed to create a new process.\n", 38);
		return (-1);
	}

	wait(&pid);

	if (path != NULL)
		free(path);

	my_free_array(arr);
	my_free_array(args);

	return (1);
}

int exec_command(char **command, param_t *param)
{
	stat_t info;
	int own = check_command(command[0], param);

	printf("%s, %d\n", command[0], own);

	if (own == 1 || check_binaries(command, param))
		return (0);
	if (own < 0)
		return (-1);

	printf("%s\n", command[0]);

	if (lstat(command[0], &info) != -1) {
		if (info.st_mode & S_IFDIR) {
			//change_dir(command[0], 0);
			return (0);
		} else if (info.st_mode & S_IXUSR) {
			return (run_command(my_strdup(command[0]), command, param));
		}
	}

	my_printf("Error: unknown command '%s'.\n", command[0]);
	return (0);
}