; Filename:     addMultiply.s 
; Author:       ece353 staff 
; Description:  

    export addMultiply

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
; Four arrays of 8-bit signed numbers are 
; passed via the first four paramters.
; The 5th paramter indicates the length of the
; arrays. For each entry in the array, the
; following operation takes place.
;
; Array3[i] = (Array0[i] + Array1[i]) * Array2[i]
;
; Parameter 0       Array Address 0
; Parameter 1       Array Address 1
; Parameter 2       Array Address 2
; Parameter 3       Array Address 3
; Parameter 4       Array Size 
;
; Returns
;   if ALL parameters are valid, return 0
;   else return -1.
;
;  An address is valid if it is non zero
;  The size is valid if it is greater than zero
;**********************************************
addMultiply PROC
    
    ; Validate Parameters
	CMP R0, #0
	BEQ invaild
	
	CMP R1, #0
	BEQ invaild
	
	CMP R2, #0
	BEQ invaild
	
	CMP R3, #0
	BEQ invaild
	
	PUSH{R4-R9}
	
	LDR R4, [SP, #24]  ; offset sinced we pushed before
	CMP R4, #0
	BLT invaild            ; EITHER < OR <=
    ; Save required registers
    
    ; For each index in the arrays, compute  
    ; Array3[i] = (Array0[i] + Array1[i]) * Array2[i]
	MOV R9, #0  ; counter
while_start
	CMP R4, R9
	BEQ done
	
	LDR R5, [R0, R9] ; load Array0[i]
	LDR R6, [R1, R9]
	LDR R7, [R2, R9]
	ADD R8, R5, R6   ; math opperations 
	MUL R8, R8, R7
	STR R8, [R3, R9]
	ADD R9, R9, #1
	B while_start
	
    ; Restore registers saved to the stack
    
    ; Return from the loop
	
	
invaild
	MOV R0, #-1
	POP{R4-R9}
	BX LR

done
	MOV R0, #0
	POP{R4-R9}
	BX LR
    ENDP


        
    align
        

    END            
