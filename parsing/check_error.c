#include "../mini_shell.h"

int	check_pipe(t_token *token, t_token *next_token)
{
	if (token->type == PIPE && next_token->type == PIPE)
		return (1);
	else
		return (0);
}

int	check_redirect(t_token *token, t_token *next_token)
{
	if (token->type == RED_IN || token->type == RED_OUT
		|| token->type == HER_DOC || token->type == APPEND)
	{
		if (next_token->type == RED_IN || next_token->type == RED_OUT
			|| next_token->type == HER_DOC || next_token->type == APPEND)
			return (1);
		return (0);
	}
	else
		return (0);
}

int	ft_check_error(t_token *token)
{
	if (token->type == PIPE)
		return (printf("syntax error near unexpected token '%s'\n"
					, token->content), 1);
	while (token->next)
	{
		if (check_pipe(token, token->next) != 0
			|| check_redirect(token, token->next) != 0)
			return (printf("syntax error near unexpected token '%s'\n"
					, token->next->content), 1);
		token = token->next;
	}
	if (token->type != WORD)
		return (printf("syntax error near unexpected token 'newline'\n"), 1);
	return (0);
}
