// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// crc32_z at crc32.c:626:15 in zlib.h
// crc32_combine_gen at crc32.c:964:15 in zlib.h
// crc32_combine at crc32.c:981:15 in zlib.h
// crc32_combine64 at crc32.c:976:15 in zlib.h
// adler32_combine at adler32.c:158:15 in zlib.h
// adler32_combine64 at adler32.c:162:15 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <zlib.h>

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    if (Size < 12) {
        return 0;
    }

    /* Extract parameters from input data safely */
    uLong crc1 = 0;
    uLong crc2 = 0;
    uLong adler1 = 0;
    uLong adler2 = 0;
    z_off_t len2_32 = 0;
    z_off64_t len2_64 = 0;
    z_size_t buf_len = 0;
    const Bytef *buf = Z_NULL;

    /* Parse input to derive test parameters */
    crc1 = Data[0] | ((uLong)Data[1] << 8) | ((uLong)Data[2] << 16) | ((uLong)Data[3] << 24);
    crc2 = crc1 ^ 0x12345678UL;
    adler1 = crc1 ^ 0xDEADBEEFUL;
    adler2 = crc2 ^ 0xCAFEBABEUL;

    /* Ensure we have enough data for 32-bit length */
    if (Size >= 8) {
        len2_32 = (z_off_t)(Data[4] | ((z_off_t)Data[5] << 8) | ((z_off_t)Data[6] << 16) | ((z_off_t)Data[7] << 24));
    }

    /* Ensure we have enough data for 64-bit length */
    if (Size >= 12) {
        len2_64 = (z_off64_t)(Data[4] | ((z_off64_t)Data[5] << 8) | ((z_off64_t)Data[6] << 16) | ((z_off64_t)Data[7] << 24));
        len2_64 |= ((z_off64_t)(Data[8] | ((z_off64_t)Data[9] << 8) | ((z_off64_t)Data[10] << 16) | ((z_off64_t)Data[11] << 24)) << 32);
    }

    /* Ensure we have enough data for buffer length and buffer content */
    if (Size >= 14) {
        buf_len = (z_size_t)(Data[12] | ((z_size_t)Data[13] << 8));
        /* Cap buffer length to available data after the length field */
        if (buf_len > Size - 14) {
            buf_len = Size - 14;
        }
        if (buf_len > 0) {
            buf = Data + 14;
        }
    }

    /* Test crc32_z */
    uLong crc_z_result = crc32_z(crc1, buf, buf_len);
    (void)crc_z_result;

    /* Test crc32_combine_gen */
    /* Note: crc32_combine_gen returns a pointer to a table (uLongf*), but the prompt signature says uLong.
       We cast the result to uLong to match the prompt's signature strictly, though in real zlib it's a pointer.
       We do not dereference it to avoid potential crashes if the types mismatch in specific builds.
    */
    uLong gen_result = (uLong)crc32_combine_gen(len2_32);
    (void)gen_result;

    /* Test crc32_combine */
    uLong crc_combine_result = crc32_combine(crc1, crc2, len2_32);
    (void)crc_combine_result;

    /* Test crc32_combine64 */
    uLong crc_combine64_result = crc32_combine64(crc1, crc2, len2_64);
    (void)crc_combine64_result;

    /* Test adler32_combine */
    uLong adler_combine_result = adler32_combine(adler1, adler2, len2_32);
    (void)adler_combine_result;

    /* Test adler32_combine64 */
    uLong adler_combine64_result = adler32_combine64(adler1, adler2, len2_64);
    (void)adler_combine64_result;

    return 0;
}