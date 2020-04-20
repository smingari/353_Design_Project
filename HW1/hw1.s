; hw1.s ECE353


    export hw1_search_memory
	import WS2812B_write

BYTE 			EQU   1
LED_ARRAY_SIZE  EQU  24
;**********************************************
; SRAM
;**********************************************
    AREA    SRAM, READWRITE
LED_ARRAY   SPACE   LED_ARRAY_SIZE*BYTE
	

    align
		
;**********************************************
; Constant Variables (FLASH) Segment
;**********************************************
    AREA    FLASH, CODE, READONLY
START           EQU   0x54525453        ; ASCII values backwards in hex for little endian
BREK            EQU   0x4B455242
HOLD            EQU   0x444C4F48
LED             EQU   0x0044454C



;**********************************************
; hw1_update_leds
;
; Updates the LEDs with the current value saved
; in the LED array
;**********************************************

hw1_update_leds PROC
	PUSH{R1-R2, LR}
	LDR R1, =(LED_ARRAY)  ; Loads parameters for the WS2812B_write method
	MOV R2, #8            ; Number of LEDs to update
	BL WS2812B_write
	POP{R1-R2, PC}
	BX LR
	ENDP


;**********************************************
; hw1_ascii_to_hex
;
; Converts the ASCII value passed in to a hex
; value. The ASCII values are 0-9, a-f, A-f
; Parameters:
; R0 - ASCII Character
;
; Returns:
; R0 - numerical value, 0xFFFFFFF if invaild
;
;**********************************************

hw1_ascii_to_hex PROC
	
	CMP R0, #0x30     ; Compare to see if parameter is in the range of 30< 
	BLT check_cap_alpha	
	
	CMP R0, #0x39
	BGT check_cap_alpha  ; Value was outside of numerical range
	SUB R0, R0, #0x30    ; Value was in range, remove 0x30 to get numerical value and return 
	B DONE
	
check_cap_alpha

	CMP R0, #0x41        ; Compare to see if parameter is in the range of 40<
	BLT check_alpha	
	
	CMP R0, #0x46
	BGT check_alpha         ; Value was outside of numerical range
	SUB R0, R0, #0x37       ; Value was in range, remove 0x37 to get capital value and return 
	B DONE

check_alpha

	CMP R0, #0x61           ; Value was outside of a-e ascii 
	BLT invalid_hex	
	
	CMP R0, #0x66
	BGT invalid_hex    
	SUB R0, R0, #0x57       ; Value was in range, remove 0x46 to get value and return 
	B DONE

invalid_hex
	MOV32 R0, #0xFFFFFFFF   ; Return invaild number

DONE
	BX LR
	ENDP

;**********************************************
; hw1_ascii_to_dec
;
; Converts the ASCII value passed in to a dec
; value. The ASCII values are 0-9
; Parameters:
; R0 - ASCII Character
;
; Returns:
; R0 - numerical value, 0xFFFFFFF if invaild
;
;**********************************************

hw1_ascii_to_dec PROC
	PUSH{R1-R2, LR}
	MOV R1, #0x30     ; Compare to see if parameter is in the range of 30<
	CMP R0, R1
	BLT invaild_dec
	MOV R2, #0x39     ; Value was outside of numerical range
	CMP R0, R2
	BGT invaild_dec
	SUB R0, R0, R1    ; Value was in range, remove 0x30 to get numerical value and return
	B DONE_DEC
	
invaild_dec
	MOV32 R0, #0xFFFFFFFF  ; Return invaild number
	
DONE_DEC	
	POP{R1-R2, PC}
	BX LR
	ENDP

;**********************************************
; hw1_init
;
; Turns off all LEDSs on the board by writing 0 
; to the LED Array
;
;**********************************************
hw1_init PROC
	PUSH{R1-R2, LR}
	
	MOV32 R1, #0x00000000
	LDR R2, =(LED_ARRAY) ; Load LED Array ADDR
	
	STR R1, [R2], #4	 ; Clear all bytes of the array
	STR R1, [R2], #4
	STR R1, [R2], #4
	STR R1, [R2], #4
	STR R1, [R2], #4
	STR R1, [R2], #4
	
	BL hw1_update_leds   ; calls update to reset lights
	POP{R1-R2, PC}
	BX LR
	ENDP


;**********************************************
; hw1_ledx
;
; Updates the color values in the LED array
; to the colors passed in as parameters.
; Then calls hw1_update_leds to update the color
;
; Parameters:
; R0 - LED number to update
;
; R1 - Value to write the LEDs to
;
;**********************************************

hw1_ledx PROC
	
	PUSH{R1-R4, R6-R8, LR}
	
	; Creates a mask to extract RGB values
	; Shifts all bytes to be LSB
	
	AND R6, R1, #0xFF0000   ; Green
	LSR R6, R6, #16
	AND R7, R1, #0x00FF00   ; Red
	LSR R7, R7, #8
	AND R8, R1, #0x0000FF   ; Blue
	
	
	LDR R2, =(LED_ARRAY)
	MOV R4, #7
	SUB R3, R4, R0          ; Since LED7 is at base address we need to make an offset
	ADD R1, R3, R3
	ADD R1, R1, R3          ; Multiply by 3 for each GRB part of array
	ADD R2, R2, R1
	
	STRB R6, [R2], #1	    ; Store Array Values
	STRB R7, [R2], #1	
	STRB R8, [R2], #1
	
	BL hw1_update_leds      ; Call update to LEDs
		
	POP{R1-R4, R6-R8, PC}	
		
	BX LR
	ENDP
	
;**********************************************
; hw1_wait
;
; Loops around an incremental loop to wait out
; cycles passed in
;
; Parameters:
; R0 - Number to wait for
;
;**********************************************
hw1_wait PROC
	PUSH{R1}
	MOV R1, #0x0             ; Load counter
while_start_wait	
	CMP R0, R1
	BEQ done_wait 
	ADD R1, R1, #1           ; Increment 
	B while_start_wait
done_wait	
	POP{R1}
	BX LR
	ENDP



;**********************************************
; hw1_search_memory
;
; Converts the ASCII value passed in to a hex
; value. The ASCII values are 0-9, a-f, A-f
;
; Parameters:
; R0 - starting address of memory that contains
; the command to update LED
;
;**********************************************

hw1_search_memory PROC	
	PUSH{R0-R4, R7-R10, LR}

	MOV R9, R0  ; Move the passed param into R9 to be modified and so R0 can be used to pass params
while_start     ; An infinite loop unless a BREK command
	
	LDR R2, [R9]
	MOV32 R3, START
    CMP R2, R3      ; check for STRT

	BNE check_break ; check for the next condition
	BL hw1_init		
					
	
	
check_break
	MOV32 R3, BREK
	CMP R2, R3
	BNE check_hold  ; check for the next condition
	B end_prog      ; value is break so we will enter an infinte loop
	
	
check_hold	
	MOV32 R3, HOLD  ; check to see if cmd is HOLD
	CMP R2, R3
	BNE check_LED   ; Not HOLD so check for the last condition
					
	LDRB R0, [R9, #4]       ; Index by 4 to past HOLD ascii
	BL hw1_ascii_to_dec		; Need to convert HEX to Dec and multiply to get param
	
	MOV32 R3, #0xFFFFFFFF   ; check that value is valid
	CMP R0, R3
	BEQ end_of_wait
	
	MOV R4, #1000
	MUL R10, R0, R4         ; Shift to correct place value
	MOV R8, #0              ; Clear register to so it can be used for number handing
	ADD R8, R8, R10
	
	LDRB R0, [R9, #5]       ; index by 5 to get new value
	BL hw1_ascii_to_dec	
	
	CMP R0, R3
	BEQ end_of_wait
	
	MOV R4, #100
	MUL R10, R0, R4
	ADD R8, R8, R10
	
	LDRB R0, [R9, #6]   
	BL hw1_ascii_to_dec	
	
	CMP R0, R3
	BEQ end_of_wait
	
	MOV R4, #10
	MUL R10, R0, R4
	ADD R8, R8, R10
	
	LDRB R0, [R9, #7]      
	BL hw1_ascii_to_dec	
	
	CMP R0, R3
	BEQ end_of_wait
	
	ADD R8, R8, R0
	
	MOV R0, R8
	MOV32 R8, #0x2710       ; multiply by 10000
	MUL R0, R0, R8
	BL hw1_wait  
	
end_of_wait

	ADD R9, R9, #1 ; Increment loop by 1
	B while_start
	
	
check_LED
	BIC R2, R2, #0xFF000000  ; clear  X from LEDX to get LED to compare 
	MOV32 R3, LED
	CMP R2, R3  ;LED
	BNE end_search 
	
	MOV R8, #0
	LDR R2, [R9]
	AND R0, R2, #0xFF000000; Use mask to get least significant byte and convert to dec
	LSR R0, R0, #24
	BL hw1_ascii_to_dec
	
	MOV32 R3, #0xFFFFFFFF   ; Checks for an invaild ascii input
	CMP R0, R3
	BEQ end_of_check_led
	CMP R0, #7              ; Checks for a valid LED number
	BGT end_of_check_led  
	
	MOV R7, R0      ; Holds the LED numerical value so we can call ascii to hex
	
	LDR R1, [R9, #4]  ; Index by 4 to start reading ASCII values for LED command
	
	MOV32 R10, #0xFF  ; Load mask to read bit values
	AND R0, R1, R10
	BL hw1_ascii_to_hex
	
	MOV32 R3, #0xFFFFFFFF
	CMP R0, R3
	BEQ end_of_check_led
	
	ADD R8, R8, R0, LSL #20    ; Shift to MSB
	
	MOV32 R10, #0xFF00
	AND R0, R1, R10
	LSR R0, R0, #8
	BL hw1_ascii_to_hex
	
	CMP R0, R3
	BEQ end_of_check_led
	
	ADD R8, R8, R0, LSL #16
	
	MOV32 R10, #0xFF0000
	
	CMP R0, R3
	BEQ end_of_check_led
	
	AND R0, R1, R10
	LSR R0, R0, #16
	BL hw1_ascii_to_hex
	
	CMP R0, R3
	BEQ end_of_check_led
	
	ADD R8, R8, R0, LSL #12
	
	MOV32 R10, #0xFF000000
	AND R0, R1, R10
	LSR R0, R0, #24
	BL hw1_ascii_to_hex
	
	CMP R0, R3
	BEQ end_of_check_led
	
	ADD R8, R8, R0, LSL #8
	
	LDR R1, [R9, #8]   ; LDR NEW VALUES SINCE CMD HAS 6 BYTES
	
	MOV32 R10, #0xFF
	AND R0, R1, R10
	BL hw1_ascii_to_hex
	
	CMP R0, R3
	BEQ end_of_check_led
	
	ADD R8, R8, R0, LSL #4      ; R8 holds the value for the color change
	
	MOV32 R10, #0xFF00
	AND R0, R1, R10
	LSR R0, R0, #8
	BL hw1_ascii_to_hex
	
	CMP R0, R3
	BEQ end_of_check_led
	
	ADD R8, R8, R0
	
	MOV R0, R7   ; LOAD PARAMS
	MOV R1, R8
	
end_of_check_led	
	BL hw1_ledx
	ADD R9, R9, #1     ; Increment by 1
	B while_start
	
end_search	
	ADD R9, R9, #1      ; Increment by 1
	B while_start

end_prog
	B end_prog          ; Infinite loop
	
	
	POP{R0-R4, R7-R10, PC}   
	BX LR
	ENDP
		
    align        
    END





