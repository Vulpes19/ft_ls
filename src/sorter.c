#include "sorter.h"

enum sort_order {
    S_REVERSE, //Reverse sort order. Works with the current sort key (name, time, size, etc)
    S_DATE, // Sort by modification time (newest first)
    S_DATE_REVERSE,
    S_DEFAULT
};

bool    compare(t_entry *left, t_entry *right, enum sort_order order) {
    int res = strcoll(left->name, right->name);
    if (res == 0)
        res = strcmp(left->name, right->name);
    switch (order)
    {
        case S_REVERSE:
            return (res > 0);

        case S_DATE:
            if (left->stat.st_mtime != right->stat.st_mtime)
                return (left->stat.st_mtime > right->stat.st_mtime);
            return (res < 0);

        case S_DATE_REVERSE:
            if (left->stat.st_mtime != right->stat.st_mtime)
                return (left->stat.st_mtime < right->stat.st_mtime);
            return (res > 0);

        default:
            return (res < 0);
    }
} 

void    merge(t_entry_data *data, int left, int middle, int right, enum sort_order order) {
    int i, j, k;
    int n1 = middle - left + 1;
    int n2 = right - middle;

    t_entry *left_arr[n1], *right_arr[n2];

    for (i = 0; i < n1; i++) {
        left_arr[i] = data->entries[left + i];
    }
    for (j = 0; j < n2; j++) {
        right_arr[j] = data->entries[middle + 1 + j];
    }

    i = 0;
    j = 0;
    k = left;
    while (i < n1 && j < n2) {
        if (compare(left_arr[i], right_arr[j], order)) {
            data->entries[k] = left_arr[i];
            i++;
        }
        else {
            data->entries[k] = right_arr[j];
            j++;
        }
        k++;
    }
    while (i < n1) {
        data->entries[k] = left_arr[i];
        i++;
        k++;
    }

    while (j < n2) {
        data->entries[k] = right_arr[j];
        j++;
        k++;
    }
}

void    sort(t_entry_data *data, int left, int right, enum sort_order order) {
    if (left < right) {
        int middle = left + (right - left) / 2;

        sort(data, left, middle, order);
        sort(data, middle + 1, right, order);

        merge(data, left, middle, right, order);
    }
}

void    sort_entries(t_flags *flags, t_entry_data *data) {
    if (flags->r_flag && flags->t_flag) {
        sort(data, 0, data->size - 1, S_DATE_REVERSE);
    }
    else if (flags->r_flag) {
        sort(data, 0, data->size - 1, S_REVERSE);
    }
    else if (flags->t_flag) {
        sort(data, 0, data->size - 1, S_DATE);
    }
    else
        sort(data, 0, data->size - 1, S_DEFAULT);
}