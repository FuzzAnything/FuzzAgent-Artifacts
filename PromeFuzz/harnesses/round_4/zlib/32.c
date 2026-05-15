// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// adler32 at adler32.c:128:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// adler32_combine64 at adler32.c:162:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// adler32_combine at adler32.c:158:15 in zlib.h
// adler32_combine at adler32.c:158:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32_combine64 at crc32.c:976:15 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void fuzz_adler32_z(const uint8_t *data, size_t size) {
    if (size == 0) {
        adler32_z(1L, NULL, 0);
        return;
    }
    
    uLong adler = 1L;
    size_t chunk_size = size / 4;
    if (chunk_size == 0) chunk_size = 1;
    
    for (size_t i = 0; i < size; i += chunk_size) {
        size_t len = (i + chunk_size <= size) ? chunk_size : size - i;
        adler = adler32_z(adler, (const Bytef *)data + i, len);
    }
}

static void fuzz_adler32(const uint8_t *data, size_t size) {
    if (size == 0) {
        adler32(0L, Z_NULL, 0);
        return;
    }
    
    uLong adler = 0L;
    size_t chunk_size = size / 4;
    if (chunk_size == 0) chunk_size = 1;
    
    for (size_t i = 0; i < size; i += chunk_size) {
        size_t len = (i + chunk_size <= size) ? chunk_size : size - i;
        if (len > 0xFFFFFFFF) len = 0xFFFFFFFF;
        adler = adler32(adler, (const Bytef *)data + i, (uInt)len);
    }
}

static void fuzz_crc32_z(const uint8_t *data, size_t size) {
    if (size == 0) {
        crc32_z(0L, NULL, 0);
        return;
    }
    
    uLong crc = 0L;
    size_t chunk_size = size / 4;
    if (chunk_size == 0) chunk_size = 1;
    
    for (size_t i = 0; i < size; i += chunk_size) {
        size_t len = (i + chunk_size <= size) ? chunk_size : size - i;
        crc = crc32_z(crc, (const Bytef *)data + i, len);
    }
}

static void fuzz_adler32_combine(const uint8_t *data, size_t size) {
    if (size < 2) return;
    
    uLong adler1 = adler32(0L, Z_NULL, 0);
    uLong adler2 = adler32(0L, Z_NULL, 0);
    
    size_t half = size / 2;
    if (half > 0) {
        adler1 = adler32(adler1, (const Bytef *)data, half);
        adler2 = adler32(adler2, (const Bytef *)data + half, size - half);
    }
    
    z_off_t len2 = (z_off_t)(size - half);
    adler32_combine(adler1, adler2, len2);
    
    if (size > 10) {
        len2 = -(z_off_t)(size - half);
        adler32_combine(adler1, adler2, len2);
    }
}

static void fuzz_crc32_combine64(const uint8_t *data, size_t size) {
    if (size < 2) return;
    
    uLong crc1 = crc32(0L, Z_NULL, 0);
    uLong crc2 = crc32(0L, Z_NULL, 0);
    
    size_t half = size / 2;
    if (half > 0) {
        crc1 = crc32(crc1, (const Bytef *)data, half);
        crc2 = crc32(crc2, (const Bytef *)data + half, size - half);
    }
    
    z_off64_t len2 = (z_off64_t)(size - half);
    crc32_combine64(crc1, crc2, len2);
}

static void fuzz_adler32_combine64(const uint8_t *data, size_t size) {
    if (size < 2) return;
    
    uLong adler1 = adler32(0L, Z_NULL, 0);
    uLong adler2 = adler32(0L, Z_NULL, 0);
    
    size_t half = size / 2;
    if (half > 0) {
        adler1 = adler32(adler1, (const Bytef *)data, half);
        adler2 = adler32(adler2, (const Bytef *)data + half, size - half);
    }
    
    z_off64_t len2 = (z_off64_t)(size - half);
    adler32_combine64(adler1, adler2, len2);
}

int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        fuzz_adler32_z(NULL, 0);
        fuzz_adler32(NULL, 0);
        fuzz_crc32_z(NULL, 0);
        return 0;
    }
    
    fuzz_adler32_z(Data, Size);
    fuzz_adler32(Data, Size);
    fuzz_crc32_z(Data, Size);
    fuzz_adler32_combine(Data, Size);
    fuzz_crc32_combine64(Data, Size);
    fuzz_adler32_combine64(Data, Size);
    
    return 0;
}