#include "parser.h"
#include <string.h>

void    print_l_flag(t_entry *entry) {
    // ft_printf()
    (void)entry;
    switch (entry->type)
    {
    case DT_REG: // Regular file
        ft_printf("-");
        break;
    case DT_DIR: // Directory
        ft_printf("d");
        break;
    case DT_LNK: // Symbolic link
        ft_printf("l");
        break;
    case DT_CHR: // Character device
        ft_printf("c");
        break;
    case DT_BLK: // Block device
        ft_printf("b");
        break;
    case DT_SOCK: // Local domain socket
        ft_printf("s");
        break;
    case DT_FIFO: // Named PIPE FIFO
        ft_printf("p");
        break;
    default:
        break;
    }
    char read = 
    ft_printf("-")
    // getpwuid -> returns user
    // getgrgid -> returns group
    // listxattr -> 
    // getxattr -> 
    // time -> 
    // ctime ->
    // readlink ->
}

void    print_output(t_flags *flags, char *directory, t_entry_data *data) {
    if (flags->dir_nbr > 1) {
        ft_printf("%s:\n", directory);
    }
    for (size_t i = 0; i < data->size && data->entries[i]; i++) {
        if (!flags->a_flag && data->entries[i]->name[0] == '.')
            continue;
        if (flags->l_flag)
            print_l_flag(data->entries[i]);
        ft_printf("%s  ", data->entries[i]->name);
    }
    ft_printf("\n");
}