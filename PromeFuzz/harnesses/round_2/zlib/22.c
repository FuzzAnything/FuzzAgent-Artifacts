// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// crc32 at crc32.c:946:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// crc32_combine_gen at crc32.c:964:15 in zlib.h
// crc32_combine_op at crc32.c:969:15 in zlib.h
// crc32_combine_op at crc32.c:969:15 in zlib.h
// crc32_combine_gen at crc32.c:964:15 in zlib.h
// crc32_combine_op at crc32.c:969:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32_combine64 at crc32.c:976:15 in zlib.h
// crc32_combine64 at crc32.c:976:15 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "zlib.h"

static void fuzz_adler32(const uint8_t *data, size_t size) {
    uLong adler = adler32(0L, Z_NULL, 0);
    
    if (size > 0) {
        size_t offset = 0;
        while (offset < size) {
            size_t chunk_size = (size - offset) % 256;
            if (chunk_size == 0) chunk_size = 1;
            if (offset + chunk_size > size) chunk_size = size - offset;
            
            adler = adler32(adler, (const Bytef *)&data[offset], (uInt)chunk_size);
            offset += chunk_size;
        }
    }
    
    adler = adler32(adler, Z_NULL, 0);
}

static void fuzz_adler32_z(const uint8_t *data, size_t size) {
    uLong adler = 1L;
    
    if (size > 0) {
        size_t offset = 0;
        while (offset < size) {
            size_t chunk_size = (size - offset) % 512;
            if (chunk_size == 0) chunk_size = 1;
            if (offset + chunk_size > size) chunk_size = size - offset;
            
            adler = adler32_z(adler, (const Bytef *)&data[offset], chunk_size);
            offset += chunk_size;
        }
    }
    
    adler = adler32_z(adler, Z_NULL, 0);
}

static void fuzz_crc32(const uint8_t *data, size_t size) {
    uLong crc = crc32(0L, Z_NULL, 0);
    
    if (size > 0) {
        size_t offset = 0;
        while (offset < size) {
            size_t chunk_size = (size - offset) % 128;
            if (chunk_size == 0) chunk_size = 1;
            if (offset + chunk_size > size) chunk_size = size - offset;
            
            crc = crc32(crc, (const Bytef *)&data[offset], (uInt)chunk_size);
            offset += chunk_size;
        }
    }
    
    crc = crc32(crc, Z_NULL, 0);
}

static void fuzz_crc32_z(const uint8_t *data, size_t size) {
    uLong crc = 0L;
    
    if (size > 0) {
        size_t offset = 0;
        while (offset < size) {
            size_t chunk_size = (size - offset) % 1024;
            if (chunk_size == 0) chunk_size = 1;
            if (offset + chunk_size > size) chunk_size = size - offset;
            
            crc = crc32_z(crc, (const Bytef *)&data[offset], chunk_size);
            offset += chunk_size;
        }
    }
    
    crc = crc32_z(crc, Z_NULL, 0);
}

static void fuzz_crc32_combine64(const uint8_t *data, size_t size) {
    if (size < 2) return;
    
    size_t split = size / 2;
    uLong crc1 = crc32(0L, Z_NULL, 0);
    uLong crc2 = crc32(0L, Z_NULL, 0);
    
    crc1 = crc32(crc1, (const Bytef *)data, (uInt)split);
    crc2 = crc32(crc2, (const Bytef *)&data[split], (uInt)(size - split));
    
    z_off64_t len2 = (z_off64_t)(size - split);
    if (len2 < 0) len2 = 0;
    
    uLong combined = crc32_combine64(crc1, crc2, len2);
    
    len2 = (z_off64_t)split;
    if (len2 < 0) len2 = 0;
    combined = crc32_combine64(crc2, crc1, len2);
}

static void fuzz_crc32_combine_op(const uint8_t *data, size_t size) {
    if (size < 2) return;
    
    size_t split = size / 2;
    uLong crc1 = crc32(0L, Z_NULL, 0);
    uLong crc2 = crc32(0L, Z_NULL, 0);
    
    crc1 = crc32(crc1, (const Bytef *)data, (uInt)split);
    crc2 = crc32(crc2, (const Bytef *)&data[split], (uInt)(size - split));
    
    z_off64_t len2 = (z_off64_t)(size - split);
    if (len2 < 0) len2 = 0;
    
    uLong op = 0;
    if (len2 > 0) {
        op = crc32_combine_gen(len2);
    }
    
    uLong combined = crc32_combine_op(crc1, crc2, op);
    
    combined = crc32_combine_op(crc1, crc2, 0);
    
    len2 = (z_off64_t)split;
    if (len2 < 0) len2 = 0;
    if (len2 > 0) {
        op = crc32_combine_gen(len2);
        combined = crc32_combine_op(crc2, crc1, op);
    }
}

int LLVMFuzzerTestOneInput_22(const uint8_t *data, size_t size) {
    if (size == 0) {
        fuzz_adler32(data, size);
        fuzz_adler32_z(data, size);
        fuzz_crc32(data, size);
        fuzz_crc32_z(data, size);
        return 0;
    }
    
    fuzz_adler32(data, size);
    fuzz_adler32_z(data, size);
    fuzz_crc32(data, size);
    fuzz_crc32_z(data, size);
    fuzz_crc32_combine64(data, size);
    fuzz_crc32_combine_op(data, size);
    
    return 0;
}