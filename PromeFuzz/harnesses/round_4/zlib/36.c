// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// adler32 at adler32.c:128:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32_combine at crc32.c:981:15 in zlib.h
// crc32_combine64 at crc32.c:976:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "code/zlib.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static volatile uLong sink = 0;

int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Split input into two parts for combine functions
    size_t split = Size / 2;
    const Bytef *buf1 = (const Bytef *)Data;
    uInt len1 = (uInt)(split < 0xFFFF ? split : 0xFFFF);
    const Bytef *buf2 = (const Bytef *)(Data + split);
    uInt len2 = (uInt)((Size - split) < 0xFFFF ? (Size - split) : 0xFFFF);
    z_size_t z_len1 = (z_size_t)split;
    z_size_t z_len2 = (z_size_t)(Size - split);

    // Test adler32
    uLong adler = adler32(0L, Z_NULL, 0);
    adler = adler32(adler, buf1, len1);
    adler = adler32(adler, buf2, len2);
    sink = adler;

    // Test adler32_z
    uLong adler_z = adler32_z(1L, Z_NULL, 0);
    adler_z = adler32_z(adler_z, buf1, z_len1);
    adler_z = adler32_z(adler_z, buf2, z_len2);
    sink = adler_z;

    // Test crc32
    uLong crc = crc32(0L, Z_NULL, 0);
    crc = crc32(crc, buf1, len1);
    crc = crc32(crc, buf2, len2);
    sink = crc;

    // Test crc32_z
    uLong crc_z = crc32_z(0L, Z_NULL, 0);
    crc_z = crc32_z(crc_z, buf1, z_len1);
    crc_z = crc32_z(crc_z, buf2, z_len2);
    sink = crc_z;

    // Test crc32_combine
    uLong crc1 = crc32(0L, buf1, len1);
    uLong crc2 = crc32(0L, buf2, len2);
    if (z_len2 >= 0) {
        uLong combined = crc32_combine(crc1, crc2, (z_off_t)z_len2);
        sink = combined;
    }

    // Test crc32_combine64
    if (z_len2 >= 0) {
        uLong combined64 = crc32_combine64(crc1, crc2, (z_off64_t)z_len2);
        sink = combined64;
    }

    // Test with NULL buffers
    adler32(0L, Z_NULL, 0);
    adler32_z(1L, Z_NULL, 0);
    crc32(0L, Z_NULL, 0);
    crc32_z(0L, Z_NULL, 0);

    // Test incremental updates with various chunk sizes
    uLong incremental_adler = adler32(0L, Z_NULL, 0);
    uLong incremental_crc = crc32(0L, Z_NULL, 0);
    
    for (size_t i = 0; i < Size; i += 64) {
        size_t chunk_size = (Size - i) < 64 ? (Size - i) : 64;
        uInt u_chunk = (uInt)(chunk_size < 0xFFFF ? chunk_size : 0xFFFF);
        z_size_t z_chunk = (z_size_t)chunk_size;
        
        incremental_adler = adler32(incremental_adler, (const Bytef *)(Data + i), u_chunk);
        incremental_crc = crc32(incremental_crc, (const Bytef *)(Data + i), u_chunk);
        
        adler32_z(incremental_adler, (const Bytef *)(Data + i), z_chunk);
        crc32_z(incremental_crc, (const Bytef *)(Data + i), z_chunk);
    }
    
    sink = incremental_adler + incremental_crc;

    return 0;
}