void MemCopy(byte *dest, const byte *src, uint count) {
    // If count is odd, copy an initial byte.
    if (count & 1) {
        *dest++ = *src++;
    }
    // Now use a stride of 2.
    count >>= 1;  // Divide count by 2.
    uint* d = (uint*)dest;
    uint* s = (uint*)src;
    for (uint i = 0; i < count; i++) {
        *d++ = *s++;
    }
}
void MemSet(byte* dest, byte value, size_t n) {
    for (word i = 0; i < n; i++) {
        dest[i] = value;
    }
}
