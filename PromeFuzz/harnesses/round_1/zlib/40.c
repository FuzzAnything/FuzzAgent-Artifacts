// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// crc32_combine64 at crc32.c:976:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// adler32_combine64 at adler32.c:162:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// adler32_combine at adler32.c:158:15 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "zlib.h"

static void fuzz_crc32_combine64(const uint8_t *data, size_t size) {
    if (size < sizeof(uLong) * 2 + sizeof(z_off64_t)) {
        return;
    }
    
    uLong crc1, crc2;
    z_off64_t len2;
    
    memcpy(&crc1, data, sizeof(crc1));
    memcpy(&crc2, data + sizeof(crc1), sizeof(crc2));
    memcpy(&len2, data + sizeof(crc1) + sizeof(crc2), sizeof(len2));
    
    crc32_combine64(crc1, crc2, len2);
}

static void fuzz_crc32_z(const uint8_t *data, size_t size) {
    if (size == 0) {
        return;
    }
    
    uLong crc = 0;
    crc = crc32_z(crc, data, size);
    
    crc = crc32_z(crc, data, size / 2);
    crc = crc32_z(crc, data + size / 2, size - size / 2);
}

static void fuzz_adler32(const uint8_t *data, size_t size) {
    if (size == 0) {
        adler32(0L, NULL, 0);
        return;
    }
    
    uLong adler = adler32(0L, Z_NULL, 0);
    
    if (size > UINT_MAX) {
        adler = adler32(adler, data, UINT_MAX);
    } else {
        adler = adler32(adler, data, (uInt)size);
    }
    
    adler = adler32(adler, NULL, 0);
}

static void fuzz_adler32_combine64(const uint8_t *data, size_t size) {
    if (size < sizeof(uLong) * 2 + sizeof(z_off64_t)) {
        return;
    }
    
    uLong adler1, adler2;
    z_off64_t len2;
    
    memcpy(&adler1, data, sizeof(adler1));
    memcpy(&adler2, data + sizeof(adler1), sizeof(adler2));
    memcpy(&len2, data + sizeof(adler1) + sizeof(adler2), sizeof(len2));
    
    adler32_combine64(adler1, adler2, len2);
}

static void fuzz_adler32_z(const uint8_t *data, size_t size) {
    if (size == 0) {
        adler32_z(1L, NULL, 0);
        return;
    }
    
    uLong adler = 1L;
    adler = adler32_z(adler, data, size);
    
    adler = adler32_z(adler, data, size / 2);
    adler = adler32_z(adler, data + size / 2, size - size / 2);
}

static void fuzz_adler32_combine(const uint8_t *data, size_t size) {
    if (size < sizeof(uLong) * 2 + sizeof(z_off_t)) {
        return;
    }
    
    uLong adler1, adler2;
    z_off_t len2;
    
    memcpy(&adler1, data, sizeof(adler1));
    memcpy(&adler2, data + sizeof(adler1), sizeof(adler2));
    memcpy(&len2, data + sizeof(adler1) + sizeof(adler2), sizeof(len2));
    
    adler32_combine(adler1, adler2, len2);
}

int LLVMFuzzerTestOneInput_40(const uint8_t *data, size_t size) {
    if (size == 0) {
        return 0;
    }
    
    fuzz_crc32_combine64(data, size);
    fuzz_crc32_z(data, size);
    fuzz_adler32(data, size);
    fuzz_adler32_combine64(data, size);
    fuzz_adler32_z(data, size);
    fuzz_adler32_combine(data, size);
    
    return 0;
}