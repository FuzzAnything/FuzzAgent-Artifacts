// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// crc32_combine_gen at crc32.c:964:15 in zlib.h
// get_crc_table at crc32.c:482:29 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32_z at crc32.c:626:15 in zlib.h
// adler32_combine at adler32.c:158:15 in zlib.h
// crc32_combine at crc32.c:981:15 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <zlib.h>

static void test_crc32_combine_gen(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(z_off_t)) {
        return;
    }
    z_off_t len2 = *(const z_off_t *)Data;
    uLong table = crc32_combine_gen(len2);
    volatile uLong _ = table;
    (void)_;
}

static void test_get_crc_table(const uint8_t *Data, size_t Size) {
    const z_crc_t FAR *table = get_crc_table();
    if (table) {
        volatile z_crc_t sum = 0;
        for (int i = 0; i < 256; i++) {
            sum += table[i];
        }
        (void)sum;
    }
}

static void test_crc32(const uint8_t *Data, size_t Size) {
    uLong crc = 0;
    crc = crc32(crc, Data, (uInt)Size);
    volatile uLong _ = crc;
    (void)_;
}

static void test_crc32_z(const uint8_t *Data, size_t Size) {
    uLong crc = 0;
    crc = crc32_z(crc, Data, (z_size_t)Size);
    volatile uLong _ = crc;
    (void)_;
}

static void test_adler32_combine(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(uLong) * 2 + sizeof(z_off_t)) {
        return;
    }
    const uint8_t *ptr = Data;
    uLong adler1 = *(const uLong *)ptr;
    ptr += sizeof(uLong);
    uLong adler2 = *(const uLong *)ptr;
    ptr += sizeof(uLong);
    z_off_t len2 = *(const z_off_t *)ptr;
    
    uLong result = adler32_combine(adler1, adler2, len2);
    volatile uLong _ = result;
    (void)_;
}

static void test_crc32_combine(const uint8_t *Data, size_t Size) {
    if (Size < sizeof(uLong) * 2 + sizeof(z_off_t)) {
        return;
    }
    const uint8_t *ptr = Data;
    uLong crc1 = *(const uLong *)ptr;
    ptr += sizeof(uLong);
    uLong crc2 = *(const uLong *)ptr;
    ptr += sizeof(uLong);
    z_off_t len2 = *(const z_off_t *)ptr;
    
    uLong result = crc32_combine(crc1, crc2, len2);
    volatile uLong _ = result;
    (void)_;
}

int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Test crc32_combine_gen */
    test_crc32_combine_gen(Data, Size);

    /* Test get_crc_table */
    test_get_crc_table(Data, Size);

    /* Test crc32 */
    test_crc32(Data, Size);

    /* Test crc32_z */
    test_crc32_z(Data, Size);

    /* Test adler32_combine */
    test_adler32_combine(Data, Size);

    /* Test crc32_combine */
    test_crc32_combine(Data, Size);

    return 0;
}