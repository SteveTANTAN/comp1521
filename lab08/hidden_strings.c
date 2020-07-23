#include <stdio.h>
#include<stdlib.h>
#include <ctype.h>
int main(int argc, char *argv[]) {
    
    FILE *fp = fopen(argv[1], "r");
    
    if (fp == NULL) {
        perror(argv[1]);
    }

    int c;
    int buf[4];
    size_t temp = 0;
    while ((c = fgetc(fp)) != EOF) {
        if(isprint(c)){
            if (temp <= 3) {
                buf[temp] = c;
                if(temp == 3) {
                    for (int i =0; i<=3; i++)
                        putchar(buf[i]);
                }
            } else {
                putchar(c);
            }
            temp++;
            
        } else{
            if(temp >= 4) {
                putchar('\n');
            }
            
            temp = 0;
        }    
    }

    if (temp > 4) 
        putchar('\n');
    fclose(fp);
    return 0;
}