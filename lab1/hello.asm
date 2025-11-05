; filepath: program.asm
        .ORIG x3000

; 读取输入 x
        LEA R0, PROMPT_X
        PUTS
        GETC                ; 读取字符
        OUT                 ; 回显
        LD R1, ASCII_OFFSET
        ADD R0, R0, R1      ; 转换为数字
        ST R0, X            ; 存储 x

; 输入换行
        LD R0, NEWLINE
        OUT

; 初始化变量
        AND R1, R1, #0
        ADD R1, R1, #2
        ST R1, Y            ; y = 2

; 计算 z = y * 3 + 1 = 2 * 3 + 1 = 7
        AND R2, R2, #0
        ADD R2, R1, R1      ; 2 * 2 = 4
        ADD R2, R2, R1      ; 4 + 2 = 6
        ADD R2, R2, #1      ; 6 + 1 = 7
        ST R2, Z            ; z = 7

; 初始化循环计数器 i = 0
        AND R3, R3, #0      ; i = 0

LOOP    LD R2, Z
        NOT R2, R2
        ADD R2, R2, #1      ; R2 = -z
        ADD R4, R3, R2      ; R4 = i - z
        BRzp END_LOOP       ; if i >= z, exit loop

; y = i + 3
        AND R1, R1, #0
        ADD R1, R3, #3
        ST R1, Y

; if(i < 0)
        ADD R3, R3, #0
        BRn DECREMENT       ; if i < 0

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
        LD R1, ASCII_OFFSET
        NOT R1, R1
        ADD R1, R1, #1      ; R1 = -ASCII_OFFSET
        ADD R0, R0, R1      ; 转换为ASCII
        OUT
        LD R0, NEWLINE
        OUT

; i++
        ADD R3, R3, #1
        BR LOOP

END_LOOP
; 输出最终结果 x, y, z
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

; 子程序:打印数字
PRINT_NUM
        ST R7, SAVE_R7
        ADD R0, R0, #-10
        BRn SINGLE_DIGIT
        
        ; 两位数
        AND R1, R1, #0
        ADD R1, R1, #1
        ST R1, TENS
        BR PRINT_TENS

SINGLE_DIGIT
        ADD R0, R0, #10
        LD R1, ASCII_OFFSET
        NOT R1, R1
        ADD R1, R1, #1
        ADD R0, R0, R1
        OUT
        LD R7, SAVE_R7
        RET

PRINT_TENS
        LD R0, TENS
        LD R1, ASCII_OFFSET
        NOT R1, R1
        ADD R1, R1, #1
        ADD R0, R0, R1
        OUT
        LD R7, SAVE_R7
        RET

; 数据区
PROMPT_X    .STRINGZ "Enter x: "
RESULT_MSG  .STRINGZ "\nResult: "
ASCII_OFFSET .FILL x0030
NEWLINE     .FILL x000A
SPACE       .FILL x0020
X           .BLKW 1
Y           .BLKW 1
Z           .BLKW 1
TENS        .BLKW 1
SAVE_R7     .BLKW 1

        .END