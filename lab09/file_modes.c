#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
void file_mode(char *name) {
    struct stat s;
    if (stat(name, &s) != 0) {
        perror(name);
        exit(1);
    }
    mode_t temp = s.st_mode;
    char permissions[] = "?rwxrwxrwx";
    int n_permissions = strlen(permissions);
    if (S_ISREG(temp)) {
        permissions[0] = '-';
    } else if (S_ISDIR(temp)) {
        permissions[0] = 'd';
    }
    for (int i = 1; i < n_permissions; i++) {
        if (!(temp & (1 << (i - 1)))) {
         
            permissions[n_permissions - i] = '-';
        }
    }
    printf("%s %s\n", permissions, name);
}
int main(int argc, char *argv[]) {
    for (int arg = 1; arg < argc; arg++) {
        file_mode(argv[arg]);
    }
    return 0;
}

