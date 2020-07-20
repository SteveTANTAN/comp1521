# read a line and print its length

main:
    la   $a0, str0       # printf("Enter a line of input: ");
    li   $v0, 4
    syscall

    la   $a0, line
    la   $a1, 256
    li   $v0, 8          # fgets(buffer, 256, stdin)
    syscall              #

    li  $t0, 0
    la  $t1, line
    	
loop1:
    add		$t3, $t1, $t0		# $t3 = $t1 + 0t2
    
    lb	    $t2, ($t3)
    beq		$t2, 0, loop1end	    # if $t2 =0t1loop1target

    add		$t0, $t0, 1		# $t0 = $01 1t2
    

    j   loop1
loop1end:

    la   $a0, str1       # printf("Line length: ");
    li   $v0, 4
    syscall

    move   $a0, $t0        # printf("%d", i);
    li   $v0, 1
    syscall

    li   $a0, '\n'       # printf("%c", '\n');
    li   $v0, 11
    syscall

    li   $v0, 0          # return 0
    jr   $31


.data
str0:
    .asciiz "Enter a line of input: "
str1:
    .asciiz "Line length: "


# line of input stored here
line:
    .space 256

