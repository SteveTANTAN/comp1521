#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
off_t file_size(char *pathname) {
    struct stat s;
    if (stat(pathname, &s) != 0) {
        perror(pathname);
        exit(1);
    }
    printf("%s: %ld bytes\n", pathname, (long)s.st_size);
    return s.st_size;
}
int main(int argc, char *argv[]) {
    off_t total_bytes = 0;
    for (int arg = 1; arg < argc; arg++) {
        total_bytes += file_size(argv[arg]);
    }
    printf("Total: %ld bytes\n", total_bytes);
    return 0;
}
