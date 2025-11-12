        .ORIG x3000

        AND R0, R0, #0      ; Clear R0
        LD  R0, VALUE    ; Load N into R0

        LEA R3, RESULT     ; R3 = &RESULT[0]
        ADD R3, R3, R0     ; R3 = &RESULT[N]
        ADD R3, R3, #1     ; R3 = &RESULT[N + 1]

        ADD R0, R0, #-2     ; R0 = N - 2
        BRnz SKIP   ; If N <= 2, skip the loop

        AND R1, R1, #0      ; Clear R1 as q
        AND R2, R2, #0      ; Clear R2 as v
        AND R4, R4, #0      ; Clear R4 as t

        AND R0, R0, #0      ; Clear R0 to store i as iterator
        LD R0, VALUE        ; Load N into R0
        ADD R0, R0, #-2     ; R0 = N - 2, then BRn so that we loop N-2 times
        NOT R0, R0
        ADD R0, R0, #1      ; set R0 = -(N - 2) as LOOP counter (better for get value in RESULT)

                            ; LDR Rx, R3, Ry  ; Load RESULT[[Ry]]] into Rx
LOOP    BRz DONE
        AND R2, R2, #0      ; Clear R2 for v storage


STEP1   ADD R4, R0, #-1     ; t1 = i - 1
        LDR R1, R3, R4      ; q1 = Q(i - 1)

        NOT R1, R1          
        ADD R1, R1, #1      ; R1 = -q1
        ADD R4, R0, R1      ; R4 = t1 = i - q1
        
        LDR R4, R3, R4      ; R4 = Q(t1)

        ADD R2, R2, R4      ; R2 += v1


STEP2   ADD R4, R0, #-2     ; t2 = i - 2
        LDR R1, R3, R4      ; q2 = Q(i - 2)

        NOT R1, R1
        ADD R1, R1, #1      ; R1 = -q2
        ADD R4, R0, R1      ; R4 = t = i - q2

        LDR R4, R3, R4      ; R4 = Q(t2)

        ADD R2, R2, R4      ; R2 += v2  now we have v1 + v2 in R2


STEP3   STR R2, R3, R0      ; Store v into Q(i)
        ADD R0, R0 , #1     ; i = i + 1
        BRnzp LOOP

SKIP    AND R0, R0, #0      ; Clear R0
        ADD R0, R0, #1      ; R0 = 1
        ST R0, RESULT       ; If N <= 2, result = 1

DONE    HALT

VALUE   .FILL #5  ; N value
RESULT  .BLKW 100   ; Reserve space for the Q[1...N] results
        .END
