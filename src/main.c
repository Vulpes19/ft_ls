#include <stdio.h>
#include <stdbool.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "structs.h"
#include <pwd.h>
#include "parser.h"
#include "sorter.h"
#include "printer.h"

void    ls_dir(t_flags *flags, char *path) {

    t_entry_data *data = NULL;
    size_t size = 0;

    data = (t_entry_data *)malloc(sizeof(t_entry_data));
    if (!data) {
        handle_error(__LINE__, __FILE__, __FUNCTION__, "Failed to allocate memory for t_entry_data * : ");
    }
    size = store_entries(path, data);
    sort_entries(flags, data);
    print_output(flags, path, data);

    free_d_ptr(data->entries, size);
    free(data);

    // if (flags->R_flag)
    //     ls_subdir(path);
}

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