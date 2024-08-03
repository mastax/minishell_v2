#include "../mini_shell.h"

char	*ft_substr(char *s, unsigned int start, size_t len)
{
	size_t	i;
	char	*str;
	size_t	slen;

	if (!s)
		return (NULL);
	slen = ft_strlen(s);
	str = malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	i = 0;
	while (start < slen && s[start + i] != '\0' && i < len)
	{
		str[i] = s[start + i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

int	is_spc_opr(char c)
{
	if (c == '|' || c == '>' || c == '<')
		return (1);
	return (0);
}

int	ft_skipe_spaces(char *line, int i)
{
	while ((line[i] == ' ' || line[i] == '\t') && line[i] != '\0')
		i++;
	return (i);
}

int	ft_skipe_qoute(char	*s, int i)
{
	if ((s[i] == '"' && s[i + 1] == '"') || (s[i] == '\''
			&& s[i + 1] == '\''))
		i += 1;
	else if (s[i] == '"')
	{
		i++;
		while (s[i] != '"' && s[i] != '\0')
			i++;
	}
	else if (s[i] == '\'')
	{
		i++;
		while (s[i] != '\'' && s[i] != '\0')
			i++;
	}
	if (s[i + 1] != ' ' && s[i + 1] != '\t' && s[i + 1] != '\0'
		&& s[i + 1] != '|' && s[i + 1] != '>' && s[i + 1] != '<')
	{
		if (s[i + 1] == '"' || s[i + 1] == '\'')
			i = ft_skipe_qoute(s, i + 1);
		while (s[i + 1] != ' ' && s[i + 1] != '\t' && s[i + 1] != '\0'
			&& s[i + 1] != '|' && s[i + 1] != '>' && s[i + 1] != '<')
			i++;
	}
	return (i);
}