; atomic_test_and_set - using the native compare and exchange on the 
; Intel x86; returns 0 if we set, 1 if not (think: l == 1 => locked,
; and we return the old value, so we get 0 if we managed to lock l).

.686

.model FLAT,C
option casemap :none

EXTERN interrupt_level:DWORD

.code

atomic_test_and_set PROC l:DWORD

    mov edx, dword ptr [l]   ; Get the pointer to l

    mov ecx, 1                  ; load 1 into the cmpxchg source
    mov eax, 0                  ; load 0 into the accumulator

                                ; if l == 0 then
    lock cmpxchg dword ptr [edx], ecx ; l = 1 (and eax = 0)
                                ; else
                                ; (l = 1 and) eax = 1
    mov esp, ebp
    pop ebp
    retn

atomic_test_and_set ENDP

; #############################################################################

; swap
; 
; atomically stores newval in *x, returns old value in *x

swap PROC x:DWORD, newval:DWORD

    mov eax, newval
    mov edx, dword ptr [x]

    lock xchg dword ptr [edx], eax

    mov esp, ebp
    pop ebp
    retn

swap ENDP

; #############################################################################

; compare and swap
; 
; compare the value at *x to oldval, swap with
; newval if successful

compare_and_swap PROC x:DWORD, oldval:DWORD, newval:DWORD

    mov eax, oldval
    mov ecx, newval
    mov edx, dword ptr [x]

    lock cmpxchg dword ptr [edx], ecx

    mov esp, ebp
    pop ebp
    retn

compare_and_swap ENDP

; #############################################################################

; minithread_root

minithread_root PROC

    push edi    ; push the arg to the main proc
    call ebx    ; call main proc

    push ebp    ; push the arg to the clean-up proc
    call esi    ; call the clean-up

minithread_root ENDP

; #############################################################################

; minithread_switch - on the intel x86

minithread_switch PROC old_thread_sp_ptr:DWORD, new_thread_sp_ptr:DWORD

    mov ecx, old_thread_sp_ptr  ; Get these before we clobber
    mov eax, dword ptr [new_thread_sp_ptr]  ; the ebp register.

    ; pop edi	   ; compiler tries to save some state which we
    ; pop esi      ; don't want it to.
    ; pop ebx      ;
    mov esp, ebp ; Brings sp to where the old bp is stored. 
    pop ebp      ;

    push ebp	   ; Save the ebp, esi, edi, and ebx on the stack
    push esi     ;
    push edi     ;
    push ebx     ;

    mov dword ptr [ecx], esp  ; pass back the old thead's sp

    mov esp, dword ptr [eax]  ; deref. the pointer and load new thread's sp

    mov dword ptr ds:[interrupt_level], 1 ; re-enable interrupts

    pop ebx		; Get the ebp, esi, edi, and ebx off the stack
    pop edi		;
    pop esi		;
    pop ebp		;

    retn

minithread_switch ENDP

END
