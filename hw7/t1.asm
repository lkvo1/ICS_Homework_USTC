        .ORIG x3000
        LEA R0, PROMPT_X
        PUTS
        GETC                
        OUT                 
        LD R1, ASCII_OFFSET
        NOT R1, R1
        ADD R1, R1, #1
        ADD R0, R0, R1
        ST R0, X      
        LD R0, NEWLINE
        OUT
        AND R1, R1, #0
        ADD R1, R1, #2
        ST R1, Y
        AND R2, R2, #0
        ADD R2, R1, R1
        ADD R2, R2, R1
        ADD R2, R2, #1
        ST R2, Z      
        AND R3, R3, #0
LOOP    LD R2, Z
        NOT R2, R2
        ADD R2, R2, #1
        ADD R4, R3, R2
        BRzp END_LOOP 
        AND R1, R1, #0
        ADD R1, R3, #3
        ST R1, Y
        ADD R3, R3, #0
        BRn DECREMENT
INCREMENT
        LD R0, X
        ADD R0, R0, #1
        ST R0, X
        BR PRINT_X
DECREMENT
        LD R0, X
        ADD R0, R0, #-1
        ST R0, X
PRINT_X
        LD R0, X
        JSR PRINT_NUM
        LD R0, NEWLINE
        OUT
        ADD R3, R3, #1
        BR LOOP
END_LOOP
        LEA R0, RESULT_MSG
        PUTS
        LD R0, X
        JSR PRINT_NUM
        LD R0, SPACE
        OUT
        LD R0, Y
        JSR PRINT_NUM
        LD R0, SPACE
        OUT
        LD R0, Z
        JSR PRINT_NUM
        HALT
PRINT_NUM
        ST R7, SAVE_R7
        ST R1, SAVE_R1
        ST R2, SAVE_R2
        ADD R0, R0, #0
        BRzp POSITIVE
        ST R0, TEMP_NUM
        LD R0, MINUS
        OUT
        LD R0, TEMP_NUM
        NOT R0, R0
        ADD R0, R0, #1
POSITIVE
        AND R1, R1, #0
COUNT_TENS
        ADD R0, R0, #-10
        BRn DONE_TENS
        ADD R1, R1, #1
        BR COUNT_TENS
DONE_TENS
        ADD R0, R0, #10
        ST R0, ONES
        ADD R1, R1, #0
        BRz PRINT_ONES_ONLY
        ADD R0, R1, #0
        LD R2, ASCII_OFFSET
        ADD R0, R0, R2
        OUT
PRINT_ONES_ONLY
        LD R0, ONES
        LD R2, ASCII_OFFSET
        ADD R0, R0, R2
        OUT
        LD R2, SAVE_R2
        LD R1, SAVE_R1
        LD R7, SAVE_R7
        RET
PROMPT_X    .STRINGZ "Enter x: "
RESULT_MSG  .STRINGZ "\nResult: "
ASCII_OFFSET .FILL x0030
NEWLINE     .FILL x000A
SPACE       .FILL x0020
MINUS       .FILL x002D
X           .BLKW 1
Y           .BLKW 1
Z           .BLKW 1
ONES        .BLKW 1
TEMP_NUM    .BLKW 1
SAVE_R7     .BLKW 1
SAVE_R1     .BLKW 1
SAVE_R2     .BLKW 1
        .END