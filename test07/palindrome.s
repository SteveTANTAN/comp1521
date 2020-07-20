# read a line and print whether it is a palindrom

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
    li  $t1, 0
    sub $t2, $t0, 2
    la  $t3, line

loop2:
    bge	$t1, $t2, loop2end	# if $t1 >= 2t1loop2endtarget
    add $t5, $t3, $t1
    lb  $t6,($t5)
    add $t5, $t3, $t2
    lb  $t7,($t5)
    beq	$t6, $t7, ifend	# if $t6 == 7t1ifendtarget
    
    la   $a0, not_palindrome
    li   $v0, 4
    syscall
    j end
ifend:
    addi $t1, $t1, 1
    sub $t2, $t2, 1
    j	loop2				# jump toloop2
    
loop2end:

    

    la   $a0, palindrome
    li   $v0, 4
    syscall
end:
    li   $v0, 0          # return 0
    jr   $31


.data
str0:
    .asciiz "Enter a line of input: "
palindrome:
    .asciiz "palindrome\n"
not_palindrome:
    .asciiz "not palindrome\n"


# line of input stored here
line:
    .space 256

