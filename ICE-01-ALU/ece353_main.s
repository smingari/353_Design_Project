; Filename:     main.s 
; Author:       ece353 staff 
; Description:  

    export __main

;**********************************************
; SRAM
;**********************************************
    AREA    SRAM, READWRITE
    align
        
;**********************************************
; Constant Variables (FLASH) Segment
;**********************************************
    AREA    FLASH, CODE, READONLY
    align

;**********************************************
; Code (FLASH) Segment
; main assembly program
;**********************************************
__main   PROC
    
    ; DO NOT MODIFY ANTHING ABOVE THIS LINE!!! 
   
    
    ; CHECK POINT (1)
    ; Initialize R1 to be 20 (decimal)
		MOV R1, #20
    
    ; CHECK POINT (2)
    ; Add 15 to R1 using an immediate
		ADD R1, R1, #0xF
    
    ; CHECK POINT (3)
    ; Multiply R1 by 3
		MOV R4, #0x3
		MUL R1, R1, R4

    ; CHECK POINT (4)
    ; Clear (set to 0) bits 4:0 of R1
    ; All other bits should not be affected
		BIC R1, R1, #0x1F

    ; CHECK POINT (5)
    ; Set bits 31:25 and 21:16 of R1 to be a 1
    ; All other bits should not be affected
		ORR R1, R1, #0xFE000000
		ORR R1, R1, #0x003F0000
    
    ; CHECK POINT (6)
    ; Invert ONLY bits 3-9 of R1
		EOR R1, R1, #0x3F8

    ; CHECK POINT (7) 
    ; Move the contents of R1 to R0
		MOV R0, R1
    
    ; CHECK POINT (8) 
    ; Total the bytes in R0 and place the results in R2
    ; R2 = R0[31:24] + R0[23:16] + R0[15:8] + R0[7:0]
    ; Treat each byte an an unsigned 8-bit number
		AND R3, R0, #0xFF000000 
		LSR R3, R3, #24
		
		AND R4, R0, #0x00FF0000
		LSR R4, R4, #16
		
		AND R5, R0, #0x0000FF00
		LSR R5, R5, #8
		
		AND R6, R0, #0x000000FF
		LSR R6, R6, #4
		
		ADD R2, R3, R4
		ADD R2, R2, R5
		ADD R2, R2, R6

    ; CHECK POINT (9) 
    ; Initialize R4 to be decimal value 100
    ; Initialize R5 to be decimal value 110
    ; Without using the subtract instruction, subtract R5 from R4
    ; and place the results in R6.  Use as many ARM assembly 
    ; instructions as needed.
	
		MOV R4, #100
		MOV R5, #110
		MVN R5, R5
		ADD R5, R5, #1
		ADD R6, R4, R5
    
    ; DO NOT MODIFY ANTHING BELOW THIS LINE!!!
        
INFINITE_LOOP
    B   INFINITE_LOOP
    ENDP
    align
        

    END
        
        
