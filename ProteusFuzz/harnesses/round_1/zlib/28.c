// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflateInit_ at inflate.c:214:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// inflateSetDictionary at inflate.c:1187:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateGetDictionary at inflate.c:1167:13 in zlib.h
// deflateInit2_ at deflate.c:387:13 in zlib.h
// deflateSetDictionary at deflate.c:559:13 in zlib.h
// deflateGetDictionary at deflate.c:625:13 in zlib.h
// deflatePrime at deflate.c:745:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <zlib.h>

static void free_z_stream(z_stream *strm) {
    if (strm) {
        deflateEnd(strm);
        inflateEnd(strm);
        free(strm);
    }
}

int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Allocate stream structures
    z_stream *deflate_strm = (z_stream *)calloc(1, sizeof(z_stream));
    z_stream *inflate_strm = (z_stream *)calloc(1, sizeof(z_stream));
    
    if (!deflate_strm || !inflate_strm) {
        free_z_stream(deflate_strm);
        free_z_stream(inflate_strm);
        return 0;
    }

    // Prepare buffers
    uint8_t *in_buf = (uint8_t *)malloc(Size);
    uint8_t *out_buf = (uint8_t *)malloc(Size * 2 + 256);
    uint8_t *dict_buf = (uint8_t *)malloc(32768); // Max dictionary size
    uint8_t *retrieved_dict = (uint8_t *)malloc(32768);
    
    if (!in_buf || !out_buf || !dict_buf || !retrieved_dict) {
        free_z_stream(deflate_strm);
        free_z_stream(inflate_strm);
        free(in_buf);
        free(out_buf);
        free(dict_buf);
        free(retrieved_dict);
        return 0;
    }

    memcpy(in_buf, Data, Size);

    // Helper to safely access data
    #define GET_BYTE(idx) ((idx) < (size_t)Size ? Data[(idx)] : 0)

    // Fuzz deflateInit2_
    int level = GET_BYTE(0) % 10;
    int method = (GET_BYTE(1) % 2) ? Z_DEFLATED : Z_DEFLATED; 
    int windowBits = (GET_BYTE(2) % 16) + 8; 
    int memLevel = (GET_BYTE(3) % 9) + 1;
    int strategy = GET_BYTE(4) % 4;
    int stream_size = sizeof(z_stream);
    const char *version = ZLIB_VERSION;

    deflate_strm->zalloc = Z_NULL;
    deflate_strm->zfree = Z_NULL;
    deflate_strm->opaque = Z_NULL;

    int ret = deflateInit2_(deflate_strm, level, method, windowBits, memLevel, strategy, version, stream_size);
    if (ret != Z_OK) {
        // Invalid init, try to recover or just continue with other tests
    }

    // Fuzz deflateSetDictionary
    uInt dictLen = (Size > 100) ? (uInt)(Size / 2) : (uInt)Size;
    if (dictLen > 32768) dictLen = 32768;
    if (dictLen > 0) {
        memcpy(dict_buf, Data, dictLen);
        ret = deflateSetDictionary(deflate_strm, dict_buf, dictLen);
        (void)ret; 
    }

    // Fuzz deflateGetDictionary
    uInt dictLength = 0;
    if (deflate_strm->state) {
        ret = deflateGetDictionary(deflate_strm, retrieved_dict, &dictLength);
        (void)ret;
    }

    // Fuzz deflatePrime
    int bits = GET_BYTE(5) % 17; 
    int value = (GET_BYTE(6) << 8) | GET_BYTE(7); 
    if (deflate_strm->state) {
        ret = deflatePrime(deflate_strm, bits, value);
        (void)ret;
    }

    // Perform some compression to generate data for inflate
    deflate_strm->next_in = in_buf;
    deflate_strm->avail_in = Size;
    deflate_strm->next_out = out_buf;
    deflate_strm->avail_out = Size * 2 + 256;

    ret = deflate(deflate_strm, Z_FINISH);
    (void)ret;

    // Fuzz inflate side
    inflate_strm->zalloc = Z_NULL;
    inflate_strm->zfree = Z_NULL;
    inflate_strm->opaque = Z_NULL;

    ret = inflateInit(inflate_strm);
    if (ret != Z_OK) {
        free_z_stream(deflate_strm);
        free_z_stream(inflate_strm);
        free(in_buf);
        free(out_buf);
        free(dict_buf);
        free(retrieved_dict);
        return 0;
    }

    // Fuzz inflateSetDictionary (only if we have a dictionary and stream is in right state)
    if (dictLen > 0 && (GET_BYTE(8) % 2)) {
        ret = inflateSetDictionary(inflate_strm, dict_buf, dictLen);
        (void)ret;
    }

    // Fuzz inflateGetDictionary
    dictLength = 0;
    ret = inflateGetDictionary(inflate_strm, retrieved_dict, &dictLength);
    (void)ret;

    // Cleanup
    free_z_stream(deflate_strm);
    free_z_stream(inflate_strm);
    free(in_buf);
    free(out_buf);
    free(dict_buf);
    free(retrieved_dict);

    return 0;
}