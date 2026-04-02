#pragma once

#include "structs.h"
#include <dirent.h>
#include <pwd.h>

size_t    store_entries(const char *directory, t_entry_data *data); // stores entries (directory names, file names etc) in a struct
void    parse_flags(t_flags *flags, char *argument); // parses the flags (-l, -a, -R etc)
void    copy_entries(t_entry **new_entries, t_entry **entries, int size);