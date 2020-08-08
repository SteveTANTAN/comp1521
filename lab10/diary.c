#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(int argc, char *argv[]) {
    char *parents_name = getenv("HOME");
    if (parents_name == NULL) {
        parents_name = ".";
    }
    char *file_name = ".diary";
    int length = strlen(parents_name) + strlen(file_name) + 2;
    char dia_name[length];
    snprintf(dia_name, sizeof dia_name, "%s/%s", parents_name, file_name);
    FILE *stream = fopen(dia_name, "a");
    if (stream == NULL) {
        perror(dia_name);
        exit(1);
    }
    for (int temp = 1; temp < argc; temp++) {
        fprintf(stream, "%s", argv[temp]);
        if (temp < argc - 1) {
            fprintf(stream, " ");
        }
    }
    fprintf(stream, "\n");
    fclose(stream);
    return 0;
}