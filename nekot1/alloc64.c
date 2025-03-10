#include "nekot1/private.h"

#define MAGIC64 0x1EE1

typedef struct header64 {
    gword magic;
    struct header64* next;
} H64;

H64* root64;

gbyte* gAlloc64() {
    H64* z = gNULL;
    gbyte cc_value = gIrqSaveAndDisable();

    if (root64) {
        gAssert(root64->magic == MAGIC64);
        z = root64;
        root64 = root64->next;
        z->magic = 0;
    }

    gIrqRestore(cc_value);
    return (gbyte*)z;
}

void gFree64(gbyte* p) {
    if (!p) return;
    H64* h = (H64*) p;

    gbyte cc_value = gIrqSaveAndDisable();

    h->next = root64;
    h->magic = MAGIC64;
    root64 = h;

    gIrqRestore(cc_value);
}

void Reset64() {
    root64 = gNULL;
}
