#ifndef _NEKOT1_FRIEND_H_
#define _NEKOT1_FRIEND_H_

// Do not call xSendClientPacket directly.
// It is used by some public macros.
void xSendClientPacket(gword p, char* pay, gword size);

// xAfterMain3 is the actual function called by
// the macro gAfterMain(after_main).
void xAfterMain3(gfunc after_main, gword* final, gword* final_startup);

#endif //  _NEKOT1_FRIEND_H_
