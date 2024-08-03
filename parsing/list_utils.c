#include "../mini_shell.h"

t_type ft_get_type(char *token)
{
	if (token[0] == '|' && token[1] == '\0')
		return (PIPE);
	else if (token[0] == '>' && token[1] == '>' && token[2] == '\0')
		return (APPEND);
	else if (token[0] == '<' && token[1] == '<' && token[2] == '\0')
		return (HER_DOC);
	else if (token[0] == '>' && token[1] == '\0')
		return (RED_OUT);
	else if (token[0] == '<' && token[1] == '\0')
		return (RED_IN);
	else
		return (WORD);
}

t_token	*ft_lstlast(t_token	*lst)
{
	if (!lst)
		return (NULL);
	while (lst->next != NULL)
	{
		lst = lst->next;
	}
	return (lst);
}

void	ft_lstadd_back(t_token **lst, t_token *new)
{
	t_token	*last;

	if (!lst || !new)
		return ;
	last = ft_lstlast(*lst);
	if (last != NULL)
		last->next = new;
	else
		*lst = new;
}

t_token	*ft_list_new(char *token)
{
	t_token	*data;

	data = malloc(sizeof(t_token));
	if (!data)
		return (write(2 , "Error\n can't malloc for new_data\n", 34), NULL);
	data->content = token;
	data->type = ft_get_type(token);
	data->next = NULL;
	return (data);
}