#include "../../mini_shell.h"

int check_format(char *arg)
{
    int cur;
    int has_equal;

    if (!ft_isalpha(arg[0]))
         return (print_error(1, &arg[0]));
    cur = 0;
    has_equal = 0;
    while (arg[cur])
    {
        if (arg[cur] == '=')
            has_equal = 1;
        else if (arg[cur] == '+' && arg[cur+1] == '=')
            has_equal = 1;
        else if (!ft_isalnum(arg[cur]) && arg[cur] != '_')
            if (!has_equal)
                return (print_error(2, &arg[cur]));
        cur++;
    }
    if (!has_equal)
        return (1);
    return (0);
}

int print_error(int error_code, char *content)
{
    if (error_code == 1) {
        printf("\033[91m%s '%c'\033[0m\n", "export: name cannot start with", *content);
    } else if (error_code == 2) {
        printf("\033[91m%s '%c'\033[0m\n", "export: not valid in this context:", *content);
    }
    return (1);
}
