#include <stdio.h>
#include <stdbool.h>
#include "../Libft/ft_printf.h"
#include "../Libft/libft.h"

typedef struct {
    bool l_flag;
    bool R_flag;
    bool a_flag;
    bool r_flag;
    bool t_flag;
} checker;

void parse_flags(checker *ch, char *argument) {
    int i = 1;
    int len = ft_strlen(argument);

    while (i < len) {
        char flag = argument[i];

        switch (flag)
        {
            case 'l':
                ch->l_flag = true;
                break;
            case 'a':
                ch->a_flag = true;
                break;
            case 'R':
                ch->R_flag = true;
                break;
            case 'r':
                ch->r_flag = true;
                break;
            case 't':
                ch->t_flag = true;
                break;
            default:
                break;
        }
        
        i += 1;
    }
}

int main(int ac, char **av)
{
    int i = 0;
    checker ch = {
        false,
        false,
        false,
        false,
        false
    };

    while (i < ac) {
        ft_printf("%s\n", av[i]);
        if (av[i][0] == '-')
            parse_flags(&ch, av[i]);
        i++;
    }

    printf("%d %d %d %d %d\n", (int)ch.a_flag, (int)ch.l_flag, (int)ch.R_flag, (int)ch.r_flag, (int)ch.t_flag);
    // printf("outside: %d\n outside: %d outside: %d\n outside: %d\n outside: %d\n", (int)ch.a_flag, (int)ch.l_flag, (int)ch.R_flag, (int)ch.r_flag, (int)ch.t_flag);
    return (0);
}