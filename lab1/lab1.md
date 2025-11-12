# ICS Lab1

## Assembly Code

```assembly
        .ORIG x3000

        AND R1, R1, #0      ; Clear R1
        ADD R1, R1, #5      ; Set val1 = 5
        
        AND R2, R2, #0      ; Clear R2
        ADD R2, R2, #10     ; Set val2 = 10

        AND R0, R0, #0      ; Clear R0
        ADD R0, R0, #1      ; Set cond = 0 / 1

Main    ADD R0, R0, #0      ; If cond == 0
        BRz Else            ; Branch to Else
        
        AND R0, R0, #0      ; Clear R0
        ADD R0, R1, #0      ; R0 = val1 (copy R1 to R0)
        BR DONE             ; Skip ELSE branch
        
Else    AND R0, R0, #0      ; Clear R0
        ADD R0, R2, #0      ; R0 = val2 (copy R2 to R0)
        
Done    HALT

        .END
```

## Machine Code

```machineCode
0011    000000000000    ; .ORIG x3000

0101 001 001 1 00000    ; AND R1, R1, #0
0001 001 001 1 00101    ; ADD R1, R1, #5

0101 010 010 1 00000    ; AND R2, R2, #0
0001 010 010 1 01010    ; ADD R2, R2, #10

0001 000 000 1 00000    ; AND R0, R0, #0
0001 000 000 1 00000    ; ADD R0, R0, #0    ; Set R0(cond) as 0
0001 000 000 1 00000    ; ADD R0, R0, #0    ; If cond == 0
0000 010   000000011    ; Brach to else
0101 000 000 1 00000    ; AND R0, R0, #0    ; Clear R0
0001 000 001 1 00000    ; ADD R0, R1, #0    ; Then set R0 = R1
0000 111   000000010    ; Branch to Done
0101 000 000 1 00000    ; AND R0, R0, #0    ; Clear R0
0001 000 010 1 00000    ; ADD R0, R2, #0    ; Then set R0 = R2

1111    000000100101    ; HALT
```

## Description

I first write the assembly language according to the requirements, and then convert it to machine code. I did plan A. First, Clear R0, R1, R2. Then, judge the branch according to the result of R0 + 1. If R0 = 0, jump to else; otherwise, execute in sequence. Then both branches will eventually reach HALT.

## Tests

The results are as follows:

### R1 = 5,  R2 = 10, Cond = 0
### R1 = 14， R2 = 3, Cond = 0
### R1 = 14， R2 = 3, Cond = 1