;--------------------------------------------------------------------------------------------------
;                                  _My_FindInListValue.s
;                                 (c) 2025 Muratov Artyom
;--------------------------------------------------------------------------------------------------

section     .text

global      _My_FindInListValue                         ; predefine func for linker

;--------------------------------------------------------------------------------------------------
; _My_FindInListValue my function FindInListValue optimized in assembly
; Entry:    rdi = pointer to first  string
;           rsi = pointer to second string
; Exit:     rax = result of comparing, rax = 0, if strings are equal, and rax != 0 else.
; Destroy:  rsi, rdx, rcx, rax
;--------------------------------------------------------------------------------------------------
_My_FindInListValue:




            ret
;-----------End-_My_FindInListValue-------------------------------------------------------------------------

section     .note.GNU-stack
