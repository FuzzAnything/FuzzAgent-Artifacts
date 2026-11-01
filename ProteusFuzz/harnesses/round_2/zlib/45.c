// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// crc32_z at crc32.c:626:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// crc32_combine64 at crc32.c:976:15 in zlib.h
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
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

static void test_adler32_z(const uint8_t *Data, size_t Size) {
    uLong adler = 0;
    if (Size > 0) {
        adler = adler32_z(adler, Data, Size);
    }
    /* Test with NULL buffer to get initial value */
    (void)adler32_z(0, Z_NULL, 0);
    /* Test incremental update */
    if (Size > 1) {
        adler = adler32_z(adler, Data + 1, Size - 1);
    }
}

static void test_crc32(const uint8_t *Data, size_t Size) {
    uLong crc = 0;
    if (Size > 0) {
        crc = crc32(crc, Data, (uInt)Size);
    }
    /* Test with NULL buffer */
    (void)crc32(0, Z_NULL, 0);
    /* Test incremental update */
    if (Size > 1) {
        crc = crc32(crc, Data + 1, (uInt)(Size - 1));
    }
}

static void test_crc32_z(const uint8_t *Data, size_t Size) {
    uLong crc = 0;
    if (Size > 0) {
        crc = crc32_z(crc, Data, Size);
    }
    /* Test with NULL buffer */
    (void)crc32_z(0, Z_NULL, 0);
    /* Test incremental update */
    if (Size > 1) {
        crc = crc32_z(crc, Data + 1, Size - 1);
    }
}

static void test_adler32_combine(const uint8_t *Data, size_t Size) {
    uLong adler1 = 0;
    uLong adler2 = 0;
    z_off_t len2 = 0;

    /* Compute checksums for two parts */
    size_t mid = Size / 2;
    if (mid > 0) {
        adler1 = adler32_z(0, Data, mid);
    }
    size_t remaining = Size - mid;
    if (remaining > 0) {
        adler2 = adler32_z(0, Data + mid, remaining);
        len2 = (z_off_t)remaining;
    }

    /* Combine them */
    if (len2 > 0) {
        (void)adler32_combine(adler1, adler2, len2);
    }
}

static void test_crc32_combine(const uint8_t *Data, size_t Size) {
    uLong crc1 = 0;
    uLong crc2 = 0;
    z_off_t len2 = 0;

    size_t mid = Size / 2;
    if (mid > 0) {
        crc1 = crc32_z(0, Data, mid);
    }
    size_t remaining = Size - mid;
    if (remaining > 0) {
        crc2 = crc32_z(0, Data + mid, remaining);
        len2 = (z_off_t)remaining;
    }

    if (len2 > 0) {
        (void)crc32_combine(crc1, crc2, len2);
    }
}

static void test_crc32_combine64(const uint8_t *Data, size_t Size) {
    uLong crc1 = 0;
    uLong crc2 = 0;
    z_off64_t len2 = 0;

    size_t mid = Size / 2;
    if (mid > 0) {
        crc1 = crc32_z(0, Data, mid);
    }
    size_t remaining = Size - mid;
    if (remaining > 0) {
        crc2 = crc32_z(0, Data + mid, remaining);
        len2 = (z_off64_t)remaining;
    }

    if (len2 > 0) {
        (void)crc32_combine64(crc1, crc2, len2);
    }
}

int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    test_adler32_z(Data, Size);
    test_crc32(Data, Size);
    test_crc32_z(Data, Size);
    test_adler32_combine(Data, Size);
    test_crc32_combine(Data, Size);
    test_crc32_combine64(Data, Size);

    return 0;
}