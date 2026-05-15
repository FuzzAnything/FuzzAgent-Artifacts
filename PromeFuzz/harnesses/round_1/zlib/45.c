// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// crc32_combine64 at crc32.c:976:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// adler32_combine64 at adler32.c:162:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// adler32_combine at adler32.c:158:15 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"
#include <stdint.h>
#include <string.h>

static void fuzz_crc32_combine64(const uint8_t *data, size_t size) {
    if (size < sizeof(uLong) * 2 + sizeof(z_off64_t)) return;
    
    uLong crc1 = *(const uLong*)data;
    uLong crc2 = *(const uLong*)(data + sizeof(uLong));
    z_off64_t len2 = *(const z_off64_t*)(data + sizeof(uLong) * 2);
    
    crc32_combine64(crc1, crc2, len2);
}

static void fuzz_crc32_z(const uint8_t *data, size_t size) {
    if (size < sizeof(uLong)) return;
    
    uLong crc = *(const uLong*)data;
    const Bytef *buf = (const Bytef*)(data + sizeof(uLong));
    z_size_t len = size - sizeof(uLong);
    
    crc32_z(crc, buf, len);
}

static void fuzz_adler32(const uint8_t *data, size_t size) {
    if (size < sizeof(uLong)) return;
    
    uLong adler = *(const uLong*)data;
    const Bytef *buf = (const Bytef*)(data + sizeof(uLong));
    uInt len = (uInt)(size - sizeof(uLong));
    if (len != (size - sizeof(uLong))) return;
    
    adler32(adler, buf, len);
}

static void fuzz_adler32_combine64(const uint8_t *data, size_t size) {
    if (size < sizeof(uLong) * 2 + sizeof(z_off64_t)) return;
    
    uLong adler1 = *(const uLong*)data;
    uLong adler2 = *(const uLong*)(data + sizeof(uLong));
    z_off64_t len2 = *(const z_off64_t*)(data + sizeof(uLong) * 2);
    
    adler32_combine64(adler1, adler2, len2);
}

static void fuzz_adler32_z(const uint8_t *data, size_t size) {
    if (size < sizeof(uLong)) return;
    
    uLong adler = *(const uLong*)data;
    const Bytef *buf = (const Bytef*)(data + sizeof(uLong));
    z_size_t len = size - sizeof(uLong);
    
    adler32_z(adler, buf, len);
}

static void fuzz_adler32_combine(const uint8_t *data, size_t size) {
    if (size < sizeof(uLong) * 2 + sizeof(z_off_t)) return;
    
    uLong adler1 = *(const uLong*)data;
    uLong adler2 = *(const uLong*)(data + sizeof(uLong));
    z_off_t len2 = *(const z_off_t*)(data + sizeof(uLong) * 2);
    
    adler32_combine(adler1, adler2, len2);
}

int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    fuzz_crc32_combine64(Data, Size);
    fuzz_crc32_z(Data, Size);
    fuzz_adler32(Data, Size);
    fuzz_adler32_combine64(Data, Size);
    fuzz_adler32_z(Data, Size);
    fuzz_adler32_combine(Data, Size);
    
    return 0;
}