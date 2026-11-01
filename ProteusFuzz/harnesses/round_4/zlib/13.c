// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// crc32_z at crc32.c:626:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// crc32_combine64 at crc32.c:976:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// adler32_combine at adler32.c:158:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// adler32_combine64 at adler32.c:162:15 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

static void test_crc32_z(const uint8_t *data, size_t size) {
    uLong crc = crc32_z(0, Z_NULL, 0);
    if (size > 0) {
        crc = crc32_z(crc, (const Bytef *)data, (z_size_t)size);
    }
    (void)crc;
}

static void test_crc32_combine64(const uint8_t *data, size_t size) {
    uLong crc1 = 0;
    uLong crc2 = 0;
    z_off64_t len2 = 0;

    if (size >= 2) {
        size_t split = size / 2;
        crc1 = crc32_z(0, (const Bytef *)data, (z_size_t)split);
        crc2 = crc32_z(0, (const Bytef *)(data + split), (z_size_t)(size - split));
        len2 = (z_off64_t)(size - split);
        uLong combined = crc32_combine64(crc1, crc2, len2);
        (void)combined;
    }
}

static void test_crc32(const uint8_t *data, size_t size) {
    uLong crc = 0;
    if (size > 0) {
        crc = crc32(crc, (const Bytef *)data, (uInt)size);
    }
    (void)crc;
}

static void test_adler32_z(const uint8_t *data, size_t size) {
    uLong adler = adler32_z(0, Z_NULL, 0);
    if (size > 0) {
        adler = adler32_z(adler, (const Bytef *)data, (z_size_t)size);
    }
    (void)adler;
}

static void test_adler32_combine(const uint8_t *data, size_t size) {
    uLong adler1 = 0;
    uLong adler2 = 0;
    z_off_t len2 = 0;

    if (size >= 2) {
        size_t split = size / 2;
        adler1 = adler32_z(0, (const Bytef *)data, (z_size_t)split);
        adler2 = adler32_z(0, (const Bytef *)(data + split), (z_size_t)(size - split));
        len2 = (z_off_t)(size - split);
        uLong combined = adler32_combine(adler1, adler2, len2);
        (void)combined;
    }
}

static void test_adler32_combine64(const uint8_t *data, size_t size) {
    uLong adler1 = 0;
    uLong adler2 = 0;
    z_off64_t len2 = 0;

    if (size >= 2) {
        size_t split = size / 2;
        adler1 = adler32_z(0, (const Bytef *)data, (z_size_t)split);
        adler2 = adler32_z(0, (const Bytef *)(data + split), (z_size_t)(size - split));
        len2 = (z_off64_t)(size - split);
        uLong combined = adler32_combine64(adler1, adler2, len2);
        (void)combined;
    }
}

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    test_crc32_z(Data, Size);
    test_crc32_combine64(Data, Size);
    test_crc32(Data, Size);
    test_adler32_z(Data, Size);
    test_adler32_combine(Data, Size);
    test_adler32_combine64(Data, Size);

    return 0;
}