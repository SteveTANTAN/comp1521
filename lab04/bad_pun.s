       
        .data
        words:
                .asciiz "I MIPS you!\n"
        
        .text
        main:
        la      $t0, words
        li      $v0, 4
        move    $a0, $t0
       
        syscall
        jr  $ra






