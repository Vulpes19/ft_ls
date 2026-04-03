#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <locale.h>
#include <langinfo.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>

char* concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
    // in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

int main(void) {
    struct dirent  *dp;
    struct stat     statbuf;
    DIR *dir = opendir("src");
    struct passwd  *pwd;
    struct group   *grp;
    struct tm      *tm;
    char            datestring[256];

    /* Loop through directory entries. */
    while ((dp = readdir(dir)) != NULL) {
        if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)
            continue;
        /* Get entry's information. */
        if (stat(concat("src/", dp->d_name), &statbuf) == -1)
            continue;


        /* Print out type, permissions, and number of links. */
        if (statbuf.st_mode)
            printf("%hu", statbuf.st_mode);
        printf("%4d", statbuf.st_nlink);


        /* Print out owner's name if it is found using getpwuid(). */
        if ((pwd = getpwuid(statbuf.st_uid)) != NULL)
            printf(" %-8.8s", pwd->pw_name);
        else
            printf(" %-8d", statbuf.st_uid);


        /* Print out group name if it is found using getgrgid(). */
        if ((grp = getgrgid(statbuf.st_gid)) != NULL)
            printf(" %-8.8s", grp->gr_name);
        else
            printf(" %-8d", statbuf.st_gid);


        /* Print size of file. */
        printf(" %9jd", (intmax_t)statbuf.st_size);


        tm = localtime(&statbuf.st_mtime);


        /* Get localized date string. */
        strftime(datestring, sizeof(datestring), nl_langinfo(D_T_FMT), tm);


        printf(" %s %s\n", datestring, dp->d_name);
    }
    return 0;
}