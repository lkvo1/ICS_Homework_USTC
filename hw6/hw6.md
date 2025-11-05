# ICS HW6

## T1
(1)A ,F

(2) PUSH D 和 PUSH E 之后

(3) F, E; PUSH D 和 PUSH E 之后
## T2
- RET会从R7获取返回地址，但是中断的时候返回地址保存在PSR和PC中，所以RET会导致程序返回到错误的地方。
- RET不会恢复之前的PSR，这样PSR状态无法恢复，会导致权限错误。
## T3
(a) 可能会读取到重复的数据。

(b) 可能会覆盖之前未读取的数据，导致数据丢失。

(c) 可能会覆盖之前未显示的数据，导致显示数据丢失。
## T4
统计从x4000开始的10个字符中最高位为1的个数，并存储到mem[x5000]中。
## T5
```assembly
PUSH    ADD R6, R6, -1
        STR R0, R6, #0
        RET

JSR     POP
```

R1是一个计数器，用于记录输入字符数，并在逆序输出的时候充当循环次数计数器。
## T6
```assembly
    .ORIG x3000
    LD R6, SP_INIT
    ADD R6, R6, #-1
    LD R0, HALT_ADDR
    STR R0, R6, #0
    ADD R6, R6, #-1
    LD R0, PRINT6_ADDR
    STR R0, R6, #0
    ADD R6, R6, #-1
    LD R0, PRINT0_ADDR
    STR R0, R6, #0
    ADD R6, R6, #-1
    LD R0, PRINT3_ADDR
    STR R0, R6, #0
    RET

HALT_ADDR .FILL x3014
PRINT6_ADDR .FILL x5018
PRINT0_ADDR .FILL x5000
PRINT3_ADDR .FILL x500C
 
    HALT
SP_INIT .FILL xFE00
    .END
```
## T7
(a) 初始化3个，循环部分8 * 16 = 128个，最后一次BRz有1个，不计入HALT，则一共132个。

(b) 初始化部分有3个内存操作(30)，循环部分有LDR和STR有内存操作，一次循环消耗20 + 6 = 26个，总计为 26 * 16 = 416个。 总计为416 + 30 + 1 = 447个。

