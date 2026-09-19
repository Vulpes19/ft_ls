#include "structs.h"
#include <dirent.h>
#include <pwd.h>
#include <errno.h>
#include <string.h>
#include "printer.h"
#include "utils.h"

#define USR_READ 0400
#define USR_WRITE 0200
#define USR_EXECUTE 0100

#define GRP_READ 0040
#define GRP_WRITE 0040
#define GRP_EXECUTE 0010

#define OTH_READ 0004
#define OTH_WRITE 0002
#define OTH_EXECUTE 0001

int  count_digits(int st_size) {
    if (st_size == 0)
        return 1;

    int count = 0;
    while (st_size != 0)
    {
        st_size = st_size / 10;
        ++count;
    }
    return count;
}

void copy_entries(t_entry **new_entries, t_entry_data *data, int size) {
    int i = 0;

    while (i < size) {
        new_entries[i] = data->entries[i];
        i++;
    }
}

size_t    store_entries(const char *directory, t_entry_data *data, t_flags *flags, int *max_size_width, int *max_nlink_width) {
    size_t size = 0;
    size_t capacity = 5;
    DIR *dir = opendir(directory);
    struct dirent *entry;
    struct stat     statbuf;

    data->entries = (t_entry **)ft_calloc(capacity, sizeof(t_entry *));
    if (!data->entries) {
        perror(ft_strjoin("Failed to allocate memory for t_entry ** in: ", strerror(errno)));
        exit(EXIT_FAILURE);
    }

    if (dir) {
        while ((entry = readdir(dir)) != NULL) {

            // modify path with slash so it can be opened with lstat()
            char *temp_path = ft_strjoin(directory, "/");
            char *full_path = ft_strjoin(temp_path, entry->d_name);
            free(temp_path);

            // stat() returns info about the file passed to it
            // lstat() is the same but it also returns info about the symlink
            if (lstat(full_path, &statbuf) == -1) {
                free_d_ptr(data->entries, size);
                free(data);
                handle_error(__LINE__, __FILE__, __FUNCTION__, "Failed to get stat");
                continue;
            }

            // -l flag needs to count:
            // total number
            // maximum width of the size column (so the output looks clean)
            // maximum width of the nlink width (so the output looks clean)
            if (flags->l_flag) {
                data->total += (!flags->a_flag && entry->d_name[0] == '.' ? 0 : statbuf.st_blocks);
                int len = count_digits(statbuf.st_size);
                int len_nlink = count_digits(statbuf.st_nlink);
                if (len > *max_size_width)
                    *max_size_width = len;
                if (len_nlink > *max_nlink_width)
                    *max_nlink_width = len_nlink;
            }

            // if size reaches capacity, reallocation happens
            if (size == capacity) {
                capacity *= 2;
                t_entry **new_entries = (t_entry **)ft_calloc(capacity, sizeof(t_entry *));
                if (!new_entries) {
                    free_d_ptr(data->entries, size);
                    free(data);
                    handle_error(__LINE__, __FILE__, __FUNCTION__, "Failed to allocate memory for t_entry ** : ");
                }
                copy_entries(new_entries, data, size);
                free(data->entries);
                data->entries = new_entries;
            }
            data->entries[size] = (t_entry *)malloc(sizeof(t_entry));
            if (!data->entries[size]) {
                free_d_ptr(data->entries, size);
                free(data);
                handle_error(__LINE__, __FILE__, __FUNCTION__, "Failed to allocate memory for t_entry *: ");
            }

            // Info we get for each file/dir/entry
            
            data->entries[size]->name = strdup(entry->d_name); // name of entry
            data->entries[size]->type = entry->d_type;         // type of entry
            data->entries[size]->full_path = strdup(full_path);// full path of entry
            free(full_path);
            if (!data->entries[size]->name || !data->entries[size]->full_path) {
                free_d_ptr(data->entries, size);
                free(data);
                handle_error(__LINE__, __FILE__, __FUNCTION__, "Failed to allocate memory for char *: ");
            }
            data->entries[size]->stat = statbuf; // stats of entry
            size++;
        }
        closedir(dir);
        data->size = size;
    }
    else {
        ft_putstr_fd("ft_ls: ", 2);
        perror(directory);
        exit(1);
    }

    return data->size;
}

void parse_flags(t_flags *flags, char *argument) {
    int i = 1;
    int len = ft_strlen(argument);

    while (i < len) {
        char flag = argument[i];

        switch (flag)
        {
            case 'l':
                flags->l_flag = true;
                break;
            case 'a':
                flags->a_flag = true;
                break;
            case 'R':
                flags->R_flag = true;
                break;
            case 'r':
                flags->r_flag = true;
                break;
            case 't':
                flags->t_flag = true;
                ft_putstr_fd("ft_ls: illegal option -- ", 2);
                ft_putchar_fd(flag, 2);
                ft_putstr_fd("\nusage: ft_ls [-1aRlrt] [file ...]\n", 2);
                break;
        }
        
        i += 1;
    }
}