#include <stdio.h>
#include<stdlib.h>
#include <ctype.h>
int main(int argc, char *argv[]) {
    
    FILE *fp = fopen(argv[1], "r");
    
    if (fp == NULL) {
        perror(argv[1]);
    }

    int c;
    size_t temp = 0;
    while ((c = fgetc(fp)) != EOF) {
        
        printf("byte %4ld: %3d 0x%02x", temp, c, c);
        if (isprint(c)) {
            printf(" '%c'", c);
        }
        printf("\n");
        temp++;
    
    }

    fclose(fp);
    return 0;
}