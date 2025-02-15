#define CMD_LOG 200
#define CMD_DATA 204
#define CMD_ECHO 217

byte logbuf[30];

size_t strlen(const char* s) {
    const char* p = s;
    while (*p) ++p;
    return p-s;
}

// HACK
void Network_Log(const char* s) {
    size_t n = strlen(s);
    logbuf[0] = CMD_LOG;
    Poke2(logbuf+1, n);
    Poke2(logbuf+3, 0);
    MemCopy(logbuf+5, (byte*)s, n);

    tx_ptr_t t = WizReserveToSend(n+5);
    t = WizBytesToSend(t, logbuf, n+5);
    WizFinalizeSend(n+5);

    logbuf[0] = CMD_ECHO;
    logbuf[5]++;
    tx_ptr_t t2 = WizReserveToSend(n+5);
    t2 = WizBytesToSend(t2, logbuf, n+5);
    WizFinalizeSend(n+5);
}

byte recv_head[5];
byte recv_buf[30];
bool recv_just_head;

void CheckRecv() {
#if 1 
    word bytes_waiting_out = 0;
    errnum e = WizRecvGetBytesWaiting(&bytes_waiting_out);
    Printf("Wait(%u,%u)", e, bytes_waiting_out);
#endif

#if 0
    byte* h = recv_head;
    byte* b = recv_buf;

    if (!recv_just_head) {
        errnum e = WizRecvChunkTry(h, 5);
        if (e==NOTYET) return;
        if (e) Fatal("RECV", e);
        recv_just_head = true;
    }

    // switch (h[0]) {
    // case CMD_DATA:
        {
            word n = Peek2(h+1);
            errnum e2 = WizRecvChunkTry(b, n);
            if (e2==NOTYET) Fatal("PANIC NY ",0);
            if (e2) Fatal("ERR E ",e2);
            recv_just_head = false;

            MemCopy(logbuf, h, 5);
            MemCopy(logbuf+5, b, n);
            logbuf[0] = CMD_LOG;
            tx_ptr_t t3 = WizReserveToSend(n+5);
            t3 = WizBytesToSend(t3, logbuf, n+5);
            WizFinalizeSend(n+5);
        }
        // break;
    // }
#endif
}
