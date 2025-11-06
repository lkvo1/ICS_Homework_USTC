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