#include "ls.h"

void    ls_subdir(char *path) {

}

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

    if (flags->R_flag)
        ls_subdir(path);
}