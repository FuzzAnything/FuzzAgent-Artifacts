// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// get_crc_table at crc32.c:482:29 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// crc32_combine_gen at crc32.c:964:15 in zlib.h
// crc32_combine at crc32.c:981:15 in zlib.h
// crc32_combine_op at crc32.c:969:15 in zlib.h
// crc32_combine_gen64 at crc32.c:954:15 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

static void fuzzer_seed_from_input(const uint8_t *Data, size_t Size, uint8_t *seed, size_t seed_len) {
    if (Size == 0) {
        memset(seed, 0, seed_len);
        return;
    }
    size_t copy_len = (Size < seed_len) ? Size : seed_len;
    memcpy(seed, Data, copy_len);
    if (seed_len > copy_len) {
        memset(seed + copy_len, 0, seed_len - copy_len);
    }
}

int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* Seed extraction for deterministic but varied inputs */
    uint8_t seed[32];
    fuzzer_seed_from_input(Data, Size, seed, sizeof(seed));

    /* Initialize CRC values */
    uLong crc1 = 0;
    uLong crc2 = 0;
    
    /* Test get_crc_table */
    const z_crc_t FAR *table = get_crc_table();
    if (table != NULL) {
        /* Just access the table to ensure it's valid and initialized */
        volatile z_crc_t val = table[0];
        (void)val;
    }

    /* Test crc32 with varying lengths */
    uInt len = (uInt)(Size % 1024);
    if (len == 0 && Size > 0) len = 1;
    if (len > Size) len = (uInt)Size;
    
    crc1 = crc32(crc1, (const Bytef *)Data, len);
    
    /* Test crc32_combine_gen */
    z_off_t len2 = (z_off_t)(Size % 10000);
    if (len2 < 0) len2 = 0;
    uLong op = crc32_combine_gen(len2);

    /* Test crc32_combine */
    crc2 = crc32_combine(crc1, crc2, len2);

    /* Test crc32_combine_op */
    uLong op_custom = (uLong)(seed[0] | (seed[1] << 8));
    if (op_custom == 0) {
        op_custom = 1; /* Ensure non-zero for meaningful test */
    }
    crc2 = crc32_combine_op(crc1, crc2, op_custom);

    /* Test crc32_combine_gen64 */
    z_off64_t len2_64 = (z_off64_t)(Size % 1000000000LL);
    if (len2_64 < 0) len2_64 = 0;
    uLong op64 = crc32_combine_gen64(len2_64);
    (void)op64; /* Use the value to prevent unused variable warnings */

    /* Additional stress test: multiple crc32 calls */
    for (size_t i = 0; i < Size && i < 100; i++) {
        crc1 = crc32(crc1, (const Bytef *)(Data + i), 1);
    }

    return 0;
}