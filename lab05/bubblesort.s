# read 10 numbers into an array
# bubblesort them
# then print the 10 numbers

# i in register $t0
# registers $t1, $t2 & $t3 used to hold temporary results

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

#$t8 = swap
    li $t8, 1
while1:
    bne	$t8, 1, endwhile1	# if $t0 != $t1 then target
    li $t8, 0
    li $t0, 1
while2:
    bge $t0, 10, endwhile2
    
    mul $t1, $t0, 4     #   calculate &numbers[i]
    la $t2, numbers     #
    add $t3, $t1, $t2   #
    lw $t5, ($t3)       #  load x  
    sub $t3, $t3, 4
    lw $t6, ($t3)       #   load y
    bge $t5, $t6, endif
    
    sw $t5, ($t3)
    
    addi $t3, $t3, 4
    sw $t6, ($t3)
    li $t8, 1

endif:
    addi $t0, $t0, 1
    j while2 
endwhile2:
    j while1
endwhile1:


    li $t0, 0           # i = 0
loop1:
    bge $t0, 10, end1   # while (i < 10) {

    mul $t1, $t0, 4     #   calculate &numbers[i]
    la $t2, numbers     #
    add $t3, $t1, $t2   #
    lw $a0, ($t3)       #   load numbers[i] into $a0
    li $v0, 1           #   printf("%d", numbers[i])
    syscall

    li   $a0, '\n'      #   printf("%c", '\n');
    li   $v0, 11
    syscall

    add $t0, $t0, 1     #   i++
    b loop1             # }
end1:

    jr $31              # return

.data

numbers:
    .word 0 0 0 0 0 0 0 0 0 0  # int numbers[10] = {0};

