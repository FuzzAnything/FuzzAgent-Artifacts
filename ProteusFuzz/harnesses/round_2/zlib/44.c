// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// adler32_z at adler32.c:61:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// adler32_combine64 at adler32.c:162:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// crc32_combine at crc32.c:981:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// crc32_combine64 at crc32.c:976:15 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

static void test_adler32_z(const uint8_t *data, size_t size) {
    uLong adler = adler32_z(0L, Z_NULL, 0);
    if (size > 0) {
        adler = adler32_z(adler, (const Bytef *)data, (z_size_t)size);
    }
    (void)adler;
}

static void test_adler32_combine64(const uint8_t *data, size_t size) {
    if (size < 2) return;
    size_t mid = size / 2;
    uLong adler1 = adler32_z(0L, (const Bytef *)data, (z_size_t)mid);
    uLong adler2 = adler32_z(0L, (const Bytef *)(data + mid), (z_size_t)(size - mid));
    uLong combined = adler32_combine64(adler1, adler2, (z_off64_t)(size - mid));
    (void)combined;
}

static void test_crc32(const uint8_t *data, size_t size) {
    uLong crc = crc32(0L, Z_NULL, 0);
    if (size > 0) {
        crc = crc32(crc, (const Bytef *)data, (uInt)(size > 65535 ? 65535 : size));
    }
    (void)crc;
}

static void test_crc32_z(const uint8_t *data, size_t size) {
    uLong crc = crc32_z(0L, Z_NULL, 0);
    if (size > 0) {
        crc = crc32_z(crc, (const Bytef *)data, (z_size_t)size);
    }
    (void)crc;
}

static void test_crc32_combine(const uint8_t *data, size_t size) {
    if (size < 2) return;
    size_t mid = size / 2;
    uLong crc1 = crc32_z(0L, (const Bytef *)data, (z_size_t)mid);
    uLong crc2 = crc32_z(0L, (const Bytef *)(data + mid), (z_size_t)(size - mid));
    uLong combined = crc32_combine(crc1, crc2, (z_off_t)(size - mid));
    (void)combined;
}

static void test_crc32_combine64(const uint8_t *data, size_t size) {
    if (size < 2) return;
    size_t mid = size / 2;
    uLong crc1 = crc32_z(0L, (const Bytef *)data, (z_size_t)mid);
    uLong crc2 = crc32_z(0L, (const Bytef *)(data + mid), (z_size_t)(size - mid));
    uLong combined = crc32_combine64(crc1, crc2, (z_off64_t)(size - mid));
    (void)combined;
}

int LLVMFuzzerTestOneInput_44(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    test_adler32_z(Data, Size);
    test_adler32_combine64(Data, Size);
    test_crc32(Data, Size);
    test_crc32_z(Data, Size);
    test_crc32_combine(Data, Size);
    test_crc32_combine64(Data, Size);

    return 0;
}