# Sieve of Eratosthenes
# https://en.wikipedia.org/wiki/Sieve_of_Eratosthenes
main:

    li $t0 , 0  # i = $t0
loop1:
    bge	$t0, 1000, loop1end	# if$t0 >1000t1loop1endtarget
    la  $t2, prime # calculate &prime[i]
    add $t3, $t2, $t0 #
    li  $t4, 1
    sb  $t4, ($t3) # prime[i] = 1

    addi $t0, $t0, 1
    j loop1
loop1end:


    li $t0 , 2  # i = $t0
loop2:
    bge	$t0, 1000, loop2end
    la  $t2, prime      # calculate &prime[i]
    add $t3, $t2, $t0 #
    lb  $t4, ($t3)
    bne $t4, 1, ifend
    
if:
    move $a0, $t0         # printf("%d", 42)
    li $v0, 1
    syscall

    li   $a0, '\n'      # printf("%c", '\n');
    li   $v0, 11
    syscall

    mul $t5, $t0, 2
loop3:
    bge $t5, 1000, loop3end
    la $t2, prime
    add $t3, $t2, $t5
    li  $t6,0
    sb  $t6,($t3)

    add $t5, $t5, $t0
    j loop3
loop3end:

    
ifend:
    addi $t0, $t0, 1
    j loop2
loop2end:
    li $v0, 0           # return 0
    jr $31

.data
prime:
    .space 1000