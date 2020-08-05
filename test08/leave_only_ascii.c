#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(int argc, char *argv[]) {
    FILE *fp =fopen(argv[1], "r");
    if (fp == NULL) {
        perror(argv[1]);
        
    }
    char *buffer = "tmp";
    char temporary_pathname[1000];
    strcpy(temporary_pathname, argv[1]);
    strcat(temporary_pathname, buffer);
    FILE *out = fopen(temporary_pathname, "w");
    if (out == NULL) {
    perror(argv[1]);
    exit(1);
    }
    int byte;
    while ((byte = fgetc(fp)) != EOF) {
        if (byte <= 127) {
            fputc(byte, out);
        }
    }
    fclose(fp);
    fclose(out);
    fp =fopen(argv[1], "w");
    out = fopen(temporary_pathname, "r");
    
    while ((byte = fgetc(out)) != EOF) {
        if (byte <= 127) {
            fputc(byte, fp);
        }
    }
    fclose(fp);
    fclose(out);
    

    
    return 0;
}
