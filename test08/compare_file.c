#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[]) {
    FILE *fp1 =fopen(argv[1], "r");
    if (fp1 == NULL) {
        perror(argv[1]);
        
    }
    FILE *fp2 =fopen(argv[2], "r");
    if (fp2 == NULL) {
        perror(argv[2]);
        
    }
    int p = 0;
    int b1 = 0;
    int b2 = 0;
    while(1) {
        b1 = fgetc(fp1);
        b2 = fgetc(fp2);
        if(b1 == EOF || b2 == EOF || b1 != b2){
            break;
        }
        p ++;
    }

    if(b1 == b2) {
    printf("Files are identical\n");

    } else if(b1 == EOF) {
    printf("EOF on %s\n", argv[1]);

    } else if(b2 == EOF) {
    printf("EOF on %s\n", argv[2]);


    } else {
    printf("Files differ at byte %d\n", p);
    }
   
   
    fclose(fp1);
    fclose(fp2);
    return 0;
}