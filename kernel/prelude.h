#ifndef _PRELUDE_H_
#define _PRELUDE_H_

typedef unsigned char bool;
typedef unsigned char byte;
typedef unsigned int uint;
typedef unsigned int word;
typedef unsigned int size_t;
typedef void (*func)(void);
typedef union wordorbytes {
    word w;
    byte b[2];
} wob;

#define true ((bool)1)
#define false ((bool)0)

#define Peek1(ADDR) (*(volatile byte*)(uint)(ADDR))
#define Poke1(ADDR,VALUE) (*(volatile byte*)(uint)(ADDR) = (byte)(VALUE))

#define Peek2(ADDR) (*(volatile uint*)(uint)(ADDR))
#define Poke2(ADDR,VALUE) (*(volatile uint*)(uint)(ADDR) = (uint)(VALUE))

#define PAND(ADDR, X) ((*(volatile byte*)(size_t)(ADDR)) &= (byte)(X))
#define POR(ADDR, X) ((*(volatile byte*)(size_t)(ADDR)) |= (byte)(X))
#define PXOR(ADDR, X) ((*(volatile byte*)(size_t)(ADDR)) ^= (byte)(X))

#define Cons ((volatile byte*)0x0200)
#define Disp ((volatile byte*)0x0400)
#define InitialStack 0x01FE // going backwards.  2-byte canary after stack.
#define KernelBegin 0x1002  // two byte canary before code.
#define KernelEntry 0x1002

const uint Pia0PortA = 0xFF00;
const uint Pia0PortB = 0xFF02;

const uint IRQVEC_COCO12 = 0x010C;
const uint IRQVEC_COCO3 = 0xFEF7;
const byte JMP_Extended = 0x7E;

void memcpy(byte *dest, byte *src, uint count);

void Fatal(const char* s, uint arg);

#define INHIBIT_IRQ() asm volatile("  orcc #$10")
#define ALLOW_IRQ()   asm volatile("  andcc #^$10")

#endif // _PRELUDE_H_
