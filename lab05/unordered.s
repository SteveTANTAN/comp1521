# Read 10 numbers into an array
# print 0 if they are in non-decreasing order
# print 1 otherwise

# i in register $t0

main:

    li $t0, 0           # i = 0
loop0:
    bge $t0, 10, end0   # while (i < 10) {

    li $v0, 5           #   scanf("%d", &numbers[i]);
    syscall             #

    mul $t1, $t0, 4     #   calculate &numbers[i]
    la $t2, numbers     #
    add $t3, $t1, $t2   #
    sw $v0, ($t3)       #   store entered number in array

    add $t0, $t0, 1     #   i++;
    b loop0             # }
end0:


    # PUT YOUR CODE HERE
    #t5 = swap
    #t0 = i, t7 = x, t8 = y

    li $t5 0
    li $t0 0
while1:
    bge $t0 10 endwhile
    mul $t1 $t0 4     #   calculate &numbers[i]
    la $t2, numbers     #
    add $t3 $t1 $t2   #
    lw $t7 ($t3)
    sub	$t3 $t3 4		#$t3 = $31 - 4
    lw  $t8 ($t3)
    blt $t7 $t8 if
    j endif
if:
    li $t5 1
endif:
    addi $t0 $t0 1
    j while1


endwhile:
    move $a0, $t5         # printf("%d", 42)
    li $v0, 1           #
    syscall

    li   $a0, '\n'      # printf("%c", '\n');
    li   $v0, 11
    syscall
end:
    jr $31

.data

numbers:
    .word 0 0 0 0 0 0 0 0 0 0  # int numbers[10] = {0};

