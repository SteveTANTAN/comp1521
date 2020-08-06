#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
// z5237560
// XINGYU(Steve) TAN
// COMP1521 ASSIGNMENT2
// FINALLY FINSHIED VERSION  ||  06/08/2020
#define add                       0b100000
#define sub                       0b100010
#define ando                      0b100100
#define or                        0b100101
#define slt                       0b101010
#define mul_1                     0b011100
#define mul_2                     0b000010
#define beq                       0b000100
#define bne                       0b000101
#define addi                      0b001000
#define slti                      0b001010
#define andi                      0b001100
#define ori                       0b001101
#define lui                       0b001111
#define sys                       0b001100


#define error_out -1
#define number 1
#define type 1
#define character 2
#define max_lines 1000
#define max_registers 32
/////////////////////////////////////////////////////////////////////////////
//
//                        MY OWEN FUCTION
//
//////////////////////////////////////////////////////////////////////////////

// the fuction using to runing the calculate program as background.
int calculate_background(__uint32_t line, int *turn, __uint32_t *reg,
int *print_array, int *type_array,int *print_num);

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
    // and make sure the total lines are less than 1000
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
    __uint32_t reg[max_registers];
    for (int i = 0; i < max_registers; i++){
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
        if (calculate_background(comand[turn], &turn, reg,
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
    // decode all the lines from file,
    // and check what kind of operation they are.
    __uint16_t first = (line >> 26) & 0b111111;
    __uint16_t last = line & 0b111111;
    // if it is add, sub, and, or, slt operation, return true.
    if (first == 0) {
        if (last == add || last == sub || last == ando
        || last == or || last == slt) {
            return 1;
        }
    }
    // if it is mul operation, return ture.
    if (first == mul_1 && last == mul_2) {
        return 1;
    }
    // if it is beq, bne, addi, slti, andi, ori, lui, sys operation, return ture.
    if(first == beq || first == bne || first == addi || first == slti
    || first == andi || first == ori || first == lui || line == sys) {
        return 1;
    }
    // if none above, just return false.
    return 0;
}

// using fuction to calculate all the registers in background
int calculate_background(__uint32_t line, int *turn, __uint32_t *reg,
int *print_array, int *type_array,int *print_num) {
    // decode all the lines from file,
    // and check what kind of operation they are.
    __uint8_t d = (line >> 11)  & 0b11111;
    __uint8_t t = (line >> 16) & 0b11111;
    __uint8_t s = (line >> 21) & 0b11111;
    __int16_t I = line & 0xFFFF;
    __uint16_t first = (line >> 26) & 0b111111;
    __uint16_t last = line & 0b111111;
    // if the line we grab is add
    if(first == 0 && last == add) {
        // doing the addition
        // add $d, $s, $t	d = s + t
        reg[d] = reg[s] + reg[t];

    } else if(first == 0 && last == sub){
        // doing the substution.
        // sub $d, $s, $t	d = s - t
        reg[d] = reg[s] - reg[t];

    } else if(first == 0 && last == ando){
        // doing the '&' operation
        // and $d, $s, $t	d = s & t
        reg[d] = reg[s] & reg[t];

    } else if(first == 0 && last == or){
        // doing the or operation
        // or $d, $s, $t	d = s | t
        reg[d] = reg[s] | reg[t];

    } else if(first == 0 && last == slt){
        // doing the slt operation
        // slt $d, $s, $t	d = 1 if s < t else 0
        if (reg[s] < reg[t]){
            reg[d] = 1;
        } else {
            reg[d] = 0;
        }

    } else if(first == mul_1 && last == mul_2){
        // doing the mul operation
        // mul $d, $s, $t	d = s * t
        reg[d] = reg[s] * reg[t];

    } else if(first == beq){
        // doing the beq operation
        // beq $s, $t, I	if (s == t) PC += I
        if(reg[t] == reg[s]) {
            *turn = (*turn) + I-1;
        }

    } else if(first == bne){
        // doing the bne operation
        // bne $s, $t, I	if (s != t) PC += I
        if(reg[t] != reg[s]) {
            *turn = (*turn) + I-1;
        }

    } else if(first == addi){
        // doing the addi operation
        // addi $t, $s, I	t = s + I
        reg[t] = reg[s] + I;

    } else if(first == slti){
        // doing the slti operation
        // slti $t, $s, I	t = (s < I)
        if (reg[s] < I) {
            reg[t] = 1;
        } else {
            reg[t] = 0;
        }

    } else if(first == andi){
        // doing the andi operation
        // andi $t, $s, I	t = s & I
        reg[t] = (reg[s] & I);

    } else if(first == ori){
        // doing the ori operation
        // ori $t, $s, I	t = s | I
        reg[t] = reg[s]|I;

    } else if(first == lui){
        // doing the lui operation
        // lui $t, I	t = I << 16
        __int32_t I_temp  = line & 0xFFFF;
        __uint32_t temp = 0;
        temp |= I_temp;
        reg[t] = temp << 16;

    } else if((line) == sys){
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
    // if $0 has been changed, modify it back
    // as $0 has to be always "0".
    if (d == 0) {
        reg[d] = 0;
    }
    return 1;
}

// this fuction is going to print out all the command
// before doing the calculate and only print them once.
void print_program(__uint32_t line, int turn){
    // decode all the lines from file,
    // and check what kind of operation they are.
    __uint8_t d = (line >> 11)  & 0b11111;
    __uint8_t t = (line >> 16) & 0b11111;
    __uint8_t s = (line >> 21) & 0b11111;
    __int16_t I = line & 0xFFFF;
    __uint16_t first = (line >> 26) & 0b111111;
    __uint16_t last = line & 0b111111;

    if(first == 0 && last == add) {
        printf("%3d: add  $%d, $%d, $%d\n",turn, d, s, t);

    } else if(first == 0 && last == sub){
        printf("%3d: sub  $%d, $%d, $%d\n",turn, d, s, t);

    } else if(first == 0 && last == ando){
        printf("%3d: and  $%d, $%d, $%d\n",turn, d, s, t);

    } else if(first == 0 && last == or){
        printf("%3d: or   $%d, $%d, $%d\n",turn, d, s, t);

    } else if(first == 0 && last == slt){
        printf("%3d: slt  $%d, $%d, $%d\n",turn, d, s, t);

    } else if(first == mul_1 && last == mul_2){
        printf("%3d: mul  $%d, $%d, $%d\n",turn, d, s, t);

    } else if(first == beq){
        printf("%3d: beq  $%d, $%d, %d\n",turn, s, t, I);

    } else if(first == bne){
        printf("%3d: bne  $%d, $%d, %d\n",turn, s, t, I);

    } else if(first == addi){
        printf("%3d: addi $%d, $%d, %d\n",turn, s, t, I);

    } else if(first == slti){
        printf("%3d: slti $%d, $%d, %d\n",turn, t, s, I);

    } else if(first == andi){
        printf("%3d: andi $%d, $%d, %d\n",turn, t, s, I);

    } else if(first == ori){
        printf("%3d: ori  $%d, $%d, %d\n",turn, t, s, I);

    } else if(first == lui){
        printf("%3d: lui  $%d, %d\n",turn, t, I);

    } else if((line) == sys){
        printf("%3d: syscall\n", turn);
    }
    return;
}