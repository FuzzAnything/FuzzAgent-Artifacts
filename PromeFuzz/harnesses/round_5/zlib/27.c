// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// adler32 at adler32.c:128:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// adler32_combine at adler32.c:158:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// crc32_combine64 at crc32.c:976:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// adler32_combine64 at adler32.c:162:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void fuzz_crc32_z(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    uLong crc = 0L;
    const Bytef *buf = (const Bytef *)data;
    z_size_t len = (z_size_t)size;
    
    crc = crc32_z(crc, buf, len);
    
    if (size > 1) {
        size_t split = size / 2;
        uLong crc1 = crc32_z(0L, buf, (z_size_t)split);
        uLong crc2 = crc32_z(crc1, buf + split, (z_size_t)(size - split));
    }
}

static void fuzz_crc32_combine64(const uint8_t *data, size_t size) {
    uLong crc1 = 0;
    uLong crc2 = 0;
    z_off64_t len2 = 0;
    
    if (size >= sizeof(uLong)) {
        memcpy(&crc1, data, sizeof(uLong));
        if (size >= 2 * sizeof(uLong)) {
            memcpy(&crc2, data + sizeof(uLong), sizeof(uLong));
            len2 = (z_off64_t)(size - 2 * sizeof(uLong));
        } else {
            len2 = (z_off64_t)(size - sizeof(uLong));
        }
    } else {
        len2 = (z_off64_t)size;
    }
    
    if (len2 < 0) len2 = 0;
    
    crc32_combine64(crc1, crc2, len2);
}

static void fuzz_adler32_z(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    uLong adler = 1L;
    const Bytef *buf = (const Bytef *)data;
    z_size_t len = (z_size_t)size;
    
    adler = adler32_z(adler, buf, len);
    
    if (size > 1) {
        size_t split = size / 2;
        uLong adler1 = adler32_z(1L, buf, (z_size_t)split);
        uLong adler2 = adler32_z(adler1, buf + split, (z_size_t)(size - split));
    }
}

static void fuzz_adler32_combine64(const uint8_t *data, size_t size) {
    uLong adler1 = 0;
    uLong adler2 = 0;
    z_off64_t len2 = 0;
    
    if (size >= sizeof(uLong)) {
        memcpy(&adler1, data, sizeof(uLong));
        if (size >= 2 * sizeof(uLong)) {
            memcpy(&adler2, data + sizeof(uLong), sizeof(uLong));
            len2 = (z_off64_t)(size - 2 * sizeof(uLong));
        } else {
            len2 = (z_off64_t)(size - sizeof(uLong));
        }
    } else {
        len2 = (z_off64_t)size;
    }
    
    if (len2 < 0) len2 = 0;
    
    adler32_combine64(adler1, adler2, len2);
}

static void fuzz_adler32(const uint8_t *data, size_t size) {
    if (size == 0) return;
    
    uLong adler = 1L;
    const Bytef *buf = (const Bytef *)data;
    uInt len = (uInt)(size > UINT_MAX ? UINT_MAX : size);
    
    adler = adler32(adler, buf, len);
    
    if (size > 1) {
        size_t split = size / 2;
        uInt len1 = (uInt)(split > UINT_MAX ? UINT_MAX : split);
        uInt len2 = (uInt)((size - split) > UINT_MAX ? UINT_MAX : (size - split));
        
        uLong adler1 = adler32(1L, buf, len1);
        uLong adler2 = adler32(adler1, buf + split, len2);
    }
}

static void fuzz_adler32_combine(const uint8_t *data, size_t size) {
    uLong adler1 = 0;
    uLong adler2 = 0;
    z_off_t len2 = 0;
    
    if (size >= sizeof(uLong)) {
        memcpy(&adler1, data, sizeof(uLong));
        if (size >= 2 * sizeof(uLong)) {
            memcpy(&adler2, data + sizeof(uLong), sizeof(uLong));
            len2 = (z_off_t)(size - 2 * sizeof(uLong));
        } else {
            len2 = (z_off_t)(size - sizeof(uLong));
        }
    } else {
        len2 = (z_off_t)size;
    }
    
    if (len2 < 0) len2 = 0;
    
    adler32_combine(adler1, adler2, len2);
}

int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    fuzz_crc32_z(Data, Size);
    fuzz_crc32_combine64(Data, Size);
    fuzz_adler32_z(Data, Size);
    fuzz_adler32_combine64(Data, Size);
    fuzz_adler32(Data, Size);
    fuzz_adler32_combine(Data, Size);
    
    return 0;
}