// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// crc32 at crc32.c:946:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// crc32_combine at crc32.c:981:15 in zlib.h
// adler32_combine at adler32.c:158:15 in zlib.h
// adler32_combine64 at adler32.c:162:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

int LLVMFuzzerTestOneInput_39(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    uLong crc = 0;
    uLong adler = 0;
    uLong crc1, crc2;
    uLong adler1, adler2;
    z_off_t len2;
    z_off64_t len2_64;
    const Bytef *buf = (const Bytef *)Data;
    z_size_t len = (z_size_t)Size;

    /* Test crc32 */
    crc = crc32(crc, buf, (uInt)(len > 65535 ? 65535 : len));

    /* Test crc32_z */
    crc = crc32_z(crc, buf, len);

    /* Test adler32_z */
    adler = adler32_z(adler, buf, len);

    /* Test adler32_z with NULL buffer */
    adler = adler32_z(0, Z_NULL, 0);

    /* Prepare for combine functions */
    crc1 = crc32(0, Z_NULL, 0);
    crc2 = crc32(0, Z_NULL, 0);
    adler1 = adler32(0, Z_NULL, 0);
    adler2 = adler32(0, Z_NULL, 0);

    /* Use part of input to derive lengths for combine functions */
    if (Size >= 8) {
        len2 = *(const z_off_t *)(Data + Size - 8);
        len2_64 = *(const z_off64_t *)(Data + Size - 8);
    } else {
        len2 = (z_off_t)Size;
        len2_64 = (z_off64_t)Size;
    }

    /* Test crc32_combine */
    crc = crc32_combine(crc1, crc2, len2);

    /* Test adler32_combine */
    adler = adler32_combine(adler1, adler2, len2);

    /* Test adler32_combine64 */
    adler = adler32_combine64(adler1, adler2, len2_64);

    /* Test incremental processing with chunks */
    size_t chunk_size = Size > 1024 ? 1024 : Size;
    size_t offset = 0;
    uLong running_crc = 0;
    uLong running_adler = 0;

    while (offset < Size) {
        size_t current_chunk = (Size - offset) < chunk_size ? (Size - offset) : chunk_size;
        running_crc = crc32(running_crc, buf + offset, (uInt)current_chunk);
        running_adler = adler32_z(running_adler, buf + offset, current_chunk);
        offset += current_chunk;
    }

    return 0;
}