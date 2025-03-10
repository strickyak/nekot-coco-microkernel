#include "nekot1/private.h"

// main.c

// Use our alternate data sections.
gword _More0 gZEROED; // not .bss
gword _More1 gZEROED = 0x9998; // not .data
gword _Final __attribute__ ((section (".final"))) = 0x9990;
gword _Final_Startup __attribute__ ((section (".final.startup"))) = 0x9991;

#if 0
// This is a "Null Game" that does nothing.
void CWait(void) {
    gEnableIrq();

    while (1) {
        // wait for any interrupt.
        asm volatile ("cwai #$00");
        SpinCWait();
    }
}
#endif

// pia_reset table traced from coco3 startup.
struct pia_reset { gword addr; gbyte value; } pia_reset[] gSTARTUP_DATA = {
     { 0xff21, 0x00 },  // choose data direction
     { 0xff23, 0x00 },  // choose data direction
     { 0xff20, 0xfe },  // bit 0 input; rest are outputs.
     { 0xff22, 0xfa },  // bit 1 and bits 3-7 are outputs.
     { 0xff21, 0x34 },
     { 0xff23, 0x34 },
     { 0xff22, 0x00 },  // output all 0s on Pia1 PortB
     { 0xff20, 0x02 },
     { 0xff01, 0x00 },  // choose data direction
     { 0xff03, 0x00 },  // choose data direction
     { 0xff00, 0x00 },  // inputs
     { 0xff02, 0xff },  // outputs
     { 0xff01, 0x34 },
     { 0xff03, 0x34 },
     { 0 }
};

void after_main() {
    // Wipe out the startup code, to prove it is never needed again.
    for (gbyte* p = sizeof(gword) + (gbyte*)&_Final;
         p < (gbyte*)_Final_Startup;
         p++) {
        *p = 0;
    }

    gEnableIrq();
    StartTask((gword)ChatTask); // Start the no-game task.
}

void ClearPage256(gword p) {
    for (gword i = 0; i<256; i+=2) {
        gPoke2(p+i, 0);
    }
}

void before_main() {
    asm volatile("\n"
        "  .globl entry \n"
        "entry:         \n"
        "  orcc #$50    \n"  // No IRQs, FIRQs, for now.
        "  lds #$01FE   \n"  // Reset the stack
        "  jmp _main    \n"
        );
}

gword PinDown[] gSTARTUP_DATA = {
    (gword) after_main,

    (gword) Breakkey_Handler,
    (gword) Irq_Handler,
    (gword) Irq_Handler_entry,
    (gword) Irq_Handler_Wrapper,
    (gword) Network_Handler,

    (gword) gTextScreen,
    (gword) gPMode1Screen,
    (gword) gModeScreen,
    (gword) xAfterMain3,
    (gword) gNetworkLog,
    (gword) gFatal,
    // (gword) Console_Printf,
    (gword) PutStr,
    (gword) PutChar,

    (gword) before_main,
    (gword) &_More0,
    (gword) &_More1,
    (gword) &_Final,
    (gword) &_Final_Startup,
};

#if 0
void TestByte(gbyte a) {
    asm volatile("  LDA %0" : : "m" (a));
}

void TestWord(gword foo) {
    gwob x = { .w= foo };
    TestByte(x.b[0]);
    TestByte(x.b[1]);
}
#endif

int main() {
#if 0
    gPin(TestByte);
    gPin(TestWord);
    TestWord(0x1234);
    TestWord(0x5678);
#endif
    ClearPage256(0x0000); // .bss
    // ClearPage256(0x0100); // stack
    ClearPage256(0x0200); // vdg console p1
    ClearPage256(0x0300); // vdg console p2
    ClearPage256(0x0400); // chunks of 64-gbyte

    // Coco3 in Compatibility Mode.
    gPoke1(0xFF90, 0x80);
    gPoke1(0xFF91, 0x00);

    // Install 4 initial 64-gbyte chunks.
    Reset64();
    for (gbyte* p = (gbyte*)0x0400; p < (gbyte*)0x0500; p += 64) {
        gFree64(p);
    }

    Kern_Init();

    // Set the IRQ vector code, for Coco 1 or 2.
    gPoke1(IRQVEC_COCO12, JMP_Extended);
    //-- gPoke2(IRQVEC_COCO12+1, Irq_Handler_Wrapper);
    gPoke2(IRQVEC_COCO12+1, Irq_Handler_entry);

    // Set the IRQ vector code, for Coco 3.
    gPoke1(IRQVEC_COCO3, JMP_Extended);
    //-- gPoke2(IRQVEC_COCO3+1, Irq_Handler_Wrapper);
    gPoke2(IRQVEC_COCO3+1, Irq_Handler_entry);

    Console_Init();
    for (struct pia_reset *p = pia_reset; p->addr; p++) {
        gPoke1(p->addr, p->value);
    }
    Vdg_Init();


    PutStr("\nNEKOT MICROKERNEL\n");
    Spin_Init();

    gPeek1(0xFF02);        // Clear VSYNC IRQ
    gPoke1(0xFF03, 0x35);  // +1: Enable VSYNC (FS) IRQ
    gPeek1(0xFF02);        // Clear VSYNC IRQ

    Network_Init();
    HelloMCP();

    // ================================
    after_main();
    gPin(PinDown);
    gFatal("EXIT", 0);
}
