	export bubble_sort
        
    AREA    FLASH, CODE, READONLY
    ALIGN
        

;******************************************************************************
; Description
;     Given the address in R7, it will read the unsigned byte at R7 and R7 + 1.
;     If [R7] > [R7 + 1], swap the values
;
;     Modify only registers R8 or greater.
;
; Parameters
;   R7 - Array Address
;
; Returns
;   Nothing
;******************************************************************************
swap_values PROC
     ;---------------------------------------
     ; START ADD CODE
     ;---------------------------------------
	 LDRB R8, [R7]    
	 LDRB R9, [R7, #1]
     CMP R8, R9   ; Compare the array values [R7] and [R7 + 1]
	 BLE DONE     ; No need for the swap
	 STRB R9, [R7]
	 STRB R8, [R7, #1]
     
DONE     
      BX      LR
     ;---------------------------------------
     ; END ADD CODE
     ;---------------------------------------
    ENDP


    
;******************************************************************************
; Description
;   Uses Bubble Sort to sort an array of unsigned 8-bit numbers.
;
;   Modify only registers R0-R7
;
; Parameters
;   R0 - Array Address
;   R1 - Array Size
;
; Returns
;   Nothing
;******************************************************************************
bubble_sort PROC

    ; Save registers
     PUSH   {R0-R12, LR}
     
     ;---------------------------------------
     ; START ADD CODE
     ;---------------------------------------
	
	 SUB R1, R1, #1
	 
	 
while_size

	 CMP R1, #0
	 BLE while_end  ; while(size > 0)
     MOV R2, #0  ; R2 holds Index
	 
while_index
	 CMP R2, R1
	 BGE while_size_sub  ; while(index < size)
	 
	 ;LDR R7, [R0, R2]
	 ADD R7, R0, R2
	 BL swap_values
	 
	 ADD R2, R2, #1  ; index++ 
	 B while_index

while_size_sub  ; size-- 
	SUB R1, R1, #1
	B while_size

while_end
     ; NOTE: The return from the function is already
     ; provided below
     
     ;---------------------------------------
     ; END ADD CODE
     ;---------------------------------------
     
    
    ; Restore Registers
    POP     {R0-R12, LR}
    
    ; Return from the function
    BX      LR
    
    ENDP
         
    END     

