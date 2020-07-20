########################################################################
# COMP1521 20T2 --- assignment 1: a cellular automaton renderer
# Xingyu(Steve) Tan
# z5237560 , July 2020.


# Maximum and minimum values for the 3 parameters.

MIN_WORLD_SIZE	=    1
MAX_WORLD_SIZE	=  128
MIN_GENERATIONS	= -256
MAX_GENERATIONS	=  256
MIN_RULE	=    0
MAX_RULE	=  255

# Characters used to print alive/dead cells.

ALIVE_CHAR	= '#'
DEAD_CHAR	= '.'

# Maximum number of bytes needs to store all generations of cells.

MAX_CELLS_BYTES	= (MAX_GENERATIONS + 1) * MAX_WORLD_SIZE

	.data

# `cells' is used to store successive generations.  Each byte will be 1
# if the cell is alive in that generation, and 0 otherwise.

cells:	.space MAX_CELLS_BYTES


# Some strings you'll need to use:

prompt_world_size:		.asciiz "Enter world size: "
error_world_size:		.asciiz "Invalid world size\n"
prompt_rule:			.asciiz "Enter rule: "
error_rule:			.asciiz "Invalid rule\n"
prompt_n_generations:		.asciiz "Enter how many generations: "
error_n_generations:		.asciiz "Invalid number of generations\n"

	
######################################################################
# .TEXT <main>
		.text
main:

# Frame:	$fp, $sp, $ra, $s1, $s2, $s3, $s4
# Uses:		$a0, $a1, $a2, $v0, $s0, $s1, $s2, $s3, $t0, $t1, $t2
# Clobbers:	$a0, $a1, $a2

# Register usage:
#	- 'world_size'	  in $s0
#	- 'n_generations' in $s1
#	- 'rule'	  in $s2
#	- 'reverse' 	  in $s3
#	- 'g'             in $s4

# Structure:
#	main
#	-> [prologue]
#	-> Word_size
#		-> if_main_word_size	
#	-> Rule
#		-> if_main_rule	
#	-> n_generations:
#		-> if_main_n_gene
#		
#	-> next_line
#
#   	-> if_main_gen_less_0
#   
#   	-> single_live_c
#
#   	-> loop_main_run_gen
#   	-> loop_main_run_gen_end
#
#   	-> if_main_reverse
#       	-> loop_main_reverse_1
#       	-> loop_main_reverse_1_end
#   	-> else_main_reverse:
#       	-> loop_main_reverse_2
#       	-> loop_main_reverse_2_end
#   	-> if_main_reverse_end
#
#   	-> main_error_end
#
#   	-> main_end
#	-> [epilogue]
	
# code:
	# set up stack frame
	sw	$fp, -4($sp)						# push $fp onto stack
	la	$fp, -4($sp)						# set up $fp for main function
	sw	$ra, -4($fp)						# save return address 
	sw	$s0, -8($fp)						# save $s0 to use as world_size
	sw	$s1, -12($fp)						# save $s1 to use as n_generations
	sw	$s2, -16($fp)						# save $s2 to use as rules
	sw	$s3, -20($fp)						# save $s3 to use as reverse
	sw	$s4, -24($fp)						# save $s4 to use as g
	addi	$sp, $sp, -28
world_size:
	la	$a0, prompt_world_size					# printf("Enter world size: ")
	li 	$v0, 4									
	syscall											

	li 	$v0, 5							# scanf("%d", &world_size);
	syscall			
	move	$s0, $v0						# $s0 = world_size

	blt	$s0, MIN_WORLD_SIZE, if_main_word_size			# if (world_size < MIN_WORLD_SIZE ||
	bgt	$s0, MAX_WORLD_SIZE, if_main_word_size			# world_size > MAX_WORLD_SIZE)
	j	Rule
if_main_word_size:
	
	la 	$a0, error_world_size        				# printf("Invalid world size\n");
	li 	$v0, 4
	syscall

	j 	main_error_end						# error return(return 1)


Rule:
	la 	$a0, prompt_rule         				# printf("Enter rule: ");
    	li 	$v0, 4
    	syscall

	li 	$v0, 5							# scanf("%d", &rule);
	syscall			
	move	$s2, $v0 						# $s2 = rule

	blt	$s2, MIN_RULE, if_main_rule				# if (rule < MIN_RULE ||
	bgt	$s2, MAX_RULE, if_main_rule				# rule > MAX_RULE) 
	j	n_generations
if_main_rule:
	
	la 	$a0, error_rule       					# printf("Invalid rule\n");
	li 	$v0, 4
	syscall
	j 	main_error_end						# error return(return 1)


n_generations:
	la 	$a0, prompt_n_generations   				# printf("Enter how many generations: ");
	li 	$v0, 4
	syscall

	li 	$v0, 5							# scanf("%d", &n_generations);
	syscall			
	move	$s1, $v0 						# $s1 = n_generations

	blt	$s1, MIN_GENERATIONS, if_main_n_gene			# if (rule < MIN_GENERATIONS||
	bgt	$s1, MAX_GENERATIONS, if_main_n_gene			# rule > MAX_GENERATIONS) 
	j	next_line
if_main_n_gene:
	
	la 	$a0, error_n_generations       				# printf("Invalid number of generations\n");
	li 	$v0, 4
	syscall
	j 	main_error_end						# error return(return 1)


next_line:
	li   	$a0, '\n'       					# printf("%c", '\n');
	li   	$v0, 11
	syscall


	##  $s0 = world_size   
	##  $s1 = n_generations  
	##  $s2 = rule  
	##  $s3 = reverse
	##  $s4 = g
	
	li	$s3, 0							# int reverse = 0
if_main_gen_less_0:
	bge	$s1, 0, single_live_c					# if (n_generations < 0)
	li	$s3, 1							# reverse = 1
	mul	$s1, $s1, -1						# n_generations = -n_generations



single_live_c:
	div	$t0, $s0, 2						# $t0 = world_size / 2;
	mul	$t0, $t0, 4						# callculat &cells[0][world_size / 2]
	la	$t1, cells
	add	$t0, $t0, $t1
	li	$t2, 1
	sw	$t2, ($t0)						# cells[0][world_size / 2] = 1;

	li	$s4, 1							# int g = 1
loop_main_run_gen:
	bgt	$s4, $s1, loop_main_run_gen_end				# g <= n_generations
	move	$a0, $s0						# $a0 = world_size
	move	$a1, $s4						# $a1 = g
	move	$a2, $s2						# $s2 = rule
	jal	run_generation						# run_generation(world_size, g, rule);
	
	
	addi	$s4, $s4, 1						# g++
	j	loop_main_run_gen
loop_main_run_gen_end:
	

	##  $s0 = world_size   
	##  $s1 = n_generations  
	##  $s2 = rule  
	##  $s3 = reverse
	##  $s4 = g


	bne	$s3, 1, else_main_reverse				#  if (reverse)	
if_main_reverse:
	move	$s4, $s1						# int g = n_generations

loop_main_reverse_1:
	blt	$s4, 0, loop_main_reverse_1_end				# g >= 0
	move	$a0, $s0						# $a0 = world_size
	move	$a1, $s4						# $a1 = g 
	jal	print_generation					# print_generation(world_size, g)


	addi	$s4, $s4, -1						# g--
	j	loop_main_reverse_1	

loop_main_reverse_1_end:
	j	if_main_reverse_end

else_main_reverse:
	li	$s4, 0							# int g = 0
loop_main_reverse_2:
	bgt	$s4, $s1, loop_main_reverse_2_end			# g <= n_generations
	move	$a0, $s0						# $a0 = world_size
	move	$a1, $s4						# $a1 = g 
	jal	print_generation					# print_generation(world_size, g)

	addi	$s4, $s4, 1						# g++
	j	loop_main_reverse_2
loop_main_reverse_2_end:

if_main_reverse_end:
	li	$v0, 0							# return 0;
	j	main_end								

main_error_end:
	li	$v0, 1							# return 1;

main_end:
# tear down stack frame
	lw	$ra, -4($fp)						# restore $ra for return
	lw	$s0, -8($fp)						# restore $s0 value
	lw	$s1, -12($fp)						# restore $s1 value
	lw 	$s2, -16($fp)						# restore $s2 value
	lw 	$s3, -20($fp)						# restore $s3 value
	lw 	$s4, -24($fp)						# restore $s4 value
	la	$sp, 4($fp)						# restore $sp (remove stack frame)
	lw	$fp, ($fp)						# restore $fp (remove stack frame)

	jr 	$ra							# return
########################################################################
# .TEXT <run_generation>
	.text
run_generation:

# Frame:	$fp, $sp, $ra, $s0
# Uses:		$a0, $a1, $a2, $s0, $t0, $t0, $t1, $t2, $t3, $t5, $t8
# Clobbers: 	$a0, $a1, $a2

# Register usage:
#	- '&cells[which_gen -1][x]' in $s0
#	- 'x' in $t0



# Structure:
#	main
#	-> [prologue]
#	-> loop_run_gen
#		-> L_run_gen
#		-> C_run_gen
#		-> R_run_gen
#
#		-> converting_run_gen
#
#		-> if_set_ture_run_gen
#		-> else_set_false_run_gen
#		-> if_set_run_gen_end
#
#	-> loop_run_gen_end
#	-> run_generation_end
#	-> [epilogue]

# Code:
# set up stack frame
	sw	$fp, -4($sp)						# push $fp onto stack
	la	$fp, -4($sp)						# set up $fp for main function
	sw	$ra, -4($fp)						# save return address 
	sw	$s0, -8($fp)						# save $s0 to use as &cells[which_gen -1][x]
	
	addi	$sp, $sp, -12


	li	$t0, 0							# int x = $t0 = 0
loop_run_gen:
	bge	$t0, $a0, loop_run_gen_end				# x < world_size

													
	sub	$t1, $a1, 1						# calculate &cells[which_gen -1][x]
	mul	$t1, $t1, MAX_WORLD_SIZE
	add	$t1, $t1, $t0

	la	$t2, cells								
	mul	$t1, $t1, 4
	add	$s0, $t1, $t2						# $s0 = &cells[which_generation - 1][x]


	#	- '&cells[which_gen -1][x]' in $s0
	#	- 'left' in $t1
	#	- 'center' in $t2
	#	- 'right' in $t3
	li 	$t1, 0							# left = 0 = $t1
L_run_gen:
	ble	$t0, 0, C_run_gen					# if (x > 0)	
	
	# left  = cells[which_generation - 1][x-1]
	lw	$t1, -4($s0)						# $t1 = left 

	# centre = cells[which_generation - 1][x];
C_run_gen:	
	lw	$t2, 0($s0)						# $t2 = center
	
	li	$t3, 0							# int right = 0
R_run_gen:
	sub	$t8, $a0, 1						# $t8 = world_size - 1 
	bge	$t0, $t8, converting_run_gen				# if (x < world_size - 1)
	
	# right = cells[which_generation - 1][x + 1]
	lw	$t3, 4($s0)						# $t3 = right

converting_run_gen:
	sll	$t1, $t1, 2						# left << 2
	sll	$t2, $t2, 1						# centre << 1
	or	$t1, $t1, $t2						# left = left|center

	or	$t1, $t1, $t3						# state = left|right

	li	$t2, 1
	sll	$t1, $t2, $t1						# bits = 1 << state
	
	and	$t2, $t1, $a2						# set = $t2 = rule & bit
	
	li	$t3, MAX_WORLD_SIZE					# &cells[which_generation][x];
	mul	$t3, $t3, 4	
	add	$t5, $s0, $t3							

	bne	$t2, 0, if_set_ture_run_gen				# if (set)
	j	else_set_false_run_gen		

if_set_ture_run_gen:
	li	$t8, 1 							# cells[which_generation][x] = 1;
	sw	$t8, ($t5)

	j 	if_set_run_gen_end
else_set_false_run_gen:

	li	$t8, 0
	sw	$t8, ($t5)						# cells[which_generation][x] = 0;

if_set_run_gen_end:

	add	$t0, $t0, 1						# x++
	j	loop_run_gen
loop_run_gen_end:

run_generation_end:
	# tear down stack frame
	lw	$s0, -8($fp)						# restore $s0 value
	lw	$ra, -4($fp)						# restore $ra for return
	
	la	$sp, 4($fp)						# restore $sp (remove stack frame)
	lw	$fp, ($fp)						# restore $fp (remove stack frame)
	
	jr	$ra							# return
	



########################################################################
# .TEXT <print_generation>
	.text
print_generation:

# Frame:	$fp, $sp, $ra, $s0
# Uses:		$a0, $a1, $v0, $s0, $t1, $t5, $t6, $t7, $t8
# Clobbers: 	$a0, $a1

# Register usage:
#	- 'world_size' in $s0
#	- 'which_generation' in $a1
#	- 'x' in $t5

# Structure:
#	-> [prologue]
#	-> loop_print_gen
#		-> p_finding_cell	
#
#		-> if_print_cell_ture
#		-> else_print_cell
#		-> if_print_gen_cell_end
#
#	-> loop_print_gen_end
#	-> print_end
#	-> [epilogue]

# Code:
	# set up stack frame
	sw	$fp, -4($sp)						# push $fp onto stack
	la	$fp, -4($sp)						# set up $fp for main function
	sw	$ra, -4($fp)						# save return address 
	sw	$s0, -8($fp)						# save $s0 to use as world_size
	addi	$sp, $sp, -12
	
	move 	$s0, $a0						# $s0 = $a0 = world_size

	
	move 	$a0, $a1        					# printf("%d", which_generation);
	li 	$v0, 1
	syscall

	li 	$a0, '\t'         					# printf('\t');
	li 	$v0, 11
	syscall
	
	li	$t5, 0							# $t5 = x = 0;
loop_print_gen:
	
	bge	$t5, $s0, loop_print_gen_end				# if $t0 >= $t1 then finish the loop	
					
p_finding_cell:	
	mul	$t6, $a1, MAX_WORLD_SIZE				# Calculate the &cells[which_generation][x]	
	add	$t6, $t6, $t5
	mul	$t6, $t6, 4
	la	$t7, cells
	add	$t6, $t7, $t6
	lw	$t8, ($t6)						# $t8 = cells[which_generation][x]
					


	bne	$t8, 0, if_print_cell_ture				# if (cells[which_generation][x]) true
	j	else_print_cell
if_print_cell_ture:
	li   	$a0, ALIVE_CHAR      					# printf("%c", '#');
	li   	$v0, 11
	syscall

	j	if_print_gen_cell_end		
else_print_cell:							# if (cells[which_generation][x]) false
	li   	$a0, DEAD_CHAR      					# printf("%c", '.');
	li   	$v0, 11
	syscall

if_print_gen_cell_end:	

	addi	$t5, $t5, 1						# x++;
	j	loop_print_gen
			
loop_print_gen_end:

	li   	$a0, '\n'       					# printf("%c", '\n');
	li   	$v0, 11
	syscall

print_end:
	# tear down stack frame
	lw	$s0, -8($fp)						# restore $s0 value
	lw	$ra, -4($fp)						# restore $ra for return
	
	la	$sp, 4($fp)						# restore $sp (remove stack frame)
	lw	$fp, ($fp)						# restore $fp (remove stack frame)
	
	jr	$ra							# return
