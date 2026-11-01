// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// crc32 at crc32.c:946:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32_combine at crc32.c:981:15 in zlib.h
// crc32_combine_gen at crc32.c:964:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// adler32_combine at adler32.c:158:15 in zlib.h
// get_crc_table at crc32.c:482:29 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

/* Fuzz driver for zlib checksum functions */

int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    /* Split input into segments for testing */
    size_t len1 = Size / 3;
    size_t len2 = Size / 3;
    size_t len3 = Size - len1 - len2;

    const Bytef *buf1 = (const Bytef *)Data;
    const Bytef *buf2 = (const Bytef *)(Data + len1);
    const Bytef *buf3 = (const Bytef *)(Data + len1 + len2);

    /* Test crc32 */
    uLong crc = 0;
    crc = crc32(crc, buf1, (uInt)len1);
    crc = crc32(crc, buf2, (uInt)len2);
    crc = crc32(crc, buf3, (uInt)len3);

    /* Test crc32_z with large length handling */
    uLong crc_z = 0;
    crc_z = crc32_z(crc_z, buf1, (z_size_t)len1);
    crc_z = crc32_z(crc_z, buf2, (z_size_t)len2);
    crc_z = crc32_z(crc_z, buf3, (z_size_t)len3);

    /* Test crc32_combine */
    uLong crc1 = crc32(0, buf1, (uInt)len1);
    uLong crc2 = crc32(0, buf2, (uInt)len2);
    uLong crc_combined = crc32_combine(crc1, crc2, (z_off_t)len2);

    /* Test crc32_combine_gen */
    uLong gen_table = crc32_combine_gen((z_off_t)len2);
    (void)gen_table; /* Suppress unused variable warning */

    /* Test adler32_combine */
    uLong adler1 = adler32(0, buf1, (uInt)len1);
    uLong adler2 = adler32(0, buf2, (uInt)len2);
    uLong adler_combined = adler32_combine(adler1, adler2, (z_off_t)len2);

    /* Test get_crc_table */
    const z_crc_t *crc_table = get_crc_table();
    if (crc_table != NULL) {
        /* Access table to ensure it's initialized */
        (void)crc_table[0];
    }

    return 0;
}