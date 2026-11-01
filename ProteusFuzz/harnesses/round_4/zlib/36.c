// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// crc32_combine_op at crc32.c:969:15 in zlib.h
// crc32_combine64 at crc32.c:976:15 in zlib.h
// crc32_combine_gen64 at crc32.c:954:15 in zlib.h
// crc32_combine_gen at crc32.c:964:15 in zlib.h
// crc32_combine at crc32.c:981:15 in zlib.h
// adler32_combine64 at adler32.c:162:15 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <zlib.h>

static void fuzz_crc32_combine_op(const uint8_t *data, size_t size) {
    if (size < 12) return;
    
    uLong crc1 = *(const uLong*)(data + 0);
    uLong crc2 = *(const uLong*)(data + 4);
    uLong op = *(const uLong*)(data + 8);
    
    crc32_combine_op(crc1, crc2, op);
}

static void fuzz_crc32_combine64(const uint8_t *data, size_t size) {
    if (size < 16) return;
    
    uLong crc1 = *(const uLong*)(data + 0);
    uLong crc2 = *(const uLong*)(data + 4);
    z_off64_t len2 = *(const z_off64_t*)(data + 8);
    
    crc32_combine64(crc1, crc2, len2);
}

static void fuzz_crc32_combine_gen64(const uint8_t *data, size_t size) {
    if (size < 8) return;
    
    z_off64_t len2 = *(const z_off64_t*)(data + 0);
    
    crc32_combine_gen64(len2);
}

static void fuzz_crc32_combine_gen(const uint8_t *data, size_t size) {
    if (size < 8) return;
    
    z_off_t len2 = *(const z_off_t*)(data + 0);
    
    crc32_combine_gen(len2);
}

static void fuzz_crc32_combine(const uint8_t *data, size_t size) {
    if (size < 16) return;
    
    uLong crc1 = *(const uLong*)(data + 0);
    uLong crc2 = *(const uLong*)(data + 4);
    z_off_t len2 = *(const z_off_t*)(data + 8);
    
    crc32_combine(crc1, crc2, len2);
}

static void fuzz_adler32_combine64(const uint8_t *data, size_t size) {
    if (size < 16) return;
    
    uLong adler1 = *(const uLong*)(data + 0);
    uLong adler2 = *(const uLong*)(data + 4);
    z_off64_t len2 = *(const z_off64_t*)(data + 8);
    
    adler32_combine64(adler1, adler2, len2);
}

int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Test all target functions
    fuzz_crc32_combine_op(Data, Size);
    fuzz_crc32_combine64(Data, Size);
    fuzz_crc32_combine_gen64(Data, Size);
    fuzz_crc32_combine_gen(Data, Size);
    fuzz_crc32_combine(Data, Size);
    fuzz_adler32_combine64(Data, Size);
    
    return 0;
}