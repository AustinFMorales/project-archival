		AREA	|.text|, CODE, READONLY, ALIGN=2
		THUMB

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; System Call Table
HEAP_TOP	EQU		0x20001000
HEAP_BOT	EQU		0x20004FE0
MAX_SIZE	EQU		0x00004000		; 16KB = 2^14
MIN_SIZE	EQU		0x00000020		; 32B  = 2^5
	
MCB_TOP		EQU		0x20006800      ; 2^10B = 1K Space
MCB_BOT		EQU		0x20006BFE
MCB_ENT_SZ	EQU		0x00000002		; 2B per entry
MCB_TOTAL	EQU		512				; 2^9 = 512 entries
	
INVALID		EQU		-1				; an invalid id
	
;
; Each MCB Entry
; FEDCBA9876543210
; 00SSSSSSSSS0000U					S bits are used for Heap size, U=1 Used U=0 Not Used

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Memory Control Block Initialization
; void _kinit( )
; this routine must be called from Reset_Handler in startup_TM4C129.s
; before you invoke main( ) in driver_keil
		EXPORT	_kinit
_kinit
		; you must correctly set the value of each MCB block
		; complete your code
		; translate each part of heap.c code in to heap.s as best as you can
		PUSH {R0-R12, LR}
		LDR R0, =HEAP_TOP 	; start from the heap_top
		MOV	R1, #0			; lets use this register to initialize each memory as zero
		LDR R5, =MAX_SIZE	; use this register to store MAX_SIZE value
		LDR R7, =HEAP_TOP	; lets calculate 0x20005000
		ADDS R7, R7, R5		; add max_size and heap_top to get 0x20005000
loop_init1					; first loop of initialization
		STRB R1, [R0], #1	; initialize each memory block of memory as zero
		CMP R0, R7			; compare R0 (current heap space) 
		BGT	l_exit1			; exit out of first loop ( > 0x20005000)
		B	loop_init1		; continue for loop
l_exit1
		LDR R2, =MCB_TOP	; read into MCB_TOP using R2
		STR R5, [R2] 		; initialize MCB_TOP to max size (essentially intialize one big block of memory on startup)
		STRB R1, [R2, #2]	; set this MCB entry to unoccupied memory (memory that can be used for allocation) by setting it to zero
		LDR R3, =MCB_TOP	; read from MCB_TOP
		ADD	R3, #4			; get 0x20006804 starting address for loop
		; LDR R6, [R3, #4]	; we want to start at 0x20006804 based on heap.c
		; LDR R4, =MCB_BOT	; get the MCB bottom value
		;ADD R4, R4, #2		; then ADD 2 to get 0x20006C00
		LDR R4, =0x20006C00	; load end address for for loop
loop_init2					; second for loop - initializes each MCB
		CMP R3, R4			; (0x20006804 < 0x20006C00)
		BGE l_exit2			; if its greater than end address, exit out of the loop (done with initializing MCB_blocks
		STRB R1, [R3]		; write the first byte entry of this MCB as free space
		STRB R1, [R3, #1] 	; write the second byte entry of this MCB as free space
		ADD R3, R3, #2		; iterate by going to each entry 2B MCB entry
		B loop_init2		; continue for loop
l_exit2
		POP {R0-R12, LR}
		BX		lr

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Kernel Memory Allocation
; void* _k_alloc( int size )
		EXPORT	_kalloc
_kalloc
		; complete your code
		; return value should be saved into r0
		; first recreate kalloc, then recreate ralloc
		; R0 is going to be the parameter passed in (size)
		; then R0 is going to return the address pointing to free memory
		; we call RALLOC(size, left_mcb_addr, right_mcb_addr)
		; R0 = size
		PUSH {R1-R12, LR}	; push the values in to the stack to start ralloc recursion
		LDR	R1, =MCB_TOP	; R1 = left_mcb_addr
		LDR R2, =MCB_BOT	; R2 = right_mcb_addr
		BL _ralloc			; recursively call _ralloc
		POP {R1-R12, LR}	; pop all the values afterwards when the recursion is finished
		BX		lr			; branch to link register (go to previous subroutine)
		
		
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Kernel Memory Allocation
; void* _r_alloc( int size )
		EXPORT	_ralloc
_ralloc
		; R3 = entire_mcb_addr_space = right_mcb_addr - left_mcb_addr  + mcb_ent_sz;
		SUB R3, R2, R1	; first do right_mcb_addr - left_mcb_addr
		ADD R3, R3, #MCB_ENT_SZ	; 
		; R4 = half_mcb_addr_space = entire_mcb_addr / 2
		MOV R4, R3, LSR #1	; logical shift right causes division by 2
		; R5 = midpoint_mcb_addr = left_mcb_addr + half_mcb_addr_space
		ADD R5, R1, R4
		; R6 = heap_addr = 0
		MOV R6, #0
		; R7 = act_entire_heap_size = entire_mcb_addr_space * 16
		MOV R7, R3, LSL #4	; grab R3, and multiply by 2^4 which equals 16
		; R8 = act_half_heap_size = half_mcb_addr_space * 16
		MOV R8, R4, LSL #4	; grab R4, and multiply by 2^4
		
		CMP R0, R8		; make our comparator here
		BLE ralloc_if_clause ; size <= act_half_heap_size
		BGT ralloc_else_clause ; size > act_half_heap_size
		
ralloc_if_clause	; 	if ( size <= act_half_heap_size ) { 
		PUSH {R0-R8, LR}	; push our parameters and the necessary calculations
		; void* heap_addr = _ralloc( size, left_mcb_addr, midpoint_mcb_addr - mcb_ent_sz );
		SUB R2, R5, #2		; to call this replace the parameter of right_mcb
		BL _ralloc
		; check on the heap_addr
		CMP R0, #0
		; if this is true, then we need to search
		; the right buddy within the heap
		; since theres no valid space here
		POPEQ {R0-R8, LR}	
		; otherwise, we search on the left
		POPNE {R1} 			; pop the R1 to change the left_mcb_addr
		POPNE{R1-R8, LR}	; then use the new recursive value
		BNE initialize_left_midpoint

move_to_right	
		MOV R1, R5	; set left mcb parameter to the midpoint mcb
		PUSH {R3-R8, LR} ; push only the right register and other important registers
		BL _ralloc		; recursively call
		POP {R3-R8, LR} ; pop out our values from recursion
		B return_ralloc_addr ; return the heap addr

initialize_left_midpoint ; otherwise we can start initialize memory from the left using midpoint
		LDR R9, [R5] ; load the address
		; recreate this check ( array[ m2a( midpoint_mcb_addr ) ] & 0x01 ) == 0
		; AND R10, R9, #0x01 ; make the comparison
		;CMP R10, #0			; generate the comparator
		RORS R10, R9, #1		; get the carry bit to determine if the address is occupied
		STRCC R8, [R5]			; *(short *)&array[ m2a( midpoint_mcb_addr ) ] = act_half_heap_size;
		; BNE	return_ralloc_addr ; return heap_addr simply
		; STREQ R8, [R5] ; *(short *)&array[ m2a( midpoint_mcb_addr ) ] = act_half_heap_size;
		B return_ralloc_addr 
ralloc_else_clause	; 	search the right side 
		; if ( ( array[ m2a( left_mcb_addr ) ] & 0x01 ) != 0 )
		; recreate this statement
		LDR R9, [R1]	; load left_mcb_addr in the heap
		ANDS R10, R9, #0x01 ; is this space occupied
		MOVNE R0, #0 	   ; if it is, then we can't allocate space here
		BNE return_ralloc_addr	; return_ralloc_addr
		; BCS return_ralloc_addr
		BEQ initialize_right_midpoint
initialize_right_midpoint
		; if ( *(short *)&array[ m2a( left_mcb_addr ) ] < act_entire_heap_size )
		; r9 is already loaded with R1
		CMP R9, R7	; generate our comparator
		MOVLT R0, #0 ; return null address because left buddy is smaller than previous midpoint
		BLT return_ralloc_addr ; return
		ORR R9, R7, #1 ; allocate new memory on the left side of the mcb
		STR R9, [R1] ; then write it to R9
		
		LDR R9, =MCB_TOP ; get the constant
		SUB R0, R1, R9	; left_mcb addr - mcb top
		LDR R9, =HEAP_TOP ; lod heap top constant
		ADD R0, R9, R0, LSL #4 ; heap_top + ( r0 subresult * 16) -> LSL  2^4 which allows us to multiply by 16
		B return_ralloc_addr
return_ralloc_addr
		BX LR
		
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Kernel Memory De-allocation
; void *_kfree( void *ptr )	
		EXPORT	_kfree
_kfree
		
		; R0 is the input parameter, so its R0
		; complete your code
		; return value should be saved into r0
		PUSH {R1-R12, LR}		; push the registers in the stack so they can be restored for later
		; in this instance, R0 is already int addr  = (int)ptr, so we don't need to change R0 the input value
		
		; if ( addr < heap_top || addr > heap_bot )
		LDR R1, =HEAP_TOP	; assign R1 to compare HEAP_TOP
		LDR R2, =HEAP_BOT	; assign R2 to compare HEAP_BOT
		CMP R0, R1			; compare R0 (addr) to R1 (heap_top)
		MOVLT R0, #0		; set to null since the address to free cannot be less than HEAP_TOP
		BLT kfree_end		; retun NULL
		CMP R0, R2			; compare R0 (addr) to R2(heap_bot)
		MOVGT R0, #0		; set to null since the address to free cannot be more than HEAP_BOT
		BGT kfree_end		; return NULL
		
		; calculate mcb_addr, let R3 -> mcb_addr = mcb_top + (addr - heap_top) / 16
		LDR R3, =MCB_TOP	; start with MCB_TOP
		MOV R4, R0			; let R4 = addr
		SUB R4, R4, R1;		; R4 = (addr - heap_top)
		;MOV R5, #16			; store 16
		LSR	R4, #4			; divide by 16 by shifting right (/16)
		ADD R3, R3, R4		; mcb_addr = mcb_top + (addr - heap_top) / 16
		
		PUSH {R0}			; save the value of R0 for the next if statement
		MOV R0, R3			; let R0 be R3 (mcb_addr) to be used in _rfree input parameter
		BL _rfree			; branch to rfree
		POP {R0}
		CMP R0, #0			; check if the memory returned from freeing is 0
		BEQ kfree_end		; if its zero, return to the end
							; space was found, restore the pointer (ptr)
kfree_end	
		POP {R1-R12, LR}	; restore the register values and the link register from past subroutines
		BX LR				; branch to next subroutine

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Kernel Memory De-allocation
; void *_rfree( int mcb_addr )	
		EXPORT	_rfree
_rfree
		; complete your code
		; return value should be saved into r0
		; input variable is R0, so R0 = 
		; R3 -> mcb_contents = *(short*)&array[m2a(mcb_addr)];
		LDR R3, [R0]	; load mcb_contents from mcb_addr
		; R4 -> mcb_index = mcb_addr - mcb_top
		; LDR R4, [R0]   	; load mcb_addr (redundant and ruins calculation)
		LDR R5, =MCB_TOP ; load MCB_TOP
		SUB R4, R0, R5  ; mcb_index = mcb_addr - mcb_top
		; R5 -> mcb_disp = (mcb_contents /= 16)
		; first, divide mcb_contents
		LSR R3, #4	;	(mcb_contents /= 16) LSR is division by 2^4
		MOV R5, R3	; assign it afterwards
		; R6 -> my_size = (mcb_contents *= 16) 
		LSL R3, #4 ; (mcb_contents *= 16)
		MOV R6, R3	; assign it afterwards
		
		; // mcb_addr's used bit was cleared
		; *(short *)&array[ m2a( mcb_addr ) ] = mcb_contents; // deallocate here by clearing the used bit
		STRH R3, [R0]	; clear the used bit address 
		
		MOV R7, R4		; get mcb_index
		MOV R8, R5		; get mcb_disp
		SDIV R9, R7, R8	; mcb_index / mcb_disp
		; do modulo here
		RORS R9, #1		; rotate right to get a carry bit
		; MLS R9, R8, R9, R7
		; if ( ( mcb_index / mcb_disp ) % 2 == 0 ) generate the comparator here
		; CMP R9, #0
		BCC	lb_even_addr	; this address is even, currently on the left buddy, dealloc right buddy
		; else statement
		BCS rb_odd_addr		; this address is odd, currently on the right buddy, dealloc left buddy
		
		; working on left_buddy label
lb_even_addr ; 
		; if ( mcb_addr + mcb_disp >= mcb_bot )
		; check that we don't go out of bounds in MCB entry
		MOV R7, R0	; assign R7 to mcb_addr
		ADD R7, R7, R5 ; mcb_addr + mcb_disp		
		LDR R8, =MCB_BOT ; load MCB_BOT for comparison
		CMP R7, R8	; create the comparator
		MOVGE R0, #0 ; return 0, buddy is beyond mcb_bot
		BGE return_free_addr ; break out
		B dealloc_lb		; deallocate the left buddy
dealloc_lb ; else statement
		; R7 is mcb_addr + mcb_disp
		; short mcb_buddy = *(short *)&array[ m2a( mcb_addr + mcb_disp ) ];
		LDRH R8, [R7]	; allow R8 to read our buddy
		; if ( ( mcb_buddy & 0x0001 ) == 0 ) {
		AND R9, R8, #0x0001 ; generate the and statement
		CMP R9, #0 ; generate comparator
		BEQ even_dealloc_rb
		BNE return_free_addr
even_dealloc_rb
		; mcb_buddy = ( mcb_buddy / 32 ) * 32;
		LSR R8, #5 ; divide R8 by 2^5
		LSL R8, #5 ; multiply R8 by 2^5
		; if ( mcb_buddy == my_size ) { 
		; calculate the right side
		CMP R8, R6
		BEQ rb_buddy_valid_size
		BNE return_free_addr
rb_buddy_valid_size
		; *(short *)&array[ m2a( mcb_addr + mcb_disp ) ] = 0;
		MOV R9, #0  
		STRH R9, [R7]
		; coalesce here
		LSL R6, #1 ; my_size *= 2 -> R6 * 2^1
		; *(short *)&array[ m2a( mcb_addr ) ] = my_size;
		; set combined size by the following line above
		STRH R6 , [R0]
		PUSH {R0-R6, LR} ; push our values to stack and LR
		BL	_rfree		; run rfree(mcb_addr), value is unchanged
		POP {R0-R6, LR} ; pop out values
		B return_free_addr ; return	
		
		; working on right_buddy_label
rb_odd_addr
		MOV R7, R0		; assign R7 to R0
		; if ( mcb_addr - mcb_disp < mcb_top )
		; another boundaries check, but prevent it being less than MCB_TOP
		SUB R7, R7, R5 ; R5 is displacement
		LDR R8, =MCB_TOP; load MCB_TOP
		CMP R7, R8 ; compare
		MOVLT R0, #0 ; buddy is below mcb_top, return 0 (invalid)
		BLT return_free_addr
		B dealloc_rb	; else statement	
dealloc_rb
		; get mcb buddy, R7 is mcb_addr - mcb_disp
		LDRH R8, [R7] ; read mcb_buddy
		; if ( ( mcb_buddy & 0x0001 ) == 0 )
		AND R9, R8, #0x0001
		CMP R9, #0 ; check if our left buddy is free for coalescing
		BEQ odd_dealloc_lb
		BNE return_free_addr
odd_dealloc_lb
		; mcb_buddy = (mcb_buddy / 32) * 32
		LSR R8, #5 ; mcb_buddy / 32
		LSL R8, #5 ; mcb_buddy * 32
		; if (mcb_buddy == my_size)
		CMP R8, R6
		BEQ lb_buddy_valid_size
		BNE return_free_addr ; size isn't exact	
lb_buddy_valid_size
		MOV R9, #0 ; use this as a temp register to write zero
		STRH R9, [R0] ; clear right buddy memory contents
		; coalesce the memory contents
		LSL R6, #1	; multiply size by 2^1
		; r7 is mcb_addr - mcb_disp
		STRH  R6, [R7] ; write R7 to my_size
		PUSH {R0-R6, LR}
		; change R0
		MOV R0, R7	
		; return _rfree( mcb_addr - mcb_disp );
		; then branch to rfree
		BL _rfree
		POP {R0-R6, LR}
		; get out of it
		B return_free_addr
return_free_addr	
		; return mcb_addr
		; R0 is the mcb_addr returned
		
		BX		lr
		
		END

