// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// crc32_combine64 at crc32.c:976:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// crc32_combine_op at crc32.c:969:15 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"

static void dummy_crc32_combine64(const uint8_t *data, size_t size) {
    if (size < sizeof(uLong) * 2 + sizeof(z_off64_t)) return;
    
    uLong crc1 = *(const uLong*)data;
    uLong crc2 = *(const uLong*)(data + sizeof(uLong));
    z_off64_t len2 = *(const z_off64_t*)(data + 2 * sizeof(uLong));
    
    if (len2 < 0) len2 = -len2;
    
    crc32_combine64(crc1, crc2, len2);
}

static void dummy_crc32_z(const uint8_t *data, size_t size) {
    if (size < sizeof(uLong)) return;
    
    uLong crc = *(const uLong*)data;
    const Bytef *buf = (const Bytef*)(data + sizeof(uLong));
    z_size_t len = size - sizeof(uLong);
    
    crc32_z(crc, buf, len);
}

static void dummy_adler32(const uint8_t *data, size_t size) {
    if (size < sizeof(uLong)) return;
    
    uLong adler = *(const uLong*)data;
    const Bytef *buf = (const Bytef*)(data + sizeof(uLong));
    uInt len = size - sizeof(uLong);
    if (len > 0xFFFFFFFF) len = 0xFFFFFFFF;
    
    adler32(adler, buf, len);
}

static void dummy_crc32(const uint8_t *data, size_t size) {
    if (size < sizeof(uLong)) return;
    
    uLong crc = *(const uLong*)data;
    const Bytef *buf = (const Bytef*)(data + sizeof(uLong));
    uInt len = size - sizeof(uLong);
    if (len > 0xFFFFFFFF) len = 0xFFFFFFFF;
    
    crc32(crc, buf, len);
}

static void dummy_adler32_z(const uint8_t *data, size_t size) {
    if (size < sizeof(uLong)) return;
    
    uLong adler = *(const uLong*)data;
    const Bytef *buf = (const Bytef*)(data + sizeof(uLong));
    z_size_t len = size - sizeof(uLong);
    
    adler32_z(adler, buf, len);
}

static void dummy_crc32_combine_op(const uint8_t *data, size_t size) {
    if (size < sizeof(uLong) * 3) return;
    
    uLong crc1 = *(const uLong*)data;
    uLong crc2 = *(const uLong*)(data + sizeof(uLong));
    uLong op = *(const uLong*)(data + 2 * sizeof(uLong));
    
    crc32_combine_op(crc1, crc2, op);
}

int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    uint8_t *mutated_data = malloc(Size);
    if (!mutated_data) return 0;
    memcpy(mutated_data, Data, Size);
    
    unsigned int selector = mutated_data[0] % 6;
    
    switch (selector) {
        case 0:
            dummy_crc32_combine64(mutated_data, Size);
            break;
        case 1:
            dummy_crc32_z(mutated_data, Size);
            break;
        case 2:
            dummy_adler32(mutated_data, Size);
            break;
        case 3:
            dummy_crc32(mutated_data, Size);
            break;
        case 4:
            dummy_adler32_z(mutated_data, Size);
            break;
        case 5:
            dummy_crc32_combine_op(mutated_data, Size);
            break;
    }
    
    free(mutated_data);
    return 0;
}