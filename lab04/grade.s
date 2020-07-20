# read a mark and print the corresponding UNSW grade
    .text
main:
    la $a0, prompt      # printf("Enter a mark: ");
    li $v0, 4
    syscall

    li $v0, 5           # scanf("%d", mark);
    syscall

    move $t0, $v0   #t0 = mark

FL:
    bge $t0, 50, PS
    la $a0, fl          # printf("FL\n");
    li $v0, 4
    syscall
    j end
PS:
    bge $t0, 65, CR
    la $a0, ps          # printf("FL\n");
    li $v0, 4
    syscall
    j end


CR:
    bge $t0, 75, DN
    la $a0, cr          # printf("FL\n");
    li $v0, 4
    syscall
    j end

DN:
    bge $t0, 85, HD
    la $a0, dn          # printf("FL\n");
    li $v0, 4
    syscall
    j end

HD:
    
    la $a0, hd          # printf("FL\n");
    li $v0, 4
    syscall
    j end


end:
    jr $ra              # return


    .data
prompt:
    .asciiz "Enter a mark: "
fl:
    .asciiz "FL\n"
ps:
    .asciiz "PS\n"
cr:
    .asciiz "CR\n"
dn:
    .asciiz "DN\n"
hd:
    .asciiz "HD\n"
