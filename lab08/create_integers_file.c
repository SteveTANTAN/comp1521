#include <stdio.h>
#include<stdlib.h>
int main(int argc, char *argv[]) {
    if (argc != 4) {
        exit(1);
    }

    int first = atoi(argv[2]);
    int last = atoi(argv[3]);
    FILE *fb = fopen(argv[1],"w");
    if (fb == NULL) {
        perror(argv[1]);
    }
    int temp = first;

    while (temp <= last) {
        fprintf(fb, "%d\n", temp);
        temp++;
    }


    fclose(fb);
    return 0;
}