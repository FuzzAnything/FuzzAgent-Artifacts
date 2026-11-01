// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// adler32 at adler32.c:128:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32_combine64 at crc32.c:976:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// adler32_combine at adler32.c:158:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// adler32_combine64 at adler32.c:162:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <zlib.h>

int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    /* Parse input to determine which functions to test and how */
    uint8_t mode = Data[0] % 7;
    const uint8_t *buf = Data + 1;
    size_t len = Size - 1;

    /* Ensure len is within safe bounds for uInt and z_size_t */
    if (len > 0x7FFFFFFF) {
        len = 0x7FFFFFFF;
    }

    uLong adler = 0;
    uLong crc = 0;
    uLong adler2 = 0;
    uLong crc2 = 0;
    z_off64_t len64 = (z_off64_t)len;
    z_off_t len32 = (z_off_t)len;

    switch (mode) {
        case 0: {
            /* Test adler32 */
            adler = adler32(adler, (const Bytef *)buf, (uInt)len);
            (void)adler;
            break;
        }
        case 1: {
            /* Test crc32 */
            crc = crc32(crc, (const Bytef *)buf, (uInt)len);
            (void)crc;
            break;
        }
        case 2: {
            /* Test crc32_combine64 */
            crc = crc32(crc, (const Bytef *)buf, (uInt)len);
            crc2 = crc32(crc2, (const Bytef *)buf, (uInt)len);
            crc = crc32_combine64(crc, crc2, len64);
            (void)crc;
            break;
        }
        case 3: {
            /* Test adler32_z */
            adler = adler32_z(adler, (const Bytef *)buf, (z_size_t)len);
            (void)adler;
            break;
        }
        case 4: {
            /* Test adler32_combine */
            adler = adler32(adler, (const Bytef *)buf, (uInt)len);
            adler2 = adler32(adler2, (const Bytef *)buf, (uInt)len);
            adler = adler32_combine(adler, adler2, len32);
            (void)adler;
            break;
        }
        case 5: {
            /* Test adler32_combine64 */
            adler = adler32(adler, (const Bytef *)buf, (uInt)len);
            adler2 = adler32(adler2, (const Bytef *)buf, (uInt)len);
            adler = adler32_combine64(adler, adler2, len64);
            (void)adler;
            break;
        }
        case 6: {
            /* Test edge cases: NULL buffer for adler32_z, zero length */
            adler = adler32_z(1, NULL, 0);
            crc = crc32(0, NULL, 0);
            adler = adler32(0, NULL, 0);
            crc = crc32(0, NULL, 0);
            (void)adler;
            (void)crc;
            break;
        }
    }

    return 0;
}