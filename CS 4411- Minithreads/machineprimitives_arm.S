	TTL	arm_md.c

	AREA	|.drectve|, DRECTVE
	DCB	"-defaultlib:coredll.lib "
	DCB	"-defaultlib:corelibc.lib "



	EXPORT	|currentTimeMillis|
	IMPORT	|GetTickCount|

	AREA	|.pdata|, PDATA
|$T201|	DCD	|currentTimeMillis|
	DCD	0x40000401

	AREA	|.text|, CODE

;=============================================
|currentTimeMillis| PROC
;---------------------------------------------
	stmdb     sp!, {lr}  ; stmfd
	bl        GetTickCount  ; 0000000C
	mov       r1, r0, asr #31
	ldmia     sp!, {pc}  ; ldmfd
;---------------------------------------------
	ENDP  ; |currentTimeMillis|
;=============================================



	EXPORT	|atomic_test_and_set|

	AREA	|.pdata|, PDATA
|$T205|	DCD	|atomic_test_and_set|
	DCD	0x40000200

	AREA	|.text|, CODE

;=============================================
|atomic_test_and_set| PROC
;---------------------------------------------
	mov       r1, #1
	swp       r1, r1, [r0]
	mov       r0, r1
	mov       pc, lr
;---------------------------------------------
	ENDP  ; |atomic_test_and_set|
;=============================================



	EXPORT	|swap|

	AREA	|.pdata|, PDATA
|$T209|	DCD	|swap|
	DCD	0x40000200

	AREA	|.text|, CODE

;=============================================
|swap|	PROC
;---------------------------------------------
	swp       r1, r1, [r0]
	mov       r0, r1
	mov       pc, lr
;---------------------------------------------
	ENDP  ; |swap|
;=============================================



	EXPORT	|compare_and_swap|

	AREA	|.pdata|, PDATA
|$T213|	DCD	|compare_and_swap|
	DCD	0x40000200

	AREA	|.text|, CODE

;=============================================
|compare_and_swap| PROC
;---------------------------------------------
	; r0: int* p, r1: oldval, r2: newval
|l0|
    ldr    r3, [r0]
    mov    r4, #0
    cmp    r3, r1
    bne    |l1|
    swp    r4, r2, [r0]
    cmp    r3, r4
    swpne  r3, r4, [r0]
    bne    |l0|
    mov    r4, #1
|l1|
	mov    r0, r4
    mov    pc, lr
;---------------------------------------------
	ENDP  ; |compare_and_swap|
;=============================================



	EXPORT	|atomic_clear|

	AREA	|.pdata|, PDATA
|$T217|	DCD	|atomic_clear|
	DCD	0x40000100

	AREA	|.text|, CODE

;=============================================
|atomic_clear| PROC
;---------------------------------------------
	mov       r3, #0
	str       r3, [r0]
	mov       pc, lr
;---------------------------------------------
	ENDP  ; |atomic_clear|
;=============================================



	EXPORT	|minithread_root|

	AREA	|.pdata|, PDATA
|$T221|	DCD	|minithread_root|
	DCD	0x40000200

	AREA	|.text|, CODE

;=============================================
|minithread_root| PROC
;---------------------------------------------
	;-- call the main_proc --
    mov   r0, v2
    mov   lr, pc
	mov   pc, v1

	;-- call the finally_proc --
	mov   r0, v4
	mov   lr, pc
	mov   pc, v3
;---------------------------------------------
	ENDP  ; |minithread_root|
;=============================================



	EXPORT	|minithread_switch|
	IMPORT	|interrupt_level|

	AREA	|.pdata|, PDATA
|$T225|	DCD	|minithread_switch|
	DCD	0x40000100

	AREA	|.text|, CODE

;=============================================
|minithread_switch| PROC
;---------------------------------------------
	; r0 : pointer to the old_sp
	; r1 : pointer to the new_sp

	;-- save old context --
    stmfd    sp!, {v1-v6, sl, fp, lr}

	; pass back the old thread's sp
	str      sp, [r0]

	; dereference the ptr and load the new thread's sp
	ldr		 r3, [r1]

	;-- restore new context --
	ldmfd    r3!, {v1-v6, sl, fp, lr}

    ;-- hack -- (to fix SP) --
    mov      r0, #0xFF, 8
	and      r1, r0, sp
	mvn      r0, r0
	and      r3, r0, r3
	orr      r3, r3, r1

	;-- put the corrected sp in place
	mov		 sp, r3

	;-- restore interrupts --
	mov      r1, #1
	ldr      r0, [pc, #0x4]
	str      r1, [r0]

	;-- return --
	mov      pc, lr

	;-- pointer to global int interrupt_level --
	DCD      |interrupt_level|
;---------------------------------------------
	ENDP  ; |minithread_switch|
;=============================================

	END
