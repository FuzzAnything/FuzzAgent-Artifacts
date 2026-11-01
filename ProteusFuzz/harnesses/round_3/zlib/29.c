// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// adler32 at adler32.c:128:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// adler32_combine at adler32.c:158:15 in zlib.h
// adler32_combine64 at adler32.c:162:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32_combine64 at crc32.c:976:15 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <zlib.h>

int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    /* Split input into segments for testing */
    size_t len1 = Size / 2;
    size_t len2 = Size - len1;
    const Bytef *buf1 = (const Bytef *)Data;
    const Bytef *buf2 = (const Bytef *)(Data + len1);

    /* Test adler32 */
    uLong adler = adler32(0L, NULL, 0);
    adler = adler32(adler, buf1, (uInt)len1);
    adler = adler32(adler, buf2, (uInt)len2);

    /* Test adler32_z */
    uLong adler_z = adler32_z(0L, NULL, 0);
    adler_z = adler32_z(adler_z, buf1, (z_size_t)len1);
    adler_z = adler32_z(adler_z, buf2, (z_size_t)len2);

    /* Test adler32_combine */
    uLong adler1_part = adler32(0L, buf1, (uInt)len1);
    uLong adler2_part = adler32(0L, buf2, (uInt)len2);
    uLong adler_combined = adler32_combine(adler1_part, adler2_part, (z_off_t)len2);

    /* Test adler32_combine64 */
    uLong adler_combined64 = adler32_combine64(adler1_part, adler2_part, (z_off64_t)len2);

    /* Test crc32 */
    uLong crc = crc32(0L, NULL, 0);
    crc = crc32(crc, buf1, (uInt)len1);
    crc = crc32(crc, buf2, (uInt)len2);

    /* Test crc32_combine64 */
    uLong crc1_part = crc32(0L, buf1, (uInt)len1);
    uLong crc2_part = crc32(0L, buf2, (uInt)len2);
    uLong crc_combined64 = crc32_combine64(crc1_part, crc2_part, (z_off64_t)len2);

    /* No cleanup needed for these stateless functions */
    return 0;
}