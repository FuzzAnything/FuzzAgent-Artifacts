// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// adler32_combine at adler32.c:158:15 in zlib.h
// adler32_combine64 at adler32.c:162:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32_combine at crc32.c:981:15 in zlib.h
// crc32_combine64 at crc32.c:976:15 in zlib.h
// crc32_combine_gen at crc32.c:964:15 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <zlib.h>

static void fuzz_adler32_combine(const uint8_t *Data, size_t Size) {
    uLong adler1 = 0;
    uLong adler2 = 0;
    z_off_t len2 = 0;

    if (Size >= sizeof(uLong)) {
        memcpy(&adler1, Data, sizeof(uLong));
    }
    if (Size >= sizeof(uLong) * 2) {
        memcpy(&adler2, Data + sizeof(uLong), sizeof(uLong));
    }
    if (Size >= sizeof(uLong) * 2 + sizeof(z_off_t)) {
        memcpy(&len2, Data + sizeof(uLong) * 2, sizeof(z_off_t));
    }

    uLong result = adler32_combine(adler1, adler2, len2);
    (void)result;
}

static void fuzz_adler32_combine64(const uint8_t *Data, size_t Size) {
    uLong adler1 = 0;
    uLong adler2 = 0;
    z_off64_t len2 = 0;

    if (Size >= sizeof(uLong)) {
        memcpy(&adler1, Data, sizeof(uLong));
    }
    if (Size >= sizeof(uLong) * 2) {
        memcpy(&adler2, Data + sizeof(uLong), sizeof(uLong));
    }
    if (Size >= sizeof(uLong) * 2 + sizeof(z_off64_t)) {
        memcpy(&len2, Data + sizeof(uLong) * 2, sizeof(z_off64_t));
    }

    uLong result = adler32_combine64(adler1, adler2, len2);
    (void)result;
}

static void fuzz_crc32(const uint8_t *Data, size_t Size) {
    uLong crc = 0;
    if (Size > 0) {
        crc = crc32(crc, Data, (uInt)Size);
    }
    (void)crc;
}

static void fuzz_crc32_combine(const uint8_t *Data, size_t Size) {
    uLong crc1 = 0;
    uLong crc2 = 0;
    z_off_t len2 = 0;

    if (Size >= sizeof(uLong)) {
        memcpy(&crc1, Data, sizeof(uLong));
    }
    if (Size >= sizeof(uLong) * 2) {
        memcpy(&crc2, Data + sizeof(uLong), sizeof(uLong));
    }
    if (Size >= sizeof(uLong) * 2 + sizeof(z_off_t)) {
        memcpy(&len2, Data + sizeof(uLong) * 2, sizeof(z_off_t));
    }

    uLong result = crc32_combine(crc1, crc2, len2);
    (void)result;
}

static void fuzz_crc32_combine64(const uint8_t *Data, size_t Size) {
    uLong crc1 = 0;
    uLong crc2 = 0;
    z_off64_t len2 = 0;

    if (Size >= sizeof(uLong)) {
        memcpy(&crc1, Data, sizeof(uLong));
    }
    if (Size >= sizeof(uLong) * 2) {
        memcpy(&crc2, Data + sizeof(uLong), sizeof(uLong));
    }
    if (Size >= sizeof(uLong) * 2 + sizeof(z_off64_t)) {
        memcpy(&len2, Data + sizeof(uLong) * 2, sizeof(z_off64_t));
    }

    uLong result = crc32_combine64(crc1, crc2, len2);
    (void)result;
}

static void fuzz_crc32_combine_gen(const uint8_t *Data, size_t Size) {
    z_off_t len2 = 0;
    if (Size >= sizeof(z_off_t)) {
        memcpy(&len2, Data, sizeof(z_off_t));
    }

    uLong table = crc32_combine_gen(len2);
    (void)table;
}

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    fuzz_crc32(Data, Size);
    fuzz_adler32_combine(Data, Size);
    fuzz_adler32_combine64(Data, Size);
    fuzz_crc32_combine(Data, Size);
    fuzz_crc32_combine64(Data, Size);
    fuzz_crc32_combine_gen(Data, Size);

    return 0;
}