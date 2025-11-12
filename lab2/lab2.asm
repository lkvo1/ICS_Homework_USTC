        .ORIG x3000

        AND R0, R0, #0      ; Clear R0
        LD  R0, VALUE    ; Load N into R0

        LEA R3, RESULT     ; R3 = &RESULT[0]
        ADD R3, R3, R0     ; R3 = &RESULT[N]
        ADD R3, R3, #1     ; R3 = &RESULT[N + 1]

        ADD R0, R0, #-2     ; R0 = N - 2
        BRnz SKIP   ; If N <= 2, skip the loop

        AND R1, R1, #0      ; Clear R1 to use as Q[i - Q[i-1]] (q1)
        AND R2, R2, #0      ; Clear R2 to use as Q[i - Q[i-2]] (q2)

        AND R0, R0, #0      ; Clear R0 to store i as iterator
        LD R0, VALUE        ; Load N into R0
        ADD R0, R0, #-2     ; R0 = N - 2, then BRn so that we loop N-2 times
        NOT R0, R0
        ADD R0, R0, #1      ; set R0 = -(N - 2) as LOOP counter (better for get value in RESULT)

; LDR Rx, R3, R0  ; Load RESULT[i] into Rx
LOOP    BRz DONE
        
        ; load q1 = Q(i - 1)
        AND R1, R1, #0
        LDR R1, R3, R0       ; q1 = Q[i - 1]

        NOT R1, R1
        ADD R1, R1, #1       ; R1 = -q1

        AND R4, R4, #0       ; Clear t1
        ADD R4, R0, R1       ; t1 = i - q1



        ADD R0, R0 , #1      ; i = i + 1
        BRnzp LOOP

SKIP    AND R0, R0, #0      ; Clear R0
        ADD R0, R0, #1      ; R0 = 1
        ST R0, RESULT       ; If N <= 2, result = 1

DONE    HALT

VALUE   .FILL #5  ; N value
RESULT  .BLKW 100   ; Reserve space for the Q[1...N] results
        .END
