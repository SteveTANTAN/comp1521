#include <stdio.h>
#include<stdlib.h>
#include <ctype.h>
// add  $d, $s, $t	d = s + t	    000000ssssstttttddddd00000100000
#define o_add                       0b00000011111111111111100000100000
// sub  $d, $s, $t	d = s - t	    000000ssssstttttddddd00000100010
#define o_sub                       0b00000011111111111111100000100010
// and  $d, $s, $t	d = s & t	    000000ssssstttttddddd00000100100
#define o_ando                      0b00000011111111111111100000100100
// or   $d, $s, $t	d = s | t	    000000ssssstttttddddd00000100101
#define o_or                        0b00000011111111111111100000100101
// slt  $d, $s, $t	d = 1 if s < t else 0	000000ssssstttttddddd00000101010
#define o_slt                       0b00000011111111111111100000101010
// mul  $d, $s, $t	d = s * t	    011100ssssstttttddddd00000000010
#define o_mul                       0b01110011111111111111100000000010
// beq  $s, $t, I	if (s == t)     PC += I	000100ssssstttttIIIIIIIIIIIIIIII
#define o_beq                       0b00010011111111111111111111111111
// bne  $s, $t, I	if (s != t)     PC += I	000101ssssstttttIIIIIIIIIIIIIIII
#define o_bne                       0b00010111111111111111111111111111
// addi $t, $s, I	t = s + I	    001000ssssstttttIIIIIIIIIIIIIIII
#define o_addi                      0b00100011111111111111111111111111

// slti $t, $s, I	t = (s < I)	    001010ssssstttttIIIIIIIIIIIIIIII
#define o_slti                      0b00101011111111111111111111111111

// andi $t, $s, I	t = s & I	    001100ssssstttttIIIIIIIIIIIIIIII
#define o_andi                      0b00110011111111111111111111111111

// ori  $t, $s, I	t = s | I	    001101ssssstttttIIIIIIIIIIIIIIII
#define o_ori                       0b00110111111111111111111111111111

// lui  $t, I	    t = I << 16	    00111100000tttttIIIIIIIIIIIIIIII
#define o_lui                       0b00111100000111111111111111111111

// syscall	        syscall	        00000000000000000000000000001100
#define o_sys                       0b00000000000000000000000000001100

#define error_out -1
#define number 1
#define type 1
#define character 2
#define max_lines 1000
/////////////////////////////////////////////////////////////////////////////
//
//                        MY OWEN FUCTION
//
//////////////////////////////////////////////////////////////////////////////

// the fuction using to runing the calculate program as background.
int calculate_background(__uint32_t line, int *turn, __uint32_t *comand,
__uint32_t *reg, int sum_turn, int *print_array,
int *type_array,int *print_num);

// the fuction using to print out all the commands.
void print_program(__uint32_t line, int turn);
int check_invalid(__uint32_t line);

int main(int argc, char *argv[]) {
    FILE *fp = fopen(argv[1], "r");

    if (fp == NULL) {
        perror(argv[1]);
        exit(1);
    }



    // grab the lines from the file
    // and make sure the total lines are less tan 1000
    __uint32_t comand[max_lines] = {'0'};
    int sum_turn = 0;
    while (fscanf(fp, "%x", &comand[sum_turn]) == 1
    && sum_turn < max_lines) {

        // make sure all the numbers garbing in are all valid.
        if(! check_invalid(comand[sum_turn])) {
            printf("%s:%d: invalid instruction code: %08X\n"
            ,argv[1], sum_turn + 1, comand[sum_turn]);
            exit(1);
        }
        sum_turn++;
    }

    fclose(fp);

    int turn = 0;
    //inisilize the registers
    __uint32_t reg[32];
    for (int i = 0; i < 32; i++){
        reg[i] = 0b00;
    }

    // print out all the comand lines
    printf("Program\n");
    while (turn < sum_turn){
        print_program(comand[turn], turn);
        turn++;
    }
    // create two arrays to recorded print infomation
    // print array using recording the words (character according to ASCII)
    // type array recording the types of each cell of print array
    int print_array[max_lines] = {'0'};
    int type_array[max_lines] = {'0'};
    int print_num = 0;
    turn = 0;
    // running the calculate_background fuction
    // to calculate all the registers in background
    while (turn < sum_turn){
        if (calculate_background(comand[turn], &turn,comand, reg, sum_turn,
        print_array, type_array, &print_num)) {
            turn++;
        } else {
            break;
        }
    }

    // print out for all the outputs
    printf("Output\n");
    for (int i = 0; i < print_num; i++){
        // print all the characters
        // if the matching type array is character symbol
        if (type_array[i] == character) {
            printf("%c", print_array[i]);
        }
        // print all the numbers
        // if the matching type array is numbers symbol
        if (type_array[i] == number) {

            printf("%d", print_array[i]);

        }
        // print all the errors
        // if the matching type array is error symbol
        if (type_array[i] == error_out) {
            printf("Unknown system call: %d\n", print_array[i]);
            if (i == 0) {
                for (int j = 0; j < 32; j++) {
                    reg[j] = 0;
                }
            }
            reg[2] = print_array[i];
            break;
        }
    }
    // print out the Registers After Execution
    printf("Registers After Execution\n");
    for (int i = 0; i < 32; i++){
        if(reg[i] != 0){
            printf("$%-3d= %d\n", i, reg[i]);
        }
    }
    return 0;
}
/////////////////////////////////////////////////////////////////////////////
//
//                        MY OWEN FUCTION
//
//////////////////////////////////////////////////////////////////////////////


// check if the number is valid by sparate first 6  numbers
// and last six numbers.
int check_invalid(__uint32_t line){
    __uint16_t first = (line >> 26) & 0b111111;
    __uint16_t last = line & 0b111111;

    if (first == 0) {
        if (last == 0b100000
        || last == 0b100010
        || last == 0b100100
        || last == 0b100101
        || last == 0b101010) {
            return 1;
        }
    }
    if (first == 0b011100 && last == 0b10) {
        return 1;
    }
    if(first == 0b000100
    || first == 0b000101
    || first == 0b001000
    || first == 0b001010
    || first == 0b001100
    || first == 0b001101
    || first == 0b001111
    || line == 0b1100
    ) {
        return 1;
    }
    return 0;
}

// using fuction to calculate all the registers in background
int calculate_background(__uint32_t line, int *turn,  __uint32_t *comand,
__uint32_t *reg,  int sum_turn, int *print_array,
int *type_array, int *print_num) {
    // if the line we grab is syscall
    if((line) == o_sys){
        // if $2 is 1
        // the symbol of print number
        // recording the number we need to print 
        // modify the type matching this number to number symbol.
        if(reg[2] == 1) {
            print_array[*print_num] = reg[4];
            type_array[*print_num] = number;
            (*print_num)++;
            return 1;
        }
        // if $2 is 11
        // the symbol of print character
        // recording the character we need to print 
        // modify the type matching this character to character symbol.
        if(reg[2] == 11) {
            print_array[*print_num] = reg[4];
            type_array[*print_num] = character;
            (*print_num)++;
            return 1;
        }
        // if $2 is 11
        // the symbol of jr ra
        // just stop the calculating fuction
        if(reg[2] == 10) {
            //exit(0);
            return 0;
        }
        // if $2 are none above
        // just record the print type into error symbol
        print_array[*print_num] = reg[2];
        type_array[*print_num] = error_out;
        (*print_num)++;
        return 1;
    }
    // if the line we grab is add
    if((line|o_add) == o_add) {
        // grab d, s, t from the it
        __uint8_t d  = (line>>11)  & 0b11111;
        __uint8_t t =((line>>16) & 0b11111);
        __uint8_t s =((line>>(16+5)) & 0b11111);
        // doing the addition
        reg[d] = reg[s] + reg[t];
        if (d == 0) {
            reg[d] = 0;
        }
        return 1;
    }
    // if the line we grab is sub
    if((line|o_sub) == o_sub){
        // grab d, s, t from the it
        __uint8_t d  = (line>>11)  & 0b11111;
        __uint8_t t =((line>>16) & 0b11111);
        __uint8_t s =((line>>(16+5)) & 0b11111);
        // doing the substution.
        reg[d] = reg[s] - reg[t];
        if (d == 0) {
            reg[d] = 0;
        }
        return 1;
    }
    // if the line we grab is and
    if((line|o_ando) == o_ando){
        // grab d, s, t from the it
        __uint8_t d  = (line>>11)  & 0b11111;
        __uint8_t t =((line>>16) & 0b11111);
        __uint8_t s =((line>>(16+5)) & 0b11111);
        // doing the '&' operation
        reg[d] = reg[s] & reg[t];
        if (d == 0) {
            reg[d] = 0;
        }
        return 1;
    }

    if((line|o_or) == o_or){
        __uint8_t d  = (line>>11)  & 0b11111;
        __uint8_t t =((line>>16) & 0b11111);
        __uint8_t s =((line>>(16+5)) & 0b11111);

        reg[d] = reg[s] | reg[t];
        if (d == 0) {
            reg[d] = 0;
        }
        return 1;

    }
    if((line|o_slt) == o_slt){
        __uint8_t d  = (line>>11)  & 0b11111;
        __uint8_t t =((line>>16) & 0b11111);
        __uint8_t s =((line>>(16+5)) & 0b11111);
        if (reg[s] < reg[t]){
            reg[d] = 1;
        } else {
            reg[d] = 0;
        }
        if (d == 0) {
            reg[d] = 0;
        }
        return 1;

    }
    if((line|o_mul) == o_mul){
        __uint8_t d  = (line>>11)  & 0b11111;
        __uint8_t t =((line>>16) & 0b11111);
        __uint8_t s =((line>>(16+5)) & 0b11111);

        reg[d] = reg[s] * reg[t];
        if (d == 0) {
            reg[d] = 0;
        }
        return 1;
    }
    if((line|o_beq) == o_beq){
        __int16_t I  = line & 0xFFFF;
        __uint8_t t =((line>>16) & 0b11111);
        __uint8_t s =((line>>(16+5)) & 0b11111);
        if(reg[t] == reg[s]) {
            *turn = (*turn) + I-1;
        }

        return 1;
    }
    if((line|o_bne) == o_bne){
        __int16_t I  = line & 0xFFFF;
        __uint8_t t =((line>>16) & 0b11111);
        __uint8_t s =((line>>(16+5)) & 0b11111);
        if(reg[t] != reg[s]) {
            *turn = (*turn) + I-1;
        }

        return 1;
    }
    if((line|o_addi) == o_addi){
        __int16_t I  = line & 0xFFFF;
        __uint8_t t =((line>>16) & 0b11111);
        __uint8_t s =((line>>(16+5)) & 0b11111);
        reg[t] = reg[s] + I;

        if (t == 0) {
            reg[t] = 0;
        }
        return 1;
    }
    if((line|o_slti) == o_slti){
        __int16_t I  = line & 0xFFFF;
        __uint8_t t =((line>>16) & 0b11111);
        __uint8_t s =((line>>(16+5)) & 0b11111);

        if (reg[s] < I) {
            reg[t] = 1;
        } else {
            reg[t] = 0;
        }

        if (t == 0) {
            reg[t] = 0;
        }
        return 1;

    }
    if((line|o_andi) == o_andi){
        __int16_t I  = line & 0xFFFF;
        __uint8_t t =((line>>16) & 0b11111);
        __uint8_t s =((line>>(16+5)) & 0b11111);
        reg[t] = (reg[s] & I);
        if (t == 0) {
            reg[t] = 0;
        }
        return 1;

    }
    if((line|o_ori) == o_ori){

        __int16_t I  = line & 0xFFFF;

        __uint8_t t =((line>>16) & 0b11111);
        __uint8_t s =((line>>(16+5)) & 0b11111);
        reg[t] = reg[s]|I;
        if (t == 0) {
            reg[t] = 0;
        }
        return 1;

    }
    if((line|o_lui) == o_lui){
        __int32_t I  = line & 0xFFFF;
        __uint32_t temp = 0;
        temp |= I;
        __uint8_t t =((line>>16) & 0b11111);
        reg[t] = temp << 16;

        if (t == 0) {
            reg[t] = 0;
        }
        return 1;
    }
    return 0;
}


void print_program(__uint32_t line, int turn){
     if((line) == o_sys){
        printf("%3d: syscall\n", turn);


        return;
    }
    else if((line|o_add) == o_add) {
        __uint8_t d  = (line>>11)  & 0b11111;
        __uint8_t t =((line>>16) & 0b11111);
        __uint8_t s =((line>>(16+5)) & 0b11111);
        printf("%3d: add  $%d, $%d, $%d\n",turn, d, s, t);

    }
    else if((line|o_sub) == o_sub){
        __uint8_t d  = (line>>11)  & 0b11111;
        __uint8_t t =((line>>16) & 0b11111);
        __uint8_t s =((line>>(16+5)) & 0b11111);
        printf("%3d: sub  $%d, $%d, $%d\n",turn, d, s, t);

    }
    else if((line|o_ando) == o_ando){
        __uint8_t d  = (line>>11)  & 0b11111;
        __uint8_t t =((line>>16) & 0b11111);
        __uint8_t s =((line>>(16+5)) & 0b11111);

        printf("%3d: and  $%d, $%d, $%d\n",turn, d, s, t);

    }
    else if((line|o_or) == o_or){
        __uint8_t d  = (line>>11)  & 0b11111;
        __uint8_t t =((line>>16) & 0b11111);
        __uint8_t s =((line>>(16+5)) & 0b11111);
        printf("%3d: or   $%d, $%d, $%d\n",turn, d, s, t);


    }
    else if((line|o_slt) == o_slt){
        __uint8_t d  = (line>>11)  & 0b11111;
        __uint8_t t = ((line>>16) & 0b11111);
        __uint8_t s = ((line>>(16+5)) & 0b11111);

        printf("%3d: slt  $%d, $%d, $%d\n",turn, d, s, t);
        return;

    }
    else if((line|o_mul) == o_mul){
        __uint8_t d  = (line>>11)  & 0b11111;
        __uint8_t t =((line>>16) & 0b11111);
        __uint8_t s =((line>>(16+5)) & 0b11111);
        printf("%3d: mul  $%d, $%d, $%d\n",turn, d, s, t);



    }
    else if((line|o_beq) == o_beq){
        __int16_t I  = line & 0xFFFF;
        __uint8_t t =((line>>16) & 0b11111);
        __uint8_t s =((line>>(16+5)) & 0b11111);
        printf("%3d: beq  $%d, $%d, %d\n",turn, s, t, I);

    }
    else if((line|o_bne) == o_bne){
        __int16_t I  = line & 0xFFFF;
        __uint8_t t =((line>>16) & 0b11111);
        __uint8_t s =((line>>(16+5)) & 0b11111);
        printf("%3d: bne  $%d, $%d, %d\n",turn, s, t, I);
        return;
    }
    else if((line|o_addi) == o_addi){

        __int16_t I  = line & 0xFFFF;
        __uint8_t t =((line>>16) & 0b11111);
        __uint8_t s =((line>>(16+5)) & 0b11111);
        printf("%3d: addi $%d, $%d, %d\n",turn, s, t, I);

        return;

    }
    else if((line|o_slti) == o_slti){
        __int16_t I  = line & 0xFFFF;
        __uint8_t t =((line>>16) & 0b11111);
        __uint8_t s =((line>>(16+5)) & 0b11111);
        printf("%3d: slti $%d, $%d, %d\n",turn, t, s, I);
        return;
    }
    else if((line|o_andi) == o_andi){
        __int16_t I  = line & 0xFFFF;
        __uint8_t t =((line>>16) & 0b11111);
        __uint8_t s =((line>>(16+5)) & 0b11111);
        printf("%3d: andi $%d, $%d, %d\n",turn, t, s, I);
        return;
    }
    else if((line|o_ori) == o_ori){

        __int16_t I  = line & 0xFFFF;
        __uint8_t t =((line>>16) & 0b11111);
        __uint8_t s =((line>>(16+5)) & 0b11111);
        printf("%3d: ori  $%d, $%d, %d\n",turn, t, s, I);
        return;
    }
    else if((line|o_lui) == o_lui){
        __int16_t I  = line & 0xFFFF;

        __uint8_t t =((line>>16) & 0b11111);
        printf("%3d: lui  $%d, %d\n",turn, t, I);
    }
    return;
}