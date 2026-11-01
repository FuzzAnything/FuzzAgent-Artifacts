// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// adler32_z at adler32.c:61:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// adler32_combine at adler32.c:158:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// crc32_combine at crc32.c:981:15 in zlib.h
// crc32_combine64 at crc32.c:976:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// adler32_combine at adler32.c:158:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// crc32_combine at crc32.c:981:15 in zlib.h
// crc32_combine64 at crc32.c:976:15 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <zlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* Fuzz driver for zlib checksum functions */

int LLVMFuzzerTestOneInput_43(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Initialize variables for checksums */
    uLong adler = 0;
    uLong crc = 0;
    uLong adler1, adler2;
    uLong crc1, crc2;
    z_off_t len2_off;
    z_off64_t len2_off64;
    z_size_t len_z;

    /* Use input data for buffer and length derivation */
    const Bytef *buf = (const Bytef *)Data;
    uInt len_uInt = (uInt)(Size & 0xFFFF); /* Limit to uInt range */
    z_size_t len_z_val = (z_size_t)Size;

    /* 1. Test adler32_z */
    /* Initial call with NULL to get base value */
    adler = adler32_z(0, NULL, 0);
    /* Compute checksum on data */
    adler = adler32_z(adler, buf, len_z_val);
    /* Compute with different initial value */
    adler = adler32_z(0x1234, buf, len_z_val);

    /* 2. Test crc32 */
    crc = crc32(0, NULL, 0);
    crc = crc32(crc, buf, len_uInt);
    crc = crc32(0x1234, buf, len_uInt);

    /* 3. Test crc32_z */
    crc = crc32_z(0, NULL, 0);
    crc = crc32_z(crc, buf, len_z_val);
    crc = crc32_z(0x1234, buf, len_z_val);

    /* 4. Test adler32_combine */
    /* Split data into two parts for combining */
    size_t split = Size / 2;
    if (split == 0) split = 1;
    if (split > Size) split = Size;

    adler1 = adler32_z(0, buf, split);
    adler2 = adler32_z(0, buf + split, Size - split);
    len2_off = (z_off_t)(Size - split);
    adler = adler32_combine(adler1, adler2, len2_off);

    /* 5. Test crc32_combine */
    crc1 = crc32_z(0, buf, split);
    crc2 = crc32_z(0, buf + split, Size - split);
    crc = crc32_combine(crc1, crc2, len2_off);

    /* 6. Test crc32_combine64 */
    len2_off64 = (z_off64_t)(Size - split);
    crc = crc32_combine64(crc1, crc2, len2_off64);

    /* Additional edge cases */
    if (Size >= 2) {
        /* Test with very small lengths */
        adler = adler32_z(0, buf, 1);
        crc = crc32(0, buf, 1);
        crc = crc32_z(0, buf, 1);

        /* Test combine with zero length second part */
        adler1 = adler32_z(0, buf, Size);
        adler2 = adler32_z(0, NULL, 0);
        adler = adler32_combine(adler1, adler2, 0);

        crc1 = crc32_z(0, buf, Size);
        crc2 = crc32_z(0, NULL, 0);
        crc = crc32_combine(crc1, crc2, 0);
        crc = crc32_combine64(crc1, crc2, 0);
    }

    /* Return success */
    return 0;
}