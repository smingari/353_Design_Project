; Filename:     main.s 
; Author:       ece353 staff 
; Description:  

    export __main

;**********************************************
; Add Symbolic Constants here
;**********************************************
BYTE      EQU 1
HALF_WORD EQU 2
WORD      EQU 4
;**********************************************
; SRAM
;**********************************************
    AREA    SRAM, READWRITE
ARRAY1 SPACE HALF_WORD*8
ARRAY2 SPACE HALF_WORD*8

    align
        
;**********************************************
; Constant Variables (FLASH) Segment
;**********************************************
    AREA    FLASH, CODE, READONLY
LTABLE DCW 0
	   DCW 1
	   DCW 8
	   DCW 27
	   DCW 64
	   DCW 125
	   DCW 216
	   DCW 343
    align

;**********************************************
; Code (FLASH) Segment
; main assembly program
;**********************************************
__main   PROC
	
	ADR R0, LTABLE
	LDR R1, =(ARRAY1)    ; In course notes under Loading ADR (2)
	LDR R2, =(ARRAY2)
	LDRH R10, [R0, HALF_WORD*6]
	
	LDRH R3, [R0, #HALF_WORD]!  ; Load and store array 1
	STRH R3, [R1, #HALF_WORD]!
	LDRH R3, [R0, #HALF_WORD]!
	STRH R3, [R1, #HALF_WORD]!
	LDRH R3, [R0, #HALF_WORD]!
	STRH R3, [R1, #HALF_WORD]!
	LDRH R3, [R0, #HALF_WORD]!
	STRH R3, [R1, #HALF_WORD]!
	LDRH R3, [R0, #HALF_WORD]!
	STRH R3, [R1, #HALF_WORD]!
	LDRH R3, [R0, #HALF_WORD]!
	STRH R3, [R1, #HALF_WORD]!
	LDRH R3, [R0, #HALF_WORD]!
	STRH R3, [R1, #HALF_WORD]!
	LDRH R3, [R0, #HALF_WORD]!
	STRH R3, [R1, #HALF_WORD]!


	LDRH R3, [R0], #HALF_WORD ; Index 1
	STRH R3, [R2], #HALF_WORD
	LDRH R3, [R0], #HALF_WORD ; Index 2
	STRH R3, [R2], #HALF_WORD
	LDRH R3, [R0], #HALF_WORD ; Index 3
	STRH R3, [R2], #HALF_WORD
	LDRH R3, [R0], #HALF_WORD ; Index 4
	STRH R3, [R2], #HALF_WORD
	LDRH R3, [R0], #HALF_WORD ; Index 5
	STRH R3, [R2], #HALF_WORD
	LDRH R3, [R0], #HALF_WORD ; Index 6
	STRH R3, [R2], #HALF_WORD
	LDRH R3, [R0], #HALF_WORD ; Index 7
	STRH R3, [R2], #HALF_WORD
	LDRH R3, [R0], #HALF_WORD ; Index 8
	STRH R3, [R2], #HALF_WORD


    
    ; DO NOT MODIFY ANTHING BELOW THIS LINE!!!	
        
INFINITE_LOOP
    B INFINITE_LOOP
    
    ENDP
    align
        

    END            