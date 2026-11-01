// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// uncompress at uncompr.c:97:13 in zlib.h
// uncompress_z at uncompr.c:92:13 in zlib.h
// uncompress2 at uncompr.c:83:13 in zlib.h
// uncompress2_z at uncompr.c:29:13 in zlib.h
// uncompress2_z at uncompr.c:29:13 in zlib.h
// uncompress2_z at uncompr.c:29:13 in zlib.h
// compressBound at compress.c:96:15 in zlib.h
// compress_z at compress.c:77:13 in zlib.h
// compress2_z at compress.c:24:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

/* Define types if not already defined in zlib.h for older versions or specific setups */
#ifndef z_size_t
typedef size_t z_size_t;
#endif

#ifndef Bytef
typedef Byte FAR Bytef;
#endif

#ifndef uLongf
typedef uLong FAR uLongf;
#endif

/* Helper to safely allocate buffers */
static void *safe_malloc(size_t size) {
    if (size == 0 || size > 1024 * 1024 * 100) { /* Limit max allocation to 100MB */
        return NULL;
    }
    void *ptr = malloc(size);
    if (ptr) {
        memset(ptr, 0, size);
    }
    return ptr;
}

int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    /* 
     * Strategy: 
     * 1. Use the input data as source for compression.
     * 2. Compress it, then use the compressed output as source for decompression.
     * 3. Test various API functions with different buffer sizes and edge cases.
     */

    /* Allocate buffers */
    /* Source buffer for compression */
    Bytef *source = (Bytef *)safe_malloc(Size);
    if (!source) return 0;
    memcpy(source, Data, Size);
    z_size_t sourceLen = Size;

    /* Destination buffer for compression (max size: sourceLen + overhead) */
    z_size_t maxCompressedSize = compressBound(sourceLen);
    Bytef *compressedBuf = (Bytef *)safe_malloc(maxCompressedSize);
    if (!compressedBuf) {
        free(source);
        return 0;
    }
    z_size_t compressedLen = maxCompressedSize;

    /* Destination buffer for decompression (initially same size as source) */
    Bytef *decompressedBuf = (Bytef *)safe_malloc(Size);
    if (!decompressedBuf) {
        free(source);
        free(compressedBuf);
        return 0;
    }
    uLong decompressedLen = Size;

    /* 
     * Test 1: compress_z 
     */
    {
        z_size_t testDestLen = compressedLen;
        int ret = compress_z(compressedBuf, &testDestLen, source, sourceLen);
        if (ret == Z_OK) {
            compressedLen = testDestLen;
        }
        /* Ignore errors, we want to test decompression on potentially invalid data too */
    }

    /* 
     * Test 2: compress2_z with various levels 
     */
    {
        z_size_t testDestLen = compressedLen;
        /* Try level 0 (store), 1 (fast), 9 (max) */
        int levels[] = {0, 1, 9};
        for (int i = 0; i < 3; i++) {
            int ret = compress2_z(compressedBuf, &testDestLen, source, sourceLen, levels[i]);
            if (ret == Z_OK) {
                compressedLen = testDestLen;
                break; /* Use the first successful compression for subsequent tests */
            }
        }
    }

    /* 
     * Test 3: uncompress 
     */
    {
        uLong testDestLen = decompressedLen;
        int ret = uncompress(decompressedBuf, &testDestLen, compressedBuf, compressedLen);
        (void)ret; /* Just exercise the function */
    }

    /* 
     * Test 4: uncompress_z 
     */
    {
        z_size_t testDestLen = decompressedLen;
        int ret = uncompress_z(decompressedBuf, &testDestLen, compressedBuf, compressedLen);
        (void)ret;
    }

    /* 
     * Test 5: uncompress2 
     */
    {
        uLong testSourceLen = compressedLen;
        uLong testDestLen = decompressedLen;
        int ret = uncompress2(decompressedBuf, &testDestLen, compressedBuf, &testSourceLen);
        (void)ret;
    }

    /* 
     * Test 6: uncompress2_z 
     */
    {
        z_size_t testSourceLen = compressedLen;
        z_size_t testDestLen = decompressedLen;
        int ret = uncompress2_z(decompressedBuf, &testDestLen, compressedBuf, &testSourceLen);
        (void)ret;
    }

    /* 
     * Edge Case: Test with NULL destination to query size (if supported by implementation)
     * Note: Standard zlib uncompress2_z might not support NULL dest for size query in all versions,
     * but we try to exercise the path.
     */
    {
        z_size_t testSourceLen = compressedLen;
        z_size_t testDestLen = 0;
        /* Try with NULL dest to see if it calculates required size */
        int ret = uncompress2_z(NULL, &testDestLen, compressedBuf, &testSourceLen);
        if (ret == Z_OK && testDestLen > 0) {
            /* Re-allocate if we got a valid size and previous buffer was too small or we want to test exact fit */
            Bytef *exactBuf = (Bytef *)safe_malloc(testDestLen);
            if (exactBuf) {
                z_size_t exactDestLen = testDestLen;
                z_size_t exactSourceLen = compressedLen;
                int ret2 = uncompress2_z(exactBuf, &exactDestLen, compressedBuf, &exactSourceLen);
                (void)ret2;
                free(exactBuf);
            }
        }
    }

    /* Cleanup */
    free(source);
    free(compressedBuf);
    free(decompressedBuf);

    return 0;
}