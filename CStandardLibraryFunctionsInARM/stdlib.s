		AREA	|.text|, CODE, READONLY, ALIGN=2
		THUMB

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; void _bzero( void *s, int n )
; Parameters
;	s 		- pointer to the memory location to zero-initialize
;	n		- a number of bytes to zero-initialize
; Return value
;   none
		EXPORT	_bzero
_bzero
		; r0 = s
		; r1 = n
		PUSH {r1-r12,lr}		
		; you need to add some code here for part 1 implmentation
		; according to man7, bzero erases data at n bytes memory,
		; so essentially we're setting each character by zero
		MOV		R2, #0	; use a register that represents zero
		MOV		R3, R1	; initialize our counter
bzero_loop	
		
		CMP		R3, #0			; check if we're done erasing amount of n bytes
		BEQ		end_bzero		; branch to end if we're done erasing n bytes
		SUB		R3, R3, #1		; subtract the n bytes 
		STRB 	R2, [R0], #1 	; erase R0's current character with zero, then post offset by 1
		B 		bzero_loop			; go back to the loop

end_bzero
		POP {r1-r12,lr}	
		BX		lr



;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; char* _strncpy( char* dest, char* src, int size )
; Parameters
;   dest 	- pointer to the buffer to copy to
;	src		- pointer to the zero-terminated string to copy from
;	size	- a total of n bytes
; Return value
;   dest
		EXPORT	_strncpy
_strncpy
		; r0 = dest
		; r1 = src
		; r2 = size
		PUSH {r1-r12,lr}		
		; you need to add some code here for part 1 implmentation
		; strncpy() is simple, but one characteristic of strncpy() is that
		; if src is less than n, additional null bytes are written until we reach size
		MOV		R3, R2					; assign a counter for our bytes written (size)
		MOV		R5, R0					; R0 is dest address, so save it to R5 so we can reassign it as return address.
strncpy_loop	
		LDRB 	R4, [R1], #1			; load a character from R1 and assign it to R4, then increment
		CMP		R4, #0					; check if we reached end of src
		BEQ		end_strncpy_loop		; break out of loop
		SUB		R3, R3, #1				; decrement
		STRB	R4, [R0], #1			; write the copied string that is stored in R4 into R0, then increment
		B 		strncpy_loop			; go back to the for loop
end_strncpy_loop						; we need to check if we written enough bytes after the loop
		CMP		R3, #0					; check if we've actually written the size of bytes		
		BEQ		end_strncpy				; end the function if we've written the right amount of bytes
		BNE		null_loop				; otherwise, keep writing null characters until we are finished
null_loop
		CMP		R3, #0					; keep track of counter	
		BEQ		end_strncpy				; end strncpy() once we've written amount correct amount of bytes
		SUB		R3, R3, #1				; decrement the bytes left we've written
		STRB	R4, [R0], #1			; R4 is already 0 by this point (null terminated) so we can keep using this
		B		null_loop				; go back to null_loop
end_strncpy
		MOV		R0, R5					; point back to dest starting address
		POP {r1-r12,lr}	
		BX		lr
		
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; void* _malloc( int size )
; Parameters
;	size	- #bytes to allocate
; Return value
;   void*	a pointer to the allocated space
		EXPORT	_malloc
_malloc
		; r0 = size
		PUSH {r1-r12,lr}		
		; you need to add some code here for part 2 implmentation
		MOV	R7, #0x1					; system call number for malloc - 0x1, so put it in R7
		SVC	#0 							; invoke the SVC 
		POP {r1-r12,lr}	
		BX		lr

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; void _free( void* addr )
; Parameters
;	size	- the address of a space to deallocate
; Return value
;   none
		EXPORT	_free
_free
		; r0 = addr
		PUSH {r1-r12,lr}		
		; you need to add some code here for part 2 implmentation
		MOV R7, #0x2					; system call number for free - 0x2 so put it in R7
		SVC #0							; invoke the SVC
		POP {r1-r12,lr}	
		BX		lr