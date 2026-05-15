// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// adler32 at adler32.c:128:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// adler32_combine64 at adler32.c:162:15 in zlib.h
// adler32_combine at adler32.c:158:15 in zlib.h
// crc32_combine64 at crc32.c:976:15 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"

#ifdef __cplusplus
extern "C" {
#endif

int LLVMFuzzerTestOneInput_44(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Split input data for various operations
    size_t split_point = Size / 2;
    const Bytef *data1 = (const Bytef *)Data;
    uInt len1 = (uInt)(split_point < 0xFFFF ? split_point : 0xFFFF);
    const Bytef *data2 = (const Bytef *)(Data + split_point);
    uInt len2 = (uInt)((Size - split_point) < 0xFFFF ? (Size - split_point) : 0xFFFF);
    z_size_t z_len1 = (z_size_t)split_point;
    z_size_t z_len2 = (z_size_t)(Size - split_point);

    // Test adler32
    uLong adler = adler32(0L, Z_NULL, 0);
    adler = adler32(adler, data1, len1);
    adler = adler32(adler, data2, len2);

    // Test adler32_z
    uLong adler_z = adler32_z(1L, Z_NULL, 0);
    adler_z = adler32_z(adler_z, data1, z_len1);
    adler_z = adler32_z(adler_z, data2, z_len2);

    // Test crc32_z
    uLong crc = crc32_z(0L, Z_NULL, 0);
    crc = crc32_z(crc, data1, z_len1);
    crc = crc32_z(crc, data2, z_len2);

    // Generate checksums for combine functions
    uLong adler1 = adler32(0L, data1, len1);
    uLong adler2 = adler32(0L, data2, len2);
    uLong crc1 = crc32_z(0L, data1, z_len1);
    uLong crc2 = crc32_z(0L, data2, z_len2);

    // Test adler32_combine64
    z_off64_t len2_64 = (z_off64_t)z_len2;
    if (len2_64 >= 0) {
        uLong combined_adler64 = adler32_combine64(adler1, adler2, len2_64);
        (void)combined_adler64;
    }

    // Test adler32_combine
    z_off_t len2_off = (z_off_t)z_len2;
    if (len2_off >= 0) {
        uLong combined_adler = adler32_combine(adler1, adler2, len2_off);
        (void)combined_adler;
    }

    // Test crc32_combine64
    if (len2_64 >= 0) {
        uLong combined_crc64 = crc32_combine64(crc1, crc2, len2_64);
        (void)combined_crc64;
    }

    return 0;
}

#ifdef __cplusplus
}
#endif