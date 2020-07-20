# Read a number n and print the first n tetrahedral numbers
# https://en.wikipedia.org/wiki/Tetrahedral_number

main:                  # int main(void) {

    la $a0, prompt     # printf("Enter how many: ");
    li $v0, 4
    syscall

    li $v0, 5           # scanf("%d", number);
    syscall
    #$t0 howmay
    #$t1= n     $t2  =  i   $t3  =  j  $t4 = total
    li $t1, 1
    move $t0 $v0
    loop1top:
        bgt $t1, $t0, end
        li $t4, 0
        li $t3, 1

        loop2top:
            bgt $t3, $t1, loop1under
            li $t2, 1
            
            loop3:
            bgt $t2, $t3, loop2under
            add $t4, $t4, $t2
            addi $t2, $t2, 1
            j loop3
    
        loop2under:
        addi $t3, $t3, 1 
        j loop2top
    
    
    
    loop1under:
        move $a0 $t4
        li   $v0, 1
        syscall

        li   $a0, '\n'      # printf("%c", '\n');
        li   $v0, 11
        syscall
        addi $t1, $t1, 1
        j loop1top

end:
    jr   $ra           # return

    .data
prompt:
    .asciiz "Enter how many: "
