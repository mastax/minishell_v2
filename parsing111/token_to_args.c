#include "../mini_shell.h"

int	count_args_red(t_token *token, int type)
{
	int		i;
	int		j;
	t_token	*tmp;

	i = 0;
	j = 0;
	tmp = token;
	while (tmp)
	{
		if (tmp->type == WORD)
			i++;
		else if (tmp->type == HER_DOC || tmp->type == APPEND
			|| tmp->type == RED_IN || tmp->type == RED_OUT)
		{
			j += 2;
			tmp = tmp->next;
		}
		else if (tmp->type == PIPE)
			break ;
		tmp = tmp->next;
	}
	if (type == 1)
		return (i);
	else if (type == 2)
		return (j);
	return (0);
}

char	**ft_handl_args(char *content, char **arv, int type)
{
	int	i;

	i = 0;
	if (type == 1)
	{
		arv[0] = ft_strdup(content);
		arv[1] = NULL;
	}
	else if (type == 2)
	{
		while (arv[i] != NULL)
			i++;
		arv[i] = ft_strdup(content);
		arv[i + 1] = NULL;
	}
	return (arv);
}

int	ft_handle_convert(t_token *token, t_arg *args, int type)
{
	int		i;

	if (args->arg == NULL && type == 1)
	{
		i = count_args_red(token, 1);
		if ((args->arg = malloc(sizeof(char *) * (i + 1))) == NULL)
			return (ft_putstr_fd("Error: can't malloc for new_arg\n", 2), -1);
		args->arg = ft_handl_args(token->content, args->arg, 1);
	}
	else if (args->arg != NULL && type == 1)
		args->arg = ft_handl_args(token->content, args->arg, 2);
	if (args->red == NULL && type == 2)
	{
		i = count_args_red(token, 2);
		if ((args->red = malloc(sizeof(char *) * (i + 1))) == NULL)
			return (ft_putstr_fd("Error: can't malloc for new_arg\n", 2), -1);
		args->red = ft_handl_args(token->content, args->red, 1);
		args->red = ft_handl_args(token->next->content, args->red, 2);
	}
	else if (args->red != NULL && type == 2)
	{
		args->red = ft_handl_args(token->content, args->red, 2);
		args->red = ft_handl_args(token->next->content, args->red, 2);
	}
	return (0);
}

int	ft_convert_token_to_arg(t_token *token, t_arg *args, int stat)
{
	t_arg	*tmp_arg;
	t_token	*tmp_token;

	tmp_arg = args;
	tmp_token = token;
	while (tmp_token)
	{
		if (tmp_token->type == WORD)
			stat = ft_handle_convert(tmp_token, tmp_arg, 1);
		else if (tmp_token->type == HER_DOC || tmp_token->type == APPEND
		|| tmp_token->type == RED_IN || tmp_token->type == RED_OUT)
		{
			stat = ft_handle_convert(tmp_token, tmp_arg, 2);
			tmp_token = tmp_token->next;
		}
		else if (tmp_token->type == PIPE)
		{
			ft_argadd_back(&args, ft_arg_new(2));
			tmp_arg = ft_arglast(args);
		}
		if (!tmp_arg || stat == -1)
			return (-1);
		tmp_token = tmp_token->next;
	}
	return (0);
}

void	ft_prin_arg_red(t_arg **arg)
{
	int	i;
	int	j;
	t_arg *tmp = *arg;

	while (tmp)
	{
		i = 0;
		j = 0;
		printf("New argument set:\n");
		if (tmp->arg != NULL)
		{
			while (tmp->arg[i] != NULL)
			{
				printf("arg : %s i=[%d]\n", tmp->arg[i], i);
				i++;
			}
		}
		if (tmp->red != NULL)
		{
			while (tmp->red[j] != NULL)
			{
				printf("red : %s j=[%d]\n", tmp->red[j], j);
				j++;
			}
		}
		tmp = tmp->next;
		printf("\n");
	}
}
