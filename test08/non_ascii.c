#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[]) {
    FILE *fp =fopen(argv[1], "r");
    if (fp == NULL) {
        perror(argv[1]);
        
    }
    ssize_t p = 0;
    int byte = 0;
    while((byte = fgetc(fp)) != EOF) {
        if(byte>127){
            printf("%s: byte %zd is non-ASCII\n", argv[1], p);
            return 0;
        }
        p ++;
    }

    printf("%s is all ASCII\n",argv[1]);
    fclose(fp);

    return 0;
}