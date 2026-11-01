// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// adler32_z at adler32.c:61:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// adler32_combine at adler32.c:158:15 in zlib.h
// adler32_combine64 at adler32.c:162:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// crc32_combine at crc32.c:981:15 in zlib.h
// crc32_combine64 at crc32.c:976:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// adler32_combine at adler32.c:158:15 in zlib.h
// adler32_combine64 at adler32.c:162:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// crc32_combine at crc32.c:981:15 in zlib.h
// crc32_combine64 at crc32.c:976:15 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <zlib.h>

int LLVMFuzzerTestOneInput_49(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    /* Split input into segments to test various combinations */
    size_t len1 = Size / 2;
    size_t len2 = Size - len1;
    const Bytef *buf1 = (const Bytef *)Data;
    const Bytef *buf2 = (const Bytef *)(Data + len1);

    /* Test adler32_z */
    uLong adler1 = adler32_z(0, buf1, (z_size_t)len1);
    uLong adler2 = adler32_z(0, buf2, (z_size_t)len2);
    uLong combined_adler = adler32_combine(adler1, adler2, (z_off_t)len2);
    uLong combined_adler64 = adler32_combine64(adler1, adler2, (z_off64_t)len2);

    /* Test crc32_z */
    uLong crc1 = crc32_z(0, buf1, (z_size_t)len1);
    uLong crc2 = crc32_z(0, buf2, (z_size_t)len2);
    uLong combined_crc = crc32_combine(crc1, crc2, (z_off_t)len2);
    uLong combined_crc64 = crc32_combine64(crc1, crc2, (z_off64_t)len2);

    /* Test edge cases with NULL buffer and zero length */
    uLong adler_null = adler32_z(0, Z_NULL, 0);
    uLong crc_null = crc32_z(0, Z_NULL, 0);

    /* Test with varying lengths to explore different states */
    if (Size > 8) {
        size_t len3 = Size / 3;
        size_t len4 = Size - len3;
        const Bytef *buf3 = (const Bytef *)Data;
        const Bytef *buf4 = (const Bytef *)(Data + len3);

        uLong adler3 = adler32_z(0, buf3, (z_size_t)len3);
        uLong adler4 = adler32_z(0, buf4, (z_size_t)len4);
        uLong combined_adler_var = adler32_combine(adler3, adler4, (z_off_t)len4);
        uLong combined_adler_var64 = adler32_combine64(adler3, adler4, (z_off64_t)len4);

        uLong crc3 = crc32_z(0, buf3, (z_size_t)len3);
        uLong crc4 = crc32_z(0, buf4, (z_size_t)len4);
        uLong combined_crc_var = crc32_combine(crc3, crc4, (z_off_t)len4);
        uLong combined_crc_var64 = crc32_combine64(crc3, crc4, (z_off64_t)len4);
    }

    /* Ensure all computed values are used to prevent compiler optimizations */
    (void)combined_adler;
    (void)combined_adler64;
    (void)combined_crc;
    (void)combined_crc64;
    (void)adler_null;
    (void)crc_null;

    return 0;
}