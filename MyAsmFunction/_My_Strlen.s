;--------------------------------------------------------------------------------------------------
;                                  _My_Strlen.s
;                         (c) 2025 Muratov Artyom
;--------------------------------------------------------------------------------------------------

section     .text

global      _My_Strlen                                  ; predefine func for linker

;--------------------------------------------------------------------------------------------------
; _My_Strlen my function strlen for words with <= 30 symbols
; Entry:    rdi = string
; Exit:     rdi = string, rax = len of string
; Destroy:  rdx, rax
;--------------------------------------------------------------------------------------------------
_My_Strlen:
            xor  rax, rax                               ; rax = 0,  rax - index of symbol in string
NextSymbol:
            mov  dl,  [rdi + rax]                       ; rax = symbol from string
            inc  rax                                    ; rdx++

            cmp  rdx, 0x00                              ; if (rax != '\0') {
            jne  NextSymbol                             ; jmp NextSymbol }

            ret
;-----------End-_My_Strlen-----------------------------------------------------------------

section     .note.GNU-stack
