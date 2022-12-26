		AREA	|.text|, CODE, READONLY, ALIGN=2
		THUMB

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; System Call Table
SYSTEMCALLTBL	EQU		0x20007B00
SYS_EXIT		EQU		0x0		; address 20007B00
SYS_MALLOC		EQU		0x1		; address 20007B04
SYS_FREE		EQU		0x2		; address 20007B08

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; System Call Table Initialization
		EXPORT	_systemcall_table_init 
_systemcall_table_init
		LDR		r0, = SYSTEMCALLTBL
		
		; Initialize SYSTEMCALLTBL[0] = _sys_exit
		LDR		r1, = _sys_exit		;  read R1 to system exit's address
		STR		r1, [r0]			; write system exit address to R0 by writing R1 to R0

		; Initialize_SYSTEMCALLTBL[1] = _sys_malloc
		; add your code here
		LDR		r1, = _sys_malloc	; then read R1 to system malloc's address
		STR		r1, [r0, #4]		; add system malloc's address to R0 by writing R1 to R0
	
		; Initialize_SYSTEMCALLTBL[2] = _sys_free
		; add your code here
		LDR		r1, = _sys_free		; then finally read R1 to system free's address
		STR		r1, [r0, #8]		; and finally add system malloc's address to R0 by writing R1 to R0
		
		BX		lr					; return to subroutine

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; System Call Table Jump Routine
; this is the function that will be callsed by SVC
        EXPORT	_systemcall_table_jump
_systemcall_table_jump
		PUSH {LR}
		LDR		r11, = SYSTEMCALLTBL	; load the starting address of SYSTEMCALLTBL
		MOV		r10, r7			; copy the system call number into r10
		LSL		r10, #0x2		; system call number * 4
		; complete the rest of the code
		LDR		R2, [R11, R10]	; retrieves the address of system call thats being invoked by reading it into R2
		BLX		R2				; then branch to register R2
		POP {LR}				; don't forget to POP LR before returning to SVC_Handler
		BX		lr				; return to SVC_Handler

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; System Call 
; provided for you to use

_sys_exit
		PUSH 	{lr}		; save lr
		BLX		r11	
		POP 	{lr}		; resume lr
		BX		lr
		
_sys_malloc
		IMPORT	_kalloc
		LDR		r11, = _kalloc	
		PUSH 	{lr}		; save lr
		BLX		r11			; call the _kalloc function 
		POP 	{lr}		; resume lr
		BX		lr
		
_sys_free
		IMPORT	_kfree
		LDR		r11, = _kfree	
		PUSH 	{lr}		; save lr
		BLX		r11			; call the _kfree function 
		POP 	{lr}		; resume lr
		BX		lr
		
		END


		