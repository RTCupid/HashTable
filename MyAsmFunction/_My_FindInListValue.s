;--------------------------------------------------------------------------------------------------
;                                  _My_FindInListValue.s
;                                 (c) 2025 Muratov Artyom
;--------------------------------------------------------------------------------------------------

section     .text

global      _My_FindInListValue                             ; predefine func for linker

;--------------------------------------------------------------------------------------------------
; _My_FindInListValue my function FindInListValue optimized in assembly
;
; Entry:    rdi  = pointer to list_t List (list_t*)
;------------------------------------------------------------
;           typedef __m128i my_key_t;                       |
;                                                           |
;           typedef struct list_t                           |
;           {                                               |
;               int         free;                           |
;               int*        next;                           |
;               int*        prev;                           |
;               my_key_t*   data;                           |
;           } list_t;                                       |
;------------------------------------------------------------
;           rsi  = pointer to my_key_t key       (__m128i*)
;           rdx  = pointer to status             (int*)
;
; Exit:     rax  = index of found key, or rax = 0, if don't found.
;
; Destroy:  rsi, rcx, rax, rdx
;--------------------------------------------------------------------------------------------------
_My_FindInListValue:
            mov  r8,  qword [rdi + 8]                       ; r8     = List->next, 8 - offset of next
            mov  eax, dword [r8]                            ; index  = List->next[0]

            test eax, eax                                   ; if (index == 0)
                                                            ; {
            je   .End_My_FindInListValue_return_0           ;       goto .End_My_FindInListValue_return_0;
                                                            ; }
            mov dword [rdx], 1                              ; status = 1

            mov  rdi, qword [rdi + 0x18]                    ; rdx = List->data, 0x18 - offset of data

            movaps  xmm1, [rsi]                             ; xmm0 = key

            jmp  .While_loop

.While_test:                                                 ; while (1)
                                                            ; {
;--------------------test-------------------
            mov  eax, dword [r8 + rdx * 4]                  ;   index = List->next[index]

            test eax, eax                                   ;   if (index == 0)
                                                            ;   {
            je  .End_My_FindInListValue_return_0            ;       goto .End_My_FindInListValue_return_0;
                                                            ;   }
;--------------------loop-------------------
.While_loop:

            movsxd rdx, eax                                 ;   rdx  = eax
            mov    rcx, rdx                                 ;   rcx  = rdx
            shl rcx, 4                                      ;   rcx *= 16, offset to read from List->data[index]

            vpcmpeqd  xmm0, xmm1, [rdi + rcx]               ;   __m128i result_of_compare = _mm_cmpeq_epi32  (List->data[index], key);

            vpmovmskb ecx,  xmm0                            ;   uint32_t mask = (uint32_t) _mm_movemask_epi8 (result_of_compare);

            cmp  ecx, 0xffff                                ;   if (result_of_compare != COMPARE_M128_MASK)
                                                            ;   {
            jne   .While_test                                ;       goto .While_test ;
                                                            ;   }
                                                            ;   else
            ret                                             ;       return index
                                                            ; }
.End_My_FindInListValue_return_0:

            xor rax, rax                                    ; index = 0;

            ret                                             ; return index;

;-----------End-_My_FindInListValue-------------------------------------------------------------------------

section     .note.GNU-stack
