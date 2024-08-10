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

int ft_skipe_qoute(char *s, int i)
{
    if (s[i] == '\0')
		return (i);
    if ((s[i] == '"' && s[i + 1] == '"') || (s[i] == '\'' && s[i + 1] == '\''))
        return (i + 1);
    char quote = (s[i] == '"' || s[i] == '\'') ? s[i] : '\0';
    if (quote)
    {
        i++;
        while (s[i] != '\0' && s[i] != quote)
            i++;
        if (s[i] == quote)
			i++;
    }
    while (s[i] != '\0')
    {
        if (s[i] == ' ' || s[i] == '\t' || s[i] == '|' || s[i] == '>' || s[i] == '<')
            break;
        if (s[i] == '"' || s[i] == '\'')
            i = ft_skipe_qoute(s, i);
        else
            i++;
    }
    return (i);
}
