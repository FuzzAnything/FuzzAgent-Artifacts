// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// adler32 at adler32.c:128:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// adler32_combine at adler32.c:158:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// adler32_combine64 at adler32.c:162:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
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

static void fuzz_adler32(const uint8_t *data, size_t size) {
    uLong adler = 0;
    const Bytef *buf = (const Bytef *)data;
    uInt len = (size_t)size;
    if (len > UINT_MAX) len = UINT_MAX;
    adler = adler32(adler, buf, len);
    (void)adler;
}

static void fuzz_adler32_z(const uint8_t *data, size_t size) {
    uLong adler = 0;
    const Bytef *buf = (const Bytef *)data;
    z_size_t len = (z_size_t)size;
    adler = adler32_z(adler, buf, len);
    (void)adler;
}

static void fuzz_adler32_combine(const uint8_t *data, size_t size) {
    if (size < 2) return;
    size_t mid = size / 2;
    uLong adler1 = adler32(0, (const Bytef *)data, (uInt)mid);
    uLong adler2 = adler32(0, (const Bytef *)(data + mid), (uInt)(size - mid));
    z_off_t len2 = (z_off_t)(size - mid);
    uLong combined = adler32_combine(adler1, adler2, len2);
    (void)combined;
}

static void fuzz_adler32_combine64(const uint8_t *data, size_t size) {
    if (size < 2) return;
    size_t mid = size / 2;
    uLong adler1 = adler32(0, (const Bytef *)data, (uInt)mid);
    uLong adler2 = adler32(0, (const Bytef *)(data + mid), (uInt)(size - mid));
    z_off64_t len2 = (z_off64_t)(size - mid);
    uLong combined = adler32_combine64(adler1, adler2, len2);
    (void)combined;
}

static void fuzz_crc32(const uint8_t *data, size_t size) {
    uLong crc = 0;
    const Bytef *buf = (const Bytef *)data;
    uInt len = (size_t)size;
    if (len > UINT_MAX) len = UINT_MAX;
    crc = crc32(crc, buf, len);
    (void)crc;
}

static void fuzz_crc32_combine64(const uint8_t *data, size_t size) {
    if (size < 2) return;
    size_t mid = size / 2;
    uLong crc1 = crc32(0, (const Bytef *)data, (uInt)mid);
    uLong crc2 = crc32(0, (const Bytef *)(data + mid), (uInt)(size - mid));
    z_off64_t len2 = (z_off64_t)(size - mid);
    uLong combined = crc32_combine64(crc1, crc2, len2);
    (void)combined;
}

int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    fuzz_adler32(Data, Size);
    fuzz_adler32_z(Data, Size);
    fuzz_adler32_combine(Data, Size);
    fuzz_adler32_combine64(Data, Size);
    fuzz_crc32(Data, Size);
    fuzz_crc32_combine64(Data, Size);

    return 0;
}