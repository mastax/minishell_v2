#ifndef LIBFT_H
#define LIBFT_H


# include <stdio.h>
# include <unistd.h>
# include <fcntl.h>
# include <stdlib.h>
# include <limits.h>
# include <string.h>



void    *ft_memset(void *b, int c, size_t len);
void    *ft_calloc(size_t count, size_t size);
char    *ft_strchr(const char *s, int c);
char	*ft_substr(char *s, unsigned int start, size_t len);
int     ft_strncmp(char *s1, const char *s2, size_t n);
int     ft_strcmp(const char *s1, const char *s2);
void    ft_putendl_fd(char *s, int fd);
void	ft_putstr_fd(char *s, int fd);
size_t  ft_strlen(const char *s);
int     is_number(int c);
int     ft_atoi(const char *str);
size_t  ft_strlcat(char *dst, const char *src, size_t dstsize);
size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize);
char	*ft_strjoin(char *s1, char *s2);
void	*ft_memset(void *b, int c, size_t len);
void	*ft_memcpy(void *dst, const void *src, size_t n);
char	*ft_strdup(char *src);
char	*ft_strndup(char *str, int n);

char	**ft_split(char const *s, char c);//
int     ft_isalnum(int c);
int     ft_isalpha(int c);
int		ft_is_space(int k);
void	ft_putnbr_fd(int n, int fd);
void	ft_putchar_fd(char c, int fd);
char    *ft_strtok(char *str, const char *delim);
char    *ft_strcpy(char *s1, const char *s2);
char	*ft_itoa(int n);
char	*ft_concat(char *s1, char *s2);

#endif