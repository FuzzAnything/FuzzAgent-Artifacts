// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// crc32_z at crc32.c:626:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// adler32_combine at adler32.c:158:15 in zlib.h
// crc32_combine at crc32.c:981:15 in zlib.h
// get_crc_table at crc32.c:482:29 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32_combine_gen at crc32.c:964:15 in zlib.h
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
        crc = crc32_z(crc, data, (z_size_t)size);
    }
    (void)crc;
}

static void test_adler32_combine(const uint8_t *data, size_t size) {
    uLong adler1 = 1;
    uLong adler2 = 1;
    z_off_t len2 = (z_off_t)size;
    if (size > 0 && len2 > 0) {
        adler32_combine(adler1, adler2, len2);
    }
    (void)adler1;
    (void)adler2;
}

static void test_crc32_combine(const uint8_t *data, size_t size) {
    uLong crc1 = 0;
    uLong crc2 = 0;
    z_off_t len2 = (z_off_t)size;
    if (size > 0 && len2 > 0) {
        crc32_combine(crc1, crc2, len2);
    }
    (void)crc1;
    (void)crc2;
}

static void test_get_crc_table(void) {
    const z_crc_t *table = get_crc_table();
    if (table != NULL) {
        volatile z_crc_t dummy = table[0];
        (void)dummy;
    }
}

static void test_crc32(const uint8_t *data, size_t size) {
    uLong crc = 0;
    if (size > 0) {
        crc = crc32(crc, data, (uInt)size);
    }
    (void)crc;
}

static void test_crc32_combine_gen(const uint8_t *data, size_t size) {
    if (size > 0) {
        uLong gen = crc32_combine_gen((z_off_t)size);
        (void)gen;
    }
}

int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    test_crc32_z(Data, Size);
    test_adler32_combine(Data, Size);
    test_crc32_combine(Data, Size);
    test_get_crc_table();
    test_crc32(Data, Size);
    test_crc32_combine_gen(Data, Size);

    // Additional diverse invocations with partial data
    size_t half = Size / 2;
    if (half > 0) {
        test_crc32_z(Data, half);
        test_crc32(Data, half);
        test_crc32_combine_gen(Data, half);
    }

    // Test with offset
    if (Size > 4) {
        test_crc32_z(Data + 1, Size - 1);
        test_crc32(Data + 2, Size - 2);
    }

    return 0;
}