// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// adler32_z at adler32.c:61:15 in zlib.h
// adler32_z at adler32.c:61:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// adler32 at adler32.c:128:15 in zlib.h
// crc32_combine64 at crc32.c:976:15 in zlib.h
// adler32_combine at adler32.c:158:15 in zlib.h
// adler32_combine64 at adler32.c:162:15 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <zlib.h>

static void fuzz_adler32_z(const uint8_t *Data, size_t Size) {
    uLong adler = 0;
    if (Size > 0) {
        adler = adler32_z(adler, Data, Size);
    }
    // Test with NULL buffer
    uLong null_adler = adler32_z(adler, NULL, 0);
    (void)null_adler;
}

static void fuzz_crc32(const uint8_t *Data, size_t Size) {
    uLong crc = 0;
    if (Size > 0) {
        crc = crc32(crc, Data, (uInt)Size);
    }
}

static void fuzz_adler32(const uint8_t *Data, size_t Size) {
    uLong adler = 0;
    if (Size > 0) {
        adler = adler32(adler, Data, (uInt)Size);
    }
}

static void fuzz_combine_functions(const uint8_t *Data, size_t Size) {
    uLong crc1 = 0, crc2 = 0, adler1 = 0, adler2 = 0;
    z_off_t len2_32 = 0;
    z_off64_t len2_64 = 0;

    // Safely extract values only if enough data is available
    if (Size >= sizeof(uLong)) {
        crc1 = *(const uLong*)(Data);
    }
    if (Size >= 2 * sizeof(uLong)) {
        crc2 = *(const uLong*)(Data + sizeof(uLong));
    }
    if (Size >= 3 * sizeof(uLong)) {
        adler1 = *(const uLong*)(Data + 2 * sizeof(uLong));
    }
    if (Size >= 4 * sizeof(uLong)) {
        adler2 = *(const uLong*)(Data + 3 * sizeof(uLong));
    }
    if (Size >= 5 * sizeof(uLong)) {
        len2_32 = *(const z_off_t*)(Data + 4 * sizeof(uLong));
    }
    if (Size >= 6 * sizeof(uLong)) {
        len2_64 = *(const z_off64_t*)(Data + 5 * sizeof(uLong));
    }

    // Fuzz crc32_combine64
    uLong combined_crc = crc32_combine64(crc1, crc2, len2_64);
    (void)combined_crc;

    // Fuzz adler32_combine
    uLong combined_adler_32 = adler32_combine(adler1, adler2, len2_32);
    (void)combined_adler_32;

    // Fuzz adler32_combine64
    uLong combined_adler_64 = adler32_combine64(adler1, adler2, len2_64);
    (void)combined_adler_64;
}

int LLVMFuzzerTestOneInput_50(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        // Handle empty input gracefully
        fuzz_adler32_z(NULL, 0);
        fuzz_crc32(NULL, 0);
        fuzz_adler32(NULL, 0);
        fuzz_combine_functions(NULL, 0);
        return 0;
    }

    fuzz_adler32_z(Data, Size);
    fuzz_crc32(Data, Size);
    fuzz_adler32(Data, Size);
    fuzz_combine_functions(Data, Size);

    return 0;
}