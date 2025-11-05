# ICS HW7

## T1
1. 
```assembly
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
```
2. 
```assembly
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
```
3. - 删除不会影响代码正常运行的无用代码；优化循环中的冗余代码。
## T2
1. - 1518
   - 20
   - 711
2. - A B + C D - *
   - A B && C D ! && ||
   - A B C D E ^ - * F / + G * H -
3. $n + m - 1 = \sum_{i=1}^{m}opt_i$
## T3
1. - 返回地址(R7)；栈指针(R5)；需要恢复的寄存器；Callee需要的参数。
   - 不是所有寄存器都要被保存。
2. - 每次递归都相当于调用了一次函数，这样栈帧就会不断积累直到最深层的函数返回，导致内存溢出。
   - 手动调用栈的时候只需要保留最小限度的数据，比如递归计算n的时候只需要在栈上保留几个数字，而不是完整的栈帧。空间利用效率比较高。

## T4
1. 42 42
2. (a)优点：不需要额外内存；缺点：两个数组指向同一块内存，修改其中一个也会影响另一个的数据。
   (b)优点：a和b相互独立；缺点：需要手动分配内存，占用内存较大，且需要手动释放内存。
3. b可以指向其他数组，因为b是一个指针；a不能重新赋值，因为a是数组名。
## T5
1. 不存在高速缓存的时候，循环一共执行64次，每个循环需要访问两次内存，所以消耗64 * 40 = 2560单位时间。
2. 448.
## T6
a, b, c存到c[2：0]中
而后d, e, f, \0会存到i中，此时i的的值是d, e, f, \0四个字符的ASCII码拼在一起的结果，是一个奇怪的数字。

可以推断结构体中每个变量所占的内存是连续的。