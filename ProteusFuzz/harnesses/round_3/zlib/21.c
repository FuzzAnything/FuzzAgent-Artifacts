// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// adler32_combine64 at adler32.c:162:15 in zlib.h
// adler32_combine64 at adler32.c:162:15 in zlib.h
// crc32_combine_gen64 at crc32.c:954:15 in zlib.h
// crc32_combine_gen64 at crc32.c:954:15 in zlib.h
// crc32_combine_op at crc32.c:969:15 in zlib.h
// crc32_combine_op at crc32.c:969:15 in zlib.h
// crc32_combine at crc32.c:981:15 in zlib.h
// crc32_combine at crc32.c:981:15 in zlib.h
// crc32_combine64 at crc32.c:976:15 in zlib.h
// crc32_combine64 at crc32.c:976:15 in zlib.h
// crc32_combine_gen at crc32.c:964:15 in zlib.h
// crc32_combine_gen at crc32.c:964:15 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <zlib.h>

static uLong fuzz_adler32_combine64(const uint8_t *Data, size_t Size) {
    if (Size < 16) {
        return adler32_combine64(1, 1, 0);
    }
    uLong crc1 = *(const uLong *)(Data + 0);
    uLong crc2 = *(const uLong *)(Data + 4);
    z_off64_t len2 = *(const z_off64_t *)(Data + 8);
    return adler32_combine64(crc1, crc2, len2);
}

static uLong fuzz_crc32_combine_gen64(const uint8_t *Data, size_t Size) {
    if (Size < 8) {
        return crc32_combine_gen64(0);
    }
    z_off64_t len2 = *(const z_off64_t *)(Data + 0);
    return crc32_combine_gen64(len2);
}

static uLong fuzz_crc32_combine_op(const uint8_t *Data, size_t Size) {
    if (Size < 12) {
        return crc32_combine_op(0, 0, 0);
    }
    uLong crc1 = *(const uLong *)(Data + 0);
    uLong crc2 = *(const uLong *)(Data + 4);
    uLong op = *(const uLong *)(Data + 8);
    return crc32_combine_op(crc1, crc2, op);
}

static uLong fuzz_crc32_combine(const uint8_t *Data, size_t Size) {
    if (Size < 12) {
        return crc32_combine(0, 0, 0);
    }
    uLong crc1 = *(const uLong *)(Data + 0);
    uLong crc2 = *(const uLong *)(Data + 4);
    z_off_t len2 = *(const z_off_t *)(Data + 8);
    return crc32_combine(crc1, crc2, len2);
}

static uLong fuzz_crc32_combine64(const uint8_t *Data, size_t Size) {
    if (Size < 16) {
        return crc32_combine64(0, 0, 0);
    }
    uLong crc1 = *(const uLong *)(Data + 0);
    uLong crc2 = *(const uLong *)(Data + 4);
    z_off64_t len2 = *(const z_off64_t *)(Data + 8);
    return crc32_combine64(crc1, crc2, len2);
}

static uLong fuzz_crc32_combine_gen(const uint8_t *Data, size_t Size) {
    if (Size < 8) {
        return crc32_combine_gen(0);
    }
    z_off_t len2 = *(const z_off_t *)(Data + 0);
    return crc32_combine_gen(len2);
}

int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Fuzz adler32_combine64
    (void)fuzz_adler32_combine64(Data, Size);

    // Fuzz crc32_combine_gen64
    (void)fuzz_crc32_combine_gen64(Data, Size);

    // Fuzz crc32_combine_op
    (void)fuzz_crc32_combine_op(Data, Size);

    // Fuzz crc32_combine
    (void)fuzz_crc32_combine(Data, Size);

    // Fuzz crc32_combine64
    (void)fuzz_crc32_combine64(Data, Size);

    // Fuzz crc32_combine_gen
    (void)fuzz_crc32_combine_gen(Data, Size);

    return 0;
}