// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// crc32_z at crc32.c:626:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// crc32_combine64 at crc32.c:976:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// adler32_combine at adler32.c:158:15 in zlib.h
// adler32_combine64 at adler32.c:162:15 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static z_off64_t bytes_to_z_off64(const uint8_t *data, size_t size) {
    z_off64_t val = 0;
    size_t bytes = size < sizeof(z_off64_t) ? size : sizeof(z_off64_t);
    for (size_t i = 0; i < bytes; i++) {
        val = (val << 8) | data[i];
    }
    return val;
}

static z_off_t bytes_to_z_off(const uint8_t *data, size_t size) {
    z_off_t val = 0;
    size_t bytes = size < sizeof(z_off_t) ? size : sizeof(z_off_t);
    for (size_t i = 0; i < bytes; i++) {
        val = (val << 8) | data[i];
    }
    return val;
}

int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Split input into three parts for different purposes
    size_t part1_size = Size / 3;
    size_t part2_size = Size / 3;
    size_t part3_size = Size - 2 * (Size / 3);
    
    const uint8_t *part1 = Data;
    const uint8_t *part2 = Data + part1_size;
    const uint8_t *part3 = Data + part1_size + part2_size;

    // Test crc32_z
    uLong crc = crc32_z(0L, part1, part1_size);
    crc = crc32_z(crc, part2, part2_size);

    // Test adler32
    uLong adler = adler32(0L, part1, part1_size);
    adler = adler32(adler, part2, part2_size);

    // Test adler32_z
    uLong adler_z = adler32_z(1L, part1, part1_size);
    adler_z = adler32_z(adler_z, part2, part2_size);

    // Test combine functions with derived values
    z_off64_t len2_64 = bytes_to_z_off64(part3, part3_size);
    z_off_t len2 = bytes_to_z_off(part3, part3_size);

    // Test crc32_combine64
    uLong crc1 = crc32_z(0L, part1, part1_size);
    uLong crc2 = crc32_z(0L, part2, part2_size);
    uLong combined_crc = crc32_combine64(crc1, crc2, len2_64);

    // Test adler32_combine
    uLong adler1 = adler32_z(1L, part1, part1_size);
    uLong adler2 = adler32_z(1L, part2, part2_size);
    uLong combined_adler = adler32_combine(adler1, adler2, len2);

    // Test adler32_combine64
    uLong combined_adler64 = adler32_combine64(adler1, adler2, len2_64);

    // Use the results to avoid optimization
    volatile uLong dummy = crc + adler + adler_z + combined_crc + combined_adler + combined_adler64;
    (void)dummy;

    return 0;
}