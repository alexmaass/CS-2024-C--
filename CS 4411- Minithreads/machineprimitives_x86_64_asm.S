; atomic_test_and_set - using the native compare and exchange on the 
; Intel x86; returns 0 if we set, 1 if not (think: l == 1 => locked,
; and we return the old value, so we get 0 if we managed to lock l).

option casemap :none

EXTERN interrupt_level:DWORD
.code

atomic_test_and_set PROC ; l:PTR

    ; We get l in rcx.

    mov rdx, rcx   ; Get the pointer to l

    ; operate on rax here to also zero the upper half
    mov rcx, 1                  ; load 1 into the cmpxchg source
    mov rax, 0                  ; load 0 into the accumulator

                                ; if l == 0 then
    ; we operate on e*x which should be the lower half of r*x
    lock cmpxchg dword ptr [rdx], ecx ; l = 1 (and eax = 0)
                                ; else
                                ; (l = 1 and) eax = 1
    ret

atomic_test_and_set ENDP

; #############################################################################

; swap
; 
; atomically stores newval in *x, returns old value in *x

swap PROC
    ; We get rcx as our first arg (x) and rdx as our second (newval)
    mov rax, rdx
    lock xchg dword ptr [rcx], eax

    ret

swap ENDP

; #############################################################################

; compare and swap
; 
; compare the value at *x to oldval, swap with
; newval if successful

compare_and_swap PROC x:PTR, oldval:DWORD, newval:DWORD
    ; we get x = rcx
    ;        oldval = rdx
    ;        neval = r8

    mov eax, edx
    mov rdx, rcx
    mov rcx, r8

    lock cmpxchg dword ptr [rdx], ecx

    ret

compare_and_swap ENDP

; #############################################################################

; minithread_root

minithread_root PROC

    sub rsp, 32;
    mov rcx, rdi;
    call rbx    ; call main proc

    mov rcx, rbp;
    call rsi    ; call the clean-up
    sub rsp, 32;

minithread_root ENDP

; #############################################################################

; minithread_switch - on the intel x86

;minithread_switch PROC old_thread_sp_ptr:PTR, new_thread_sp_ptr:PTR
minithread_switch PROC

    ; uncomment this to get a breakpoint on context switch
    ; int 3

    ;x64 calling conventions effectively do this for us.
    ;mov rcx, old_thread_sp_ptr  ; Get these before we clobber
    ;mov rdx, new_thread_sp_ptr  ; the ebp register.
    mov rax, rdx

    ; No longer necessary.
    ; pop edi	   ; compiler tries to save some state which we
    ; pop esi      ; don't want it to.
    ; pop ebx      ;
    ; mov rsp, rbp ; Brings sp to where the old bp is stored. 
    ; pop rbp      ;

    push rbp	   ; Save the ebp, esi, edi, and ebx on the stack
    push rsi     ;
    push rdi     ;
    push rbx     ;

    mov qword ptr [rcx], rsp  ; pass back the old thead's sp

    mov rsp, qword ptr [rax]  ; deref. the pointer and load new thread's sp

    mov dword ptr [interrupt_level], 1 ; re-enable interrupts

    pop rbx		; Get the ebp, esi, edi, and ebx off the stack
    pop rdi		;
    pop rsi		;
    pop rbp		;

    ret;

minithread_switch ENDP

END
