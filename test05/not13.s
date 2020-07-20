main:
    li $v0, 5           #   scanf("%d", &x);
    syscall             #
    move $t0, $v0

    li $v0, 5           #   scanf("%d", &y);
    syscall             #
    move $t1, $v0

    addi $t2 $t0 1
loop:
    bge $t2 $t1 end
    bne $t2, 13, printf	# if   != $t1 then target
loopend:
    addi $t2 $t2 1
    j loop
    
printf:
   
    move $a0, $t2          #   printf("%d\n", 42);
    li $v0, 1
    syscall
    li   $a0, '\n'      #   printf("%c", '\n');
    li   $v0, 11
    syscall
    j loopend

end:

    li $v0, 0           # return 0
    jr $31
