#include <zephyr/toolchain.h>

_ASM_FILE_PROLOGUE

.syntax unified
GTEXT( __aeabi_memclr8)

SECTION_FUNC(TEXT, __aeabi_memclr8)
SECTION_FUNC(TEXT, __aeabi_memclr4)
SECTION_FUNC(TEXT, __aeabi_memclr)

     movs  r2, #0
SECTION_FUNC(TEXT, __aeabi_memset8)
SECTION_FUNC(TEXT, __aeabi_memset4)
SECTION_FUNC(TEXT, __aeabi_memset)
     cmp   r1, #0
     beq   2f
     adds  r1, r0, r1
 1:  strb  r2, [r0]
     adds  r0, r0, #1
     cmp   r1, r0
     bne   1b
 2:  bx    lr
