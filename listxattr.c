#include <sys/xattr.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void print_xattrs(const char *path) {
    ssize_t n = listxattr(path, NULL, 0, 0); // options = 0
    if (n < 0) { perror("listxattr"); return; }
    if (n == 0) { printf("No xattrs\n"); return; }

    char *names = malloc(n);
    if (!names) { perror("malloc"); return; }

    n = listxattr(path, names, n, 0);
    if (n < 0) { perror("listxattr 2"); free(names); return; }

    for (ssize_t i = 0; i < n; ) {
    const char *name = names + i;
    printf("attr: %s\n", name);

    ssize_t m = getxattr(path, name, NULL, 0, 0, 0); // position=0, options=0
    if (m >= 0) {
    unsigned char *val = malloc(m);
    if (val && m > 0) {
    ssize_t r = getxattr(path, name, val, m, 0, 0);
    if (r >= 0) {
    printf(" value (%zd bytes):", r);
    for (ssize_t k = 0; k < r; k++) printf(" %02x", val[k]);
    printf("\n");
    } else perror("getxattr value");
    }
    free(val);
    } else {
    perror("getxattr size");
    }

    i += strlen(name) + 1; // names are NUL-separated
    }
    free(names);
}

int main(int argc, char **argv) {
    const char *p = (argc > 1) ? argv[1] : "somefile";
    print_xattrs(p);
    return 0;
}