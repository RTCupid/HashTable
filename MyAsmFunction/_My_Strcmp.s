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
            vlddqu     ymm0, [rdi]                     ; ymm0 = first string
            vlddqu     ymm1, [rsi]                     ; ymm1 = second string

            vpcmpeqb    ymm0, ymm1                      ; compare first and second strings as bytes

            vpmovmskb   eax,  ymm0                      ; move int mask, if (first_string[i] ==
                                                        ; second_string[i]) mask[i] = 1
                                                        ; else              mask[i] = 0
            not         eax                             ; if (eax == 0xFFFFFFFF) eax =  0
                                                        ; else                   eax != 0
            ret
;-----------End-_My_Strcmp-------------------------------------------------------------------------

section     .note.GNU-stack
