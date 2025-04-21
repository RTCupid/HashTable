;--------------------------------------------------------------------------------------------------
;                                  _My_Strcmp.s
;                         (c) 2025 Muratov Artyom
;--------------------------------------------------------------------------------------------------

section     .text

global      _My_Strcmp                                  ; predefine func for linker

;--------------------------------------------------------------------------------------------------
; _My_Strcmp my function strcmp for compare strings with len <= 32 bytes
; Entry:    rdi = pointer to first  string
;           rsi = pointer to second string
; Exit:     rax = result of comparing, rax = 0, if strings are equal, and rax != 0 else.
; Destroy:  rsi, rdx, rcx, rax
;--------------------------------------------------------------------------------------------------
_My_Strcmp:
            movaps ymm0, [rdi]
            movaps ymm1, [rsi]

            vpcmpeqb ymm0, ymm1

            ret
;-----------End-_My_Strcmp-----------------------------------------------------------------

section     .note.GNU-stack
