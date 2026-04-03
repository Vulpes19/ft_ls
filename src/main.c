#include "ls.h"

int main(int ac, char **av)
{
    int i = 1;
    t_flags flags = {
        false,
        false,
        false,
        false,
        false,
        0
    };

    while (i < ac && av[i][0] == '-') {
        ft_printf("%s\n", av[i]);
        parse_flags(&flags, av[i]);
        i++;
    }
    if (i == ac)
        ls_dir(&flags, ".");
    else
        flags.dir_nbr = ac - i;
    while (i < ac) {
        ls_dir(&flags, av[i]);
        i++;
    }
    
    return (0);
}