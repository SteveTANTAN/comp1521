#include <stdio.h>
#include<stdlib.h>
#include <ctype.h>
int main(int argc, char *argv[]) {
    
    FILE *fp = fopen(argv[1], "w");
    
    if (fp == NULL) {
        perror(argv[1]);
    }

    
    size_t temp = 2;
    while (temp <argc) {
        int c = atoi(argv[temp]);
        if(c<=255 && c >= 0) {
            fputc(c,fp);
        }
        
        
        temp++;
    }

    fclose(fp);
    return 0;
}