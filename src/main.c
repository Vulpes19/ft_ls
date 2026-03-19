#include <stdio.h>
#include <stdbool.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "structs.h"
#include <pwd.h>
#include "parser.h"

// void    parse_arg(char **target_dir, char *argument) {
//     if (opendir(argument) != NULL)

// }

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
        // else
        //     parse_arg(&target_dir, av[i]);
        i++;
    }
    store_entries(&ch);

    // printf("%d %d %d %d %d\n", (int)ch.a_flag, (int)ch.l_flag, (int)ch.R_flag, (int)ch.r_flag, (int)ch.t_flag);
    return (0);
}