#ifndef MINI_SHELL_H
# define MINI_SHELL_H

#define MAX_ARGS 100
#define DELIMITERS " \t\r\n\a"
#define STDIN_FILENO 0
#define STDOUT_FILENO 1
#define BUFFER_SIZE 1024

# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>

# include "./include/struct.h"
# include "./include/libft.h"

// Function prototypes
t_env *create_env(char **envp);
void free_env(t_env *env);
char *get_env_value(t_env *env, const char *key);
int set_env_value(t_env *env, const char *key, const char *value);
int unset_env_value(t_env *env, const char *key);

int execute_builtin(t_arg *cmd, t_env *env, int *exit_status);
int execute_external_command(char **argv, char **envp);
int execute_command(t_arg *cmd, t_env *env, int *exit_status);
int main_shell_loop(t_env *env);
int ft_env(t_env *env);

int ft_change_dir(char **av, t_env *env);
int change_to_home(t_env *env);
int change_to_previous(t_env *env);
                    /*PIP - HELPER FUNCTIONS*/

char **split_pipeline(char *line);
void redirect_io(int input_fd, int output_fd);
int run_pipeline_command(char *command, t_env *env, int input_fd, int output_fd);
int handle_pipeline(char **commands, t_env *env);
                    /*UNSET - HELPER FUNCTIONS*/


                    /*EXPORT - HELPER FUNCTIONS*/

int     check_format(char *arg);
int     print_error(int error_code, char *content);
void    print_sorted_env(t_env *env);

                    /*exit BUiLTIN*/

int     ft_is_arg_nbr(char *ar);
                    /*THE BUILTIN*/

int		ft_echo(char **av);
int		ft_exit(char **av, int status);
int     pwd(int fd);
int ft_exports(t_env *env, char **args, int *exit_status);
int ft_unsets(t_env *env, char **args, int *exit_status);
                    /*THE_EXECUTOR*/

void    free_argv(char **av);
char    **split_line(char *line);
int     is_builtin(const char *cmd);

                    /*THE EXECUTOR HELPER*/
char    **split_line(char *line);

/*free the list*/
void free_command_list(t_arg *cmd_list);

/*parsing*/

int		is_spc_opr(char c);

// //
int 	is_valid_var(char c);
t_token	*ft_list_new(char *token);
char	*ft_compress_spaces(char *s);
int		ft_check_qoutes(char	*line);
int	ft_skipe_qoute(char	*s, int i);
// int		ft_skipe_qoute(char	*s, int i);
int		ft_check_error(t_token *token);
void	ft_putstr_fd(char *str, int fd);
char	*ft_remove_quotes(char *s);
// int		expanden(t_token **token, char **env);
int	expanden(t_token **token, t_env *env);
int	get_token(t_token **token, char	*s);
int		ft_skipe_spaces(char *s, int index);
char    *ft_remove_char(char *s, unsigned int index);
void	ft_lstadd_back(t_token **lst, t_token *new);

// t_arg *parsing(char *line, char **env);
int	parsing(char *line, t_token	**token, t_env *env);
t_arg	*ft_arg_new(int fd);
void	ft_argadd_back(t_arg **lst, t_arg *new);
t_arg	*ft_arglast(t_arg	*lst);

int is_pipe_token(t_arg *arg);
t_arg *get_next_arg(t_arg *arg);
char *get_arg_content(t_arg *arg);
void free_arg_content(t_arg *arg);
// t_arg *convert_tokens_to_args(t_token *tokens);
int ft_convert_token_to_arg(t_token *token, t_arg *args, int stat);

/*Redirections functions*/

int apply_redirections(char **red);

/*HERDOC*/

int count_heredocs(char **red);
int *handle_heredocs(char **red, int count, t_env *env);

int	ft_expand_variable(char **var, t_env *env, t_type prv_type, int i);//expending

#endif