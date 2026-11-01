// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// crc32_combine at crc32.c:981:15 in zlib.h
// crc32_combine_gen at crc32.c:964:15 in zlib.h
// get_crc_table at crc32.c:482:29 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// adler32_combine at adler32.c:158:15 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <zlib.h>

static void fuzz_crc32_combine_gen(const uint8_t *Data, size_t Size) {
    z_off_t len2 = 0;
    if (Size >= sizeof(z_off_t)) {
        memcpy(&len2, Data, sizeof(z_off_t));
        Data += sizeof(z_off_t);
        Size -= sizeof(z_off_t);
    }
    /* crc32_combine_gen returns a uLong (table handle), not a pointer */
    uLong table = crc32_combine_gen(len2);
    (void)table;
}

static void fuzz_get_crc_table(const uint8_t *Data, size_t Size) {
    const z_crc_t FAR *table = get_crc_table();
    /* Ensure the table is accessed to trigger any initialization side effects */
    if (table != NULL) {
        volatile z_crc_t dummy = table[0];
        (void)dummy;
    }
}

static void fuzz_crc32(const uint8_t *Data, size_t Size) {
    uLong crc = 0;
    /* Use all data as a single chunk */
    crc = crc32(crc, Data, (uInt)Size);
    (void)crc;
    
    /* Also test with a partial chunk if size is large enough */
    if (Size > 100) {
        crc = 0;
        crc = crc32(crc, Data, 100);
        crc = crc32(crc, Data + 100, (uInt)(Size - 100));
        (void)crc;
    }
}

static void fuzz_crc32_z(const uint8_t *Data, size_t Size) {
    uLong crc = 0;
    /* Use all data as a single chunk with z_size_t */
    crc = crc32_z(crc, Data, (z_size_t)Size);
    (void)crc;

    /* Test incremental processing */
    if (Size > 100) {
        crc = 0;
        crc = crc32_z(crc, Data, 100);
        crc = crc32_z(crc, Data + 100, (z_size_t)(Size - 100));
        (void)crc;
    }
}

static void fuzz_adler32_combine(const uint8_t *Data, size_t Size) {
    uLong adler1 = 1;
    uLong adler2 = 1;
    z_off_t len2 = 0;

    if (Size >= sizeof(uLong)) {
        memcpy(&adler1, Data, sizeof(uLong));
        Data += sizeof(uLong);
        Size -= sizeof(uLong);
    }
    if (Size >= sizeof(uLong)) {
        memcpy(&adler2, Data, sizeof(uLong));
        Data += sizeof(uLong);
        Size -= sizeof(uLong);
    }
    if (Size >= sizeof(z_off_t)) {
        memcpy(&len2, Data, sizeof(z_off_t));
    }

    uLong result = adler32_combine(adler1, adler2, len2);
    (void)result;
}

static void fuzz_crc32_combine(const uint8_t *Data, size_t Size) {
    uLong crc1 = 0;
    uLong crc2 = 0;
    z_off_t len2 = 0;

    if (Size >= sizeof(uLong)) {
        memcpy(&crc1, Data, sizeof(uLong));
        Data += sizeof(uLong);
        Size -= sizeof(uLong);
    }
    if (Size >= sizeof(uLong)) {
        memcpy(&crc2, Data, sizeof(uLong));
        Data += sizeof(uLong);
        Size -= sizeof(uLong);
    }
    if (Size >= sizeof(z_off_t)) {
        memcpy(&len2, Data, sizeof(z_off_t));
    }

    uLong result = crc32_combine(crc1, crc2, len2);
    (void)result;
}

int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    /* Fuzz crc32_combine_gen */
    fuzz_crc32_combine_gen(Data, Size);

    /* Fuzz get_crc_table */
    fuzz_get_crc_table(Data, Size);

    /* Fuzz crc32 */
    fuzz_crc32(Data, Size);

    /* Fuzz crc32_z */
    fuzz_crc32_z(Data, Size);

    /* Fuzz adler32_combine */
    fuzz_adler32_combine(Data, Size);

    /* Fuzz crc32_combine */
    fuzz_crc32_combine(Data, Size);

    return 0;
}