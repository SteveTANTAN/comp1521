1########################################################################
# COMP1521 20T2 --- assignment 1: a cellular automaton renderer
#
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

prompt_world_size:	.asciiz "Enter world size: "
error_world_size:	.asciiz "Invalid world size\n"
prompt_rule:		.asciiz "Enter rule: "
error_rule:		.asciiz "Invalid rule\n"
prompt_n_generations:	.asciiz "Enter how many generations: "
error_n_generations:	.asciiz "Invalid number of generations\n"

	.text

	#
	# REPLACE THIS COMMENT WITH A LIST OF THE REGISTERS USED IN
	# `main', AND THE PURPOSES THEY ARE ARE USED FOR
	#
	# YOU SHOULD ALSO NOTE WHICH REGISTERS DO NOT HAVE THEIR
	# ORIGINAL VALUE WHEN `run_generation' FINISHES
	#

main:
# set up stack frame
	sw	$fp, -4($sp)
	la	$fp, -4($sp)
	sw	$ra, -4($fp)
	sw	$s0, -8($fp)
	sw 	$s1, -12($fp)
	sw 	$s2, -16($fp)
	sw 	$s3, -20($fp)
	sw	$s4, -24($fp)
	addi	$sp, $sp, -28
	
	la 	$a0, prompt_world_size         	# printf("Enter world size: ")
    	li 	$v0, 4
    	syscall

	li 	$v0, 5				# scanf("%d", &world_size);
    	syscall			
	move	$s0, $v0 			# $s0 = world_size

	blt	$s0, MIN_WORLD_SIZE, if_main_word_size	# if (world_size < MIN_WORLD_SIZE ||
	bgt	$s0, MAX_WORLD_SIZE, if_main_word_size	# world_size > MAX_WORLD_SIZE)
	j	if_main_word_size_end
if_main_word_size:
	
	la 	$a0, error_world_size        	# printf("Invalid world size\n");
    	li 	$v0, 4
    	syscall

	j main_error_end
if_main_word_size_end:
###########
	la 	$a0, prompt_rule         	# printf("Enter rule: ");
    	li 	$v0, 4
    	syscall

	li 	$v0, 5				# scanf("%d", &rule);
    	syscall			
	move	$s2, $v0 			# $s2 = rule

	blt	$s2, MIN_RULE, if_main_rule	# if (rule < MIN_RULE ||
	bgt	$s2, MAX_RULE, if_main_rule	# rule > MAX_RULE) 
	j	if_main_rule_end
if_main_rule:
	
	la 	$a0, error_rule       		# printf("Invalid rule\n");
	li 	$v0, 4
	syscall
	j 	main_error_end
if_main_rule_end:
###################
	la 	$a0, prompt_n_generations   # printf("Enter how many generations: ");
    	li 	$v0, 4
    	syscall

	li 	$v0, 5						# scanf("%d", &n_generations);
    	syscall			
	move	$s1, $v0 				# $s1 = n_generations

	blt	$s1, MIN_GENERATIONS, if_main_n_gene	# if (rule < MIN_GENERATIONS||
	bgt	$s1, MAX_GENERATIONS, if_main_n_gene	# rule > MAX_GENERATIONS) 
	j	if_main_n_gene_end
if_main_n_gene:
	
	la 	$a0, error_n_generations       		# printf("Invalid number of generations\n");
	li 	$v0, 4
	syscall
	j 	main_error_end
if_main_n_gene_end:
####################################

	li   	$a0, '\n'       	# printf("%c", '\n');
	li   	$v0, 11
    	syscall


##  $s0 = world_size   
##  $s1 = n_generations  
##  $s2 = rule  
##  $s3 = reverse
##  $s4 = g
	li	$s3, 0
if_main_gen:
	bge	$s1, 0, if_main_gen_end
	li	$s3, 1
	mul	$s1, $s1, -1
	
if_main_gen_end:

	mul	$t0, $s0, 2		# cells[0][world_size / 2] = 1;
	la	$t1, cells
	add	$t0, $t0, $t1
	li	$t2, 1
	sw	$t2, ($t0)

	li	$s4, 1			# int g = 1
loop_main_1:
	bgt	$s4, $s1, loop_main_1_end	# g <= n_generations
	move	$a0, $s0
	move	$a1, $s4
	move	$a2, $s2
	jal	run_generation			# jump t and save position to $ra
	
	
	addi	$s4, $s4, 1
	j	loop_main_1
loop_main_1_end:

	bne	$s3, 1, else_main_reverse		
if_main_reverse:
	move	$s4, $s1
##  $s0 = world_size   
##  $s1 = n_generations  
##  $s2 = rule  
##  $s3 = reverse
##  $s4 = g
loop_main_reverse_1:
	blt	$s4, 0, loop_main_reverse_1_end
	move	$a0, $s0
	move	$a1, $s4
	jal	print_generation


	addi	$s4, $s4, -1
	j	loop_main_reverse_1
loop_main_reverse_1_end:

	j	if_main_reverse_end
else_main_reverse:
	li	$s4, 0
loop_main_reverse_2:
	bgt	$s4, $s1, loop_main_reverse_2_end	# g <= n_generations
	move	$a0, $s0
	move	$a1, $s4
	jal	print_generation

	addi	$s4, $s4, 1
	j	loop_main_reverse_2
loop_main_reverse_2_end:

if_main_reverse_end:







	# replace the syscall below with
	#
	# li	$v0, 0
	# jr	$ra
	#
	# if your code for `main' preserves $ra by saving it on the
	# stack, and restoring it after calling `print_world' and
	# `run_generation'.  [ there are style marks for this ]

	



	#
	# Given `world_size', `which_generation', and `rule', calculate
	# a new generation according to `rule' and store it in `cells'.
	#

	#
	# REPLACE THIS COMMENT WITH A LIST OF THE REGISTERS USED IN
	# `run_generation', AND THE PURPOSES THEY ARE ARE USED FOR
	#
	# YOU SHOULD ALSO NOTE WHICH REGISTERS DO NOT HAVE THEIR
	# ORIGINAL VALUE WHEN `run_generation' FINISHES

	li	$v0, 0
	j	main_end			# jump t

main_error_end:
	li	$v0, 1

main_end:
	# tear down stack frame
	lw	$ra, -4($fp)
   	lw	$s0, -8($fp)
   	lw	$s1, -12($fp)
	lw 	$s2, -16($fp)
	lw 	$s3, -20($fp)
	lw 	$s4, -24($fp)
   	la	$sp, 4($fp)
   	lw	$fp, ($fp)

	jr 	$ra
#########################################################################


##########################################################################

run_generation:
# set up stack frame
	sw	$fp, -4($sp)
	la	$fp, -4($sp)
	sw	$ra, -4($fp)
	sw	$s0, -8($fp)
	sw 	$s1, -12($fp)
	sw 	$s2, -16($fp)
	sw 	$s3, -20($fp)
	sw	$s4, -24($fp)
	sw	$s5, -28($fp)
	addi	$sp, $sp, -32
	
	move	$s0, $a0
	move	$s1, $a1
	move	$s2, $a2

	#$t0 = x
	# $t8 = [which_gen -1 ][x]
	#world_size = $s0
	#which_gen = $s1
	#rule = $s2
	#left = $s3
	#center = $s4
	#right = $s5

	
	li	$t0, 0				# int x = $t0 = 0
loop_run_gen:
	bge	$t0, $s0, loop_run_gen_end

	la	$t1, cells
	sub	$t2, $s1, 1
	mul 	$t2, $t2, MAX_WORLD_SIZE
	add	$t3, $t2, $t0
	mul 	$t4, $t3, 4

	add	$t8, $t4, $t1			# $t8 = &[which_gen-1][x]

	
	li 	$s3, 0				# left = 0 = $s3
if_1_run_gen:
	ble	$t0, 0, if_1_run_gen_end# if (x > 0)	
	
	lw	$s3, -4($t8)		# left = cells[which_generation - 1][x - 1];
	
	
	

if_1_run_gen_end:

	# centre = cells[which_generation - 1][x];
	
	lw	$s4, 0($t8)		# $s4 = center
	
	li	$s5, 0
if_2_run_gen:
	sub	$t1, $s0, 1
	bge	$t0, $t1, if_2_run_gen_end
	
	# $t3 = right # = cells[which_generation - 1][x + 1]
	lw	$s5, 4($t8)		
if_2_run_gen_end:
	sll	$s3, $s3, 2		# left << 2
	sll	$s4, $s4, 1		# centre << 1
	sll	$s5, $s5, 0
	or	$t1, $s3, $s4		# $t1 = $s3|$s4

	or	$t2, $t1, $s5		# state = $t2 = $t1|$s5

	li	$t3, 1
	sllv	$t4, $t3, $t2		# $t4 = bits = 1 << state
	
	and	$s3, $t4, $s2		# set = $s3 = rule & bit

	la	$t1, cells
	mul 	$t2, $s1, MAX_WORLD_SIZE
	add	$t3, $t2, $t0
	mul 	$t4, $t3, 4

	add	$t8, $t4, $t1			# $t8 = &[which_gen][x]
	
	

	bne	$s3, 0, if_3_run_gen
	j	else_3_run_gen
	
if_3_run_gen:
	li	$t7, 1 
	sw	$t7, ($t8)

	j 	if_3_run_gen_end
else_3_run_gen:
	
	li	$t7, 0
	sw	$t7, ($t8)

if_3_run_gen_end:
	
	add	$t0, $t0, 1		# x++
	j	loop_run_gen
loop_run_gen_end:

run_generation_end:
# tear down stack frame
	# tear down stack frame
	lw	$ra, -4($fp)
   	lw	$s0, -8($fp)
   	lw	$s1, -12($fp)
	lw 	$s2, -16($fp)
	lw 	$s3, -20($fp)
	lw 	$s4, -24($fp)
	lw	$s5, -28($fp)
   	la	$sp, 4($fp)
   	lw	$fp, ($fp)
	
	jr	$ra
####################################################################

#########################################################################
print_generation:
	# set up stack frame
	sw	$fp, -4($sp)
	la	$fp, -4($sp)
	sw	$ra, -4($fp)
	sw	$s0, -8($fp)
	
	addi	$sp, $sp, -12
	
	move 	$s0, $a0		# $s0 = $a0 = world_size
	
	
	move 	$a0, $a1        	# printf("%d", which_generation);
    	li 	$v0, 1
   	syscall

	li 	$a0, '\t'         	# pprintf('\t');
    	li 	$v0, 11
   	syscall
	
	li	$t5, 0			#$t5 = x = 0;
loop_print_gen:
	# if $t0 >= $t1 then finish the loop
	bge	$t5, $s0, loop_print_gen_end	
					
	# Calculate the &cells[which_generation][x]
	mul	$t6, $a1, MAX_WORLD_SIZE
	
	add	$t6, $t6, $t5
	mul	$t6, $t6, 4

	la	$t7, cells
	add	$t6, $t7, $t6
	lw	$t8, ($t6)		#$t8 = cells[which_generation][x]
					

if_print_gen:
	bne	$t8, 1, else_print_gen	
	li   	$a0, ALIVE_CHAR      	# printf("%c", '#');
    	li   	$v0, 11
    	syscall


	j	if_print_gen_end		
else_print_gen:
	li   	$a0, DEAD_CHAR      	# printf("%c", '.');
    	li   	$v0, 11
        syscall

	
if_print_gen_end:	

	addi	$t5, $t5, 1
	j	loop_print_gen
				
loop_print_gen_end:

	li   	$a0, '\n'       	# printf("%c", '\n');
    	li   	$v0, 11
    	syscall

print_end:
# tear down stack frame
	lw	$ra, -4($fp)
   	lw	$s0, -8($fp)
   	
   	la	$sp, 4($fp)
   	lw	$fp, ($fp)

	jr 	$ra

