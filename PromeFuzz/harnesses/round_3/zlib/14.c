// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// crc32 at crc32.c:946:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// crc32_combine at crc32.c:981:15 in zlib.h
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

static void fuzz_crc32(const uint8_t *data, size_t size) {
    if (size < 4) return;
    
    uLong crc = (uLong)data[0] | ((uLong)data[1] << 8) | 
                ((uLong)data[2] << 16) | ((uLong)data[3] << 24);
    
    const Bytef *buf = data + 4;
    uInt len = (uInt)(size - 4);
    if (len > size - 4) len = (uInt)(size - 4);
    
    crc32(crc, buf, len);
}

static void fuzz_crc32_z(const uint8_t *data, size_t size) {
    if (size < 4) return;
    
    uLong crc = (uLong)data[0] | ((uLong)data[1] << 8) | 
                ((uLong)data[2] << 16) | ((uLong)data[3] << 24);
    
    const Bytef *buf = data + 4;
    z_size_t len = (z_size_t)(size - 4);
    if (len > size - 4) len = (z_size_t)(size - 4);
    
    crc32_z(crc, buf, len);
}

static void fuzz_adler32(const uint8_t *data, size_t size) {
    if (size < 4) return;
    
    uLong adler = (uLong)data[0] | ((uLong)data[1] << 8) | 
                  ((uLong)data[2] << 16) | ((uLong)data[3] << 24);
    
    const Bytef *buf = data + 4;
    uInt len = (uInt)(size - 4);
    if (len > size - 4) len = (uInt)(size - 4);
    
    adler32(adler, buf, len);
}

static void fuzz_adler32_z(const uint8_t *data, size_t size) {
    if (size < 4) return;
    
    uLong adler = (uLong)data[0] | ((uLong)data[1] << 8) | 
                  ((uLong)data[2] << 16) | ((uLong)data[3] << 24);
    
    const Bytef *buf = data + 4;
    z_size_t len = (z_size_t)(size - 4);
    if (len > size - 4) len = (z_size_t)(size - 4);
    
    adler32_z(adler, buf, len);
}

static void fuzz_crc32_combine(const uint8_t *data, size_t size) {
    if (size < 12) return;
    
    uLong crc1 = (uLong)data[0] | ((uLong)data[1] << 8) | 
                 ((uLong)data[2] << 16) | ((uLong)data[3] << 24);
    uLong crc2 = (uLong)data[4] | ((uLong)data[5] << 8) | 
                 ((uLong)data[6] << 16) | ((uLong)data[7] << 24);
    
    z_off_t len2 = (z_off_t)data[8] | ((z_off_t)data[9] << 8) | 
                   ((z_off_t)data[10] << 16) | ((z_off_t)data[11] << 24);
    
    if (len2 < 0) len2 = -len2;
    
    crc32_combine(crc1, crc2, len2);
}

static void fuzz_crc32_combine64(const uint8_t *data, size_t size) {
    if (size < 16) return;
    
    uLong crc1 = (uLong)data[0] | ((uLong)data[1] << 8) | 
                 ((uLong)data[2] << 16) | ((uLong)data[3] << 24);
    uLong crc2 = (uLong)data[4] | ((uLong)data[1] << 8) | 
                 ((uLong)data[2] << 16) | ((uLong)data[3] << 24);
    
    z_off64_t len2 = (z_off64_t)data[8] | ((z_off64_t)data[9] << 8) | 
                     ((z_off64_t)data[10] << 16) | ((z_off64_t)data[11] << 24) |
                     ((z_off64_t)data[12] << 32) | ((z_off64_t)data[13] << 40) |
                     ((z_off64_t)data[14] << 48) | ((z_off64_t)data[15] << 56);
    
    if (len2 < 0) len2 = -len2;
    
    crc32_combine64(crc1, crc2, len2);
}

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    fuzz_crc32(Data, Size);
    fuzz_crc32_z(Data, Size);
    fuzz_adler32(Data, Size);
    fuzz_adler32_z(Data, Size);
    fuzz_crc32_combine(Data, Size);
    fuzz_crc32_combine64(Data, Size);
    
    return 0;
}