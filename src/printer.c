#include "parser.h"
#include <string.h>


void    set_permissions(unsigned int type, mode_t st_mode, char *permissions) {
    switch (type)
    {
        case DT_REG: // Regular file
            permissions[0] = '-';
            break;
        case DT_DIR: // Directory
            permissions[0] = 'd';
            break;
        case DT_LNK: // Symbolic link
            permissions[0] = 'l';
            break;
        case DT_CHR: // Character device
            permissions[0] = 'c';
            break;
        case DT_BLK: // Block device
            permissions[0] = 'b';
            break;
        case DT_SOCK: // Local domain socket
            permissions[0] = 's';
            break;
        case DT_FIFO: // Named PIPE FIFO
            permissions[0] = 'p';
            break;
        default:
            permissions[0] = '-';
            break;
    }
    permissions[1] = (st_mode & S_IRUSR) ? 'r' : '-'; // 0400
    permissions[2] = (st_mode & S_IWUSR) ? 'w' : '-'; // 0200
    permissions[3] = '-';
    if (st_mode & S_ISUID) 
        permissions[3] = (st_mode & S_IXUSR) ? 's' : 'S'; // 04000
    else
        permissions[3] = (st_mode & S_IXUSR) ? 'x' : '-'; // 0100

    permissions[4] = (st_mode & S_IRGRP) ? 'r' : '-'; // 0040
    permissions[5] = (st_mode & S_IWGRP) ? 'w' : '-'; // 0020
    permissions[6] = '-';
    if (st_mode & S_ISGID)
        permissions[6] = (st_mode & S_IXGRP) ? 's' : 'S'; // 02000
        else
        permissions[6] = (st_mode & S_IXGRP) ? 'x' : '-'; // 0010

    permissions[7] = (st_mode & S_IROTH) ? 'r' : '-'; // 0004
    permissions[8] = (st_mode & S_IWOTH) ? 'w' : '-'; // 0002
    permissions[9] = '-';
    // Sticky bit directory, users can create files according to normal permissions
    // but they can't delete/edit files owned by other users (Unless they own the dir or are root)
    if (st_mode & S_ISVTX)
        permissions[9] = (st_mode & S_IXOTH) ? 't' : 'T'; // 01000
    else
        permissions[9] = (st_mode & S_IXOTH) ? 'x' : '-'; // 0001
    permissions[10] = '\0';
}

void    get_date(time_t mtime, char *month, char *day, char *time, time_t current_time) {
    char *full_date = ctime(&mtime);
    const long six_months = 15552000;
    time_t file_age = current_time - mtime;
    size_t date_len = 25;

    ft_strlcpy(month, &full_date[4], 4);
    ft_strlcpy(day, &full_date[8], 3);
    if (file_age > six_months)
        ft_strlcpy(time, &full_date[19], 6);
    else
        ft_strlcpy(time, &full_date[11], 6);

}

void    print_l_flag(t_entry *entry, time_t current_time, int max_size_width) {
    char permissions[11];
    char month[4];
    char day[3];
    char time[6];
    char *filename = entry->name;
    set_permissions(entry->type, entry->stat.st_mode, permissions);
    
    struct passwd *pwuid = getpwuid(entry->stat.st_uid);
    struct group *gr = getgrgid(entry->stat.st_gid);
    get_date(entry->stat.st_mtime, month, day, time, current_time);

    if (S_ISLNK(entry->stat.st_mode)) {
        char buf[PATH_MAX];

        buf[0] = ' ';
        ssize_t len = readlink(entry->full_path, buf + 1, sizeof(buf) - 1);
        if (len == -1) {
            handle_error(__LINE__, __FILE__, __FUNCTION__, "Failed to read symlink target file");
            exit(EXIT_FAILURE);
        }
        buf[len + 1] = '\0';
        char *temp = ft_strjoin(entry->name, " ->");
        filename = ft_strjoin(temp, buf);
        free(temp);
    }
    printf("%s  %d %s  %s %*lld %s %s %s %s\n", permissions, entry->stat.st_nlink, pwuid->pw_name, gr->gr_name, max_size_width, entry->stat.st_size, month, day, time , filename); 
}

void    print_output(t_flags *flags, char *directory, t_entry_data *data) {
    if (flags->dir_nbr > 1) {
        printf("%s:\n", directory);
    }
    if (flags->l_flag)
        printf("total %zu\n", data->total);
    time_t current_time = time(NULL);
    for (size_t i = 0; i < data->size && data->entries[i]; i++) {
        if (!flags->a_flag && data->entries[i]->name[0] == '.')
            continue;
        if (flags->l_flag)
            print_l_flag(data->entries[i], current_time, data->max_size_width);
        else
            printf("%s  ", data->entries[i]->name);
    }
    printf("\n");
}