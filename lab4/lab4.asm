; x在x3100处, y在x3101处, 结果最终存x3200处
; func_ROUTE: input = (x, y) -> output = ROUTE(x - 1, y) + ROUTE(x, y - 1), 不妨将栈元素视为二元组(x, y)
; 每个LOOP, 如果栈不为空, 则弹出(x, y), 然后入栈(x-1, y)和(x, y-1). 同时如果 x==0 或 y==0 ,则结果+1
; 每个ROUTE用R1取x, R2取y\
        .ORIG x3000

        ; 测试用: 将X_VALUE和 Y_VALUE分别存到x3100和x3101中
        AND R5, R5, #0
        ADD R5, R5, #1          ; x = 1
        ST R5, X_VALUE
        STI R5, X_ADDR

        AND R5, R5, #0
        ADD R5, R5, #1          ; y = 1
        ST R5, Y_VALUE
        STI R5, Y_ADDR

        LD R6, STACK_TOP        ; 初始化栈指针R6, 此时默认x

        LDI R5, X_ADDR
        STR R5, R6, #2          ; 将 x 存到栈底
        LDI R5, Y_ADDR
        STR R5, R6, #1          ; 将 y 存到栈顶

        AND R5, R5, #0          ; 清零R5用作结果计数器
        

        
ROUTE
        ; 如果栈是空的, 相当于已经得到了结果
        ST R4, SAVER4
        ST R6, SAVER6

        LD R4, NEG_STACK_TOP
        ADD R6, R4, R6
        BRz CALCRET

        LD R4, SAVER4
        LD R6, SAVER6
        ; 如果栈是非空的, 处理栈顶元素
        JSR POP
        ADD R2, R0, #0          ; 将y保存到R2
        JSR POP
        ADD R1, R0, #0          ; 将x保存到R1

        ADD R5, R5, #1          ;
        ADD R1, R1, #-1         ;
        BRn ROUTE               ;
        ADD R5, R5, #-1         ;
        ADD R1, R1, #1          ;

        ADD R5, R5, #1          ;
        ADD R2, R2, #-1         ;
        BRn ROUTE               ;
        ADD R2, R2, #1          ;
        ADD R5, R5, #-1         ;

        ADD R0, R1, #-1         ;
        JSR PUSH                ;
        ADD R0, R2, #0          ;
        JSR PUSH                ;
        ADD R0, R1, #0          ;
        JSR PUSH                ;
        ADD R0, R2, #-1         ;
        JSR PUSH                ; 如果x != 0 && y != 0, 入栈(x-1, y)和(x, y-1)


        BRnzp ROUTE
        
PUSH    ST R0, SAVER0
        ST R1, SAVER1
        ST R2, SAVER2
        ST R3, SAVER3
        ST R4, SAVER4
        ST R5, SAVER5
        
        STR R0, R6, #0          ; 保存R0到栈中
        ADD R6, R6, #-1
        

        LD R0, SAVER0
        LD R1, SAVER1
        LD R2, SAVER2
        LD R3, SAVER3
        LD R4, SAVER4
        LD R5, SAVER5
        
        RET

POP     ST R1, SAVER1
        ST R2, SAVER2
        ST R3, SAVER3
        ST R4, SAVER4
        ST R5, SAVER5

        LDR R0, R6, #1          ; 取栈顶元素到R0
        ADD R6, R6, #1
        
        LD R1, SAVER1
        LD R2, SAVER2
        LD R3, SAVER3
        LD R4, SAVER4
        LD R5, SAVER5

        RET

CALCRET ; 此时R5中已经存储了ROUTE(x, y), 需计算5 * R5 - x - y
        ADD R4, R5, #0
        ADD R5, R5, R5
        ADD R5, R5, R5
        ADD R5, R5, R4

        LD R0, X_VALUE
        NOT R0, R0
        ADD R0, R0, #1
        ADD R5, R5, R0

        LD R0, Y_VALUE
        NOT R0, R0
        ADD R0, R0, #1
        ADD R5, R5, R0

        STI R5, RESADDR

        HALT

; 数据存储区
SAVER0  .FILL x0000
SAVER1  .FILL x0000
SAVER2  .FILL x0000
SAVER3  .FILL x0000
SAVER4  .FILL x0000
SAVER5  .FILL x0000
SAVER6  .FILL x0000
X_ADDR  .FILL x3100
Y_ADDR  .FILL x3101
X_VALUE .FILL x0001
Y_VALUE .FILL x0001
STACK_TOP .FILL x31FD        ; 栈顶指针初始值, 此时默认已经存储了第一队x, y, 因此是x31FFF - 2 = x31FD
NEG_STACK_TOP .FILL xCE01    ; 栈底坐标的相反数-x3FFF, 这是为了判断方便
RESADDR .FILL x3200
        .END