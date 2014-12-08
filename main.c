#include <stdio.h>
#include <stdint.h>
#include <sys/mman.h>

#include "test.h"

void func_NEW()
{
    printf("hallo NEW :)\n");
}

void patch_func(void *func, void *new_func)
{
    uint32_t *ptr = func;

    printf("func %p, new_func: %p\n", func, new_func);

    if((uint32_t)ptr & 1)
    {
        ptr = (uint32_t*)((char*)ptr - 1);
        uint16_t *tptr = (uint16_t*) ptr;
        tptr[0] = 0xB401; /* push {r0} */
        tptr[1] = 0xF8DF; /* ldr r0, [pc, #8] */
        tptr[2] = 0x0008; /* continuation of last instruction */
        tptr[3] = 0x4684; /* mov ip, r0 */
        tptr[4] = 0xBC01; /* pop {r0} */
        tptr[5] = 0x4760; /* bx ip */
        tptr[6] = ((uint32_t)new_func & 0x0000FFFF);
        tptr[7] = ((uint32_t)new_func >> 16);
    }
    else
    {
        ptr[0] = 0xE51ff004; /* ldr pc, [pc, -#4] */
        ptr[1] = (uint32_t)new_func;
    }
}

int main(int argc, char *argv[])
{
    printf("calling thumb function\n");
    functhumb();
    printf("calling arm function\n");
    funcarm();
    printf("calling done\n");

    void *page_thumb = (void*) ((uintptr_t) functhumb & (uintptr_t) ~ (4096-1));
    void *page_arm = (void*) ((uintptr_t) funcarm & (uintptr_t) ~ (4096-1));
    mprotect(page_thumb, 4096, PROT_WRITE | PROT_EXEC | PROT_READ);
    mprotect(page_arm, 4096, PROT_WRITE | PROT_EXEC| PROT_READ);

    printf("patching thumb function\n");
    patch_func(functhumb, func_NEW);
    printf("patching arm function\n");
    patch_func(funcarm, func_NEW);
    printf("patching done.\n");

    printf("calling thumb function\n");
    functhumb();
    printf("calling arm function\n");
    funcarm();
    printf("calling done\n");

    return 0;
}

