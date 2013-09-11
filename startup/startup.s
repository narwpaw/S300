;-----------------------------------------------------------------------------
; This file contains the startup code used by the ICCARM C compiler.
;
; The modules in this file are included in the libraries, and may be replaced
; by any user-defined modules that define the PUBLIC symbol _program_start or
; a user defined start symbol.
; To override the cstartup defined in the library, simply add your modified
; version to the workbench project.
;
; All code in the modules (except ?RESET) will be placed in the ICODE segment.
;
; $Revision: 1.1.2.1 $
;
;-----------------------------------------------------------------------------

;
; Naming covention of labels in this file:
;
;  ?xxx   - External labels only accessed from assembler.
;  __xxx  - External labels accessed from or defined in C.
;  xxx    - Labels local to one module (note: this file contains
;           several modules).
;  main   - The starting point of the user program.
;
;---------------------------------------------------------------
; Macros and definitions for the whole file
;---------------------------------------------------------------
;------------------------------------------------------------------------------
;       Includes
;------------------------------------------------------------------------------
        INCLUDE config.h

    #if  AT91SAM7S321
        INCLUDE AT91SAM7S321_inc.h
    #endif
    #if  AT91SAM7S64
        INCLUDE AT91SAM7S64_inc.h
    #endif
    #if  AT91SAM7S128
        INCLUDE AT91SAM7S128_inc.h
    #endif
    #if  AT91SAM7S256
        INCLUDE AT91SAM7S256_inc.h
    #endif
    #if  AT91SAM7S512
        INCLUDE AT91SAM7S512_inc.h
    #endif
    #if  AT91SAM7SE32
        INCLUDE AT91SAM7SE32_inc.h
    #endif
    #if  AT91SAM7SE256
        INCLUDE AT91SAM7SE256_inc.h
    #endif
    #if  AT91SAM7SE512
        INCLUDE AT91SAM7SE512_inc.h
    #endif
    #if  AT91SAM7X128
        INCLUDE AT91SAM7X128_inc.h
    #endif
    #if  AT91SAM7X256
        INCLUDE AT91SAM7X256_inc.h
    #endif
    #if  AT91SAM7X512
        INCLUDE AT91SAM7X512_inc.h
    #endif
    #if  AT91SAM7A3
        INCLUDE AT91SAM7A3_inc.h
    #endif
    #if  AT91RM9200
        INCLUDE AT91RM9200_inc.h
    #endif
    #if  AT91SAM9260
        INCLUDE AT91SAM9260_inc.h
    #endif
    #if  AT91SAM9261
        INCLUDE AT91SAM9261_inc.h
    #endif
    #if  AT91SAM9263
        INCLUDE AT91SAM9263_inc.h
    #endif
    #if  AT91SAM9265
        INCLUDE AT91SAM9265_inc.h
    #endif
    #if  AT91SAM926C
        INCLUDE AT91SAM926C_inc.h
    #endif

;-------------------------------------------------------------------------------
;       Constants
;-------------------------------------------------------------------------------
; Mode, correspords to bits 0-5 in CPSR
MODE_BITS DEFINE  0x1F    ; Bit mask for mode bits in CPSR
USR_MODE  DEFINE  0x10    ; User mode
FIQ_MODE  DEFINE  0x11    ; Fast Interrupt Request mode
IRQ_MODE  DEFINE  0x12    ; Interrupt Request mode
SVC_MODE  DEFINE  0x13    ; Supervisor mode
ABT_MODE  DEFINE  0x17    ; Abort mode
UND_MODE  DEFINE  0x1B    ; Undefined Instruction mode
SYS_MODE  DEFINE  0x1F    ; System mode

;-- Status register bits
I_BIT     DEFINE  0x80
F_BIT     DEFINE  0x40

;---------------------------------------------------------------
; ?RESET
; Reset Vector.
; Normally, segment INTVEC is linked at address 0.
; For debugging purposes, INTVEC may be placed at other
; addresses.
; A debugger that honors the entry point will start the
; program in a normal way even if INTVEC is not at address 0.
;---------------------------------------------------------------
#ifdef AT91SAM9261
        SECTION PROGRAM_DATA:CODE:ROOT(2)
#endif // AT91SAM9261
        SECTION .intvec:CODE:NOROOT(2)
        PUBLIC  __vector
        PUBLIC  __iar_program_start

        ARM
__vector:
        ldr  pc,[pc,#+24]             ;; Reset
__und_handler:
        ldr  pc,[pc,#+24]             ;; Undefined instructions
__swi_handler:
        ldr  pc,[pc,#+24]             ;; Software interrupt (SWI/SVC)
__prefetch_handler:
        ldr  pc,[pc,#+24]             ;; Prefetch abort
__data_handler:
        ldr  pc,[pc,#+24]             ;; Data abort
#ifdef AT91SAM9261
        DC32  SFE(PROGRAM_DATA)
#else
        DC32  0xFFFFFFFF              ;; RESERVED
#endif // AT91SAM9261
__irq_handler:
        ldr  pc,[pc,#+24]             ;; IRQ
__fiq_handler:
        ldr  pc,[pc,#+24]             ;; FIQ

        DC32  __iar_program_start
        DC32  __und_handler
        DC32  __swi_handler
        DC32  __prefetch_handler
        DC32  __data_handler
        B .
        DC32  ?irq_handler
        DC32  __fiq_handler


;------------------------------------------------------------------------------
; ?INIT
; Program entry.
;------------------------------------------------------------------------------

    SECTION FIQ_STACK:DATA:NOROOT(3)
    SECTION IRQ_STACK:DATA:NOROOT(3)
    SECTION SVC_STACK:DATA:NOROOT(3)
    SECTION ABT_STACK:DATA:NOROOT(3)
    SECTION UND_STACK:DATA:NOROOT(3)
    SECTION CSTACK:DATA:NOROOT(3)
    SECTION text:CODE:NOROOT(2)
    REQUIRE __vector
    EXTERN  ?main
    PUBLIC  __iar_program_start

__iar_program_start:

;------------------------------------------------------------------------------
;- Low level Init is performed in a C function: AT91F_LowLevelInit
;- Init Stack Pointer to a valid memory area before calling AT91F_LowLevelInit
;------------------------------------------------------------------------------

; Initialize the stack pointers.
; The pattern below can be used for any of the exception stacks:
; FIQ, IRQ, SVC, ABT, UND, SYS.
; The USR mode uses the same stack as SYS.
; The stack segments must be defined in the linker command file,
; and be declared above.

                mrs     r0,cpsr                             ; Original PSR value
                bic     r0,r0,#MODE_BITS                    ; Clear the mode bits
                orr     r0,r0,#SVC_MODE                     ; Set SVC mode bits
                msr     cpsr_c,r0                           ; Change the mode
                ldr     sp,=SFE(SVC_STACK)                  ; End of SVC_STACK

                bic     r0,r0,#MODE_BITS                    ; Clear the mode bits
                orr     r0,r0,#UND_MODE                     ; Set UND mode bits
                msr     cpsr_c,r0                           ; Change the mode
                ldr     sp,=SFE(UND_STACK)                  ; End of UND_STACK

                bic     r0,r0,#MODE_BITS                    ; Clear the mode bits
                orr     r0,r0,#ABT_MODE                     ; Set ABT mode bits
                msr     cpsr_c,r0                           ; Change the mode
                ldr     sp,=SFE(ABT_STACK)                  ; End of ABT_STACK

                bic     r0,r0,#MODE_BITS                    ; Clear the mode bits  
                orr     r0,r0,#FIQ_MODE                     ; Set FIQ mode bits
                msr     cpsr_c,r0                           ; Change the mode
                ldr     sp,=SFE(FIQ_STACK)                  ; End of FIQ_STACK

                bic     r0,r0,#MODE_BITS                    ; Clear the mode bits
                orr     r0,r0,#IRQ_MODE                     ; Set IRQ mode bits
                msr     cpsr_c,r0                           ; Change the mode
                ldr     sp,=SFE(IRQ_STACK)                  ; End of IRQ_STACK

                bic     r0,r0,#MODE_BITS                    ; Clear the mode bits
                orr     r0,r0,#SYS_MODE                     ; Set System mode bits
                msr     cpsr_c,r0                           ; Change the mode
                ldr     sp,=SFE(CSTACK)                     ; End of CSTACK

#ifdef __ARMVFP__
; Enable the VFP coprocessor.
                mov     r0, #0x40000000                 ; Set EN bit in VFP
                fmxr    fpexc, r0                       ; FPEXC, clear others.

; Disable underflow exceptions by setting flush to zero mode.
; For full IEEE 754 underflow compliance this code should be removed
; and the appropriate exception handler installed.
                mov     r0, #0x01000000           ; Set FZ bit in VFP
                fmxr    fpscr, r0                       ; FPSCR, clear others.
#endif



; Add more initialization here


; Continue to ?main for more IAR specific system startup

                ldr     r0,=?main
                bx      r0

;---------------------------------------------------------------
; ?IRQ_HANDLER
;---------------------------------------------------------------
?irq_handler

;---- Adjust and save return address on the stack
    sub     lr, lr, #4
    stmfd   sp!, {lr}

;---- Save r0 and SPSR on the stack
    mrs     r14, SPSR
    stmfd   sp!, {r0, r14}

;---- Write in the IVR to support Protect mode
;---- No effect in Normal Mode
;---- De-assert NIRQ and clear the source in Protect mode
    ldr     r14, =AT91C_BASE_AIC
    ldr     r0, [r14, #AIC_IVR]
    str     r14, [r14, #AIC_IVR]

;---- Enable nested interrupts and switch to Supervisor mode
    msr     CPSR_c, #SYS_MODE

;---- Save scratch/used registers and LR on the stack
    stmfd   sp!, {r1-r3, r12, r14}

;---- Branch to the routine pointed by AIC_IVR
    mov     r14, pc
    bx      r0

;---- Restore scratch/used registers and LR from the stack
    ldmia   sp!, {r1-r3, r12, r14}

;---- Disable nested interrupts and switch back to IRQ mode
    msr     CPSR_c, #I_BIT | IRQ_MODE

;---- Acknowledge interrupt by writing AIC_EOICR
    ldr     r14, =AT91C_BASE_AIC
    str     r14, [r14, #AIC_EOICR]

;---- Restore SPSR and r0 from the stack
    ldmia   sp!, {r0, r14}
    msr     SPSR_cxsf, r14

;---- Return from interrupt handler
    ldmia   sp!, {pc}^

    END         ;- Terminates the assembly of the last module in a file
