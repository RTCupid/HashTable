;--------------------------------------------------------------------------------------------------
;                                  _My_FindInListValue.s
;                                 (c) 2025 Muratov Artyom
;--------------------------------------------------------------------------------------------------

section     .text

global      _My_FindInListValue                             ; predefine func for linker

;--------------------------------------------------------------------------------------------------
; _My_FindInListValue my function FindInListValue optimized in assembly
;
; Entry:    rdi  = pointer to struct list_t List (list_t*)
;           rsi  = pointer to my_key_t key       (__m128i*)
;           rdx  = pointer to status             (int*)
;
; Exit:     rax  = index of found key, or rax = 0, if don't found.
;
; Destroy:  rsi, rcx, rax, rdx
;--------------------------------------------------------------------------------------------------
_My_FindInListValue:
            xor  rax, rax                                   ; rax = 0, register for index

            mov  eax, dword [rdi + offset_int_ptr_next]     ; index  = List->next[0]

            test eax, eax                                   ; if (index == 0)
                                                            ; {
            je   End_My_FindInListValue                     ;       goto End_My_FindInListValue;
                                                            ; }
            mov dword [rdx], true                           ; status = 1

            movaps  xmm0, [rsi]                             ; xmm0 = key

            lea  rdx, [rdi + offset_my_key_t_ptr_data]      ; rdx = List->data
            lea  r8,  [rdi + offset_int_ptr_next]           ; r8  = List->next

            jmp  While_true

While_test:                                                 ; while (1)
                                                            ; {

            mov  eax, dword [r8 + rax * 8]                  ;   index = List->next[index]

            test eax, eax                                   ;   if (index == 0)
                                                            ;   {
            je  End_My_FindInListValue_return_0             ;       goto End_My_FindInListValue_return_0;
                                                            ;   }
While_true:
            mov r9, rax                                     ;   r9  = rax
            shl r9, 4                                       ;   r9 *= 16, offset to read from List->data[index]

            vpcmpeqd  xmm1, xmm0, [rdx + r9 * 1]            ;   __m128i result_of_compare = _mm_cmpeq_epi32   (List->data[index], key);

            vpmovmskb ecx,  xmm1                            ;   uint32_t mask = (uint32_t) _mm_movemask_epi8 (result_of_compare);

            cmp  ecx, COMPARE_M128_MASK                     ;   if (result_of_compare == COMPARE_M128_MASK)
                                                            ;   {
            je   End_My_FindInListValue                     ;       goto End_My_FindInListValue;
                                                            ;   }
            jmp While_true                                  ; }

End_My_FindInListValue_return_0:

            xor rax, rax                                    ; index = 0;

End_My_FindInListValue:

            ret                                             ; return index;

;-----------End-_My_FindInListValue-------------------------------------------------------------------------

section     .data

true:                       equ 1                           ; true = 1

offset_int_ptr_next:        equ 4                           ; offset of int*next
                                                            ; in struct list_t = 4

offset_my_key_t_ptr_data:   equ 20                          ; offset of my_key_t* data
                                                            ; in struct list_t = 20
COMPARE_M128_MASK:          equ 0x0000FFFF

section     .note.GNU-stack
