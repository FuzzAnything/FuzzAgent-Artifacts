// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflateSetDictionary at inflate.c:1187:13 in zlib.h
// inflateGetDictionary at inflate.c:1167:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflateUndermine at inflate.c:1370:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflateGetDictionary at inflate.c:1167:13 in zlib.h
// inflateSetDictionary at inflate.c:1187:13 in zlib.h
// inflateGetDictionary at inflate.c:1167:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflateUndermine at inflate.c:1370:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflateSetDictionary at deflate.c:559:13 in zlib.h
// deflateGetDictionary at deflate.c:625:13 in zlib.h
// deflateGetDictionary at deflate.c:625:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateInit_ at deflate.c:379:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateSetDictionary at deflate.c:559:13 in zlib.h
// deflateGetDictionary at deflate.c:625:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

static int test_inflate_functions(const uint8_t *data, size_t size) {
    z_stream strm = {0};
    Bytef dict_buf[32768];
    uInt dict_len = 0;
    int ret;

    if (inflateInit(&strm) != Z_OK) return 0;

    // Test inflateGetDictionary before setting any dictionary
    ret = inflateGetDictionary(&strm, dict_buf, &dict_len);
    (void)ret;

    // Try to set a dictionary if we have enough data
    if (size > 0) {
        uInt dict_size = (size < 32768) ? (uInt)size : 32768;
        ret = inflateSetDictionary(&strm, (const Bytef *)data, dict_size);
        if (ret == Z_OK || ret == Z_STREAM_ERROR) {
            ret = inflateGetDictionary(&strm, dict_buf, &dict_len);
            (void)ret;
        }
    }

    // Test inflatePrime with various bit values
    int bits = (size > 0) ? (int)(data[0] % 17) : 0;
    int value = (size > 1) ? (int)(data[1] & 0xFF) : 0;
    ret = inflatePrime(&strm, bits, value);
    (void)ret;

    // Test negative bits to clear buffer
    ret = inflatePrime(&strm, -1, 0);
    (void)ret;

    // Test inflateUndermine (requires INFLATE_ALLOW_INVALID_DISTANCE_TOOFAR_ARRR)
    int subvert = (size > 2) ? (int)(data[2] % 2) : 0;
    ret = inflateUndermine(&strm, subvert);
    (void)ret;

    inflateEnd(&strm);
    return 0;
}

static int test_deflate_functions(const uint8_t *data, size_t size) {
    z_stream strm = {0};
    Bytef dict_buf[32768];
    uInt dict_len = 0;
    int ret;

    if (deflateInit(&strm, Z_DEFAULT_COMPRESSION) != Z_OK) return 0;

    // Test deflateSetDictionary
    if (size > 0) {
        uInt dict_size = (size < 32768) ? (uInt)size : 32768;
        ret = deflateSetDictionary(&strm, (const Bytef *)data, dict_size);
        if (ret == Z_OK || ret == Z_STREAM_ERROR) {
            ret = deflateGetDictionary(&strm, dict_buf, &dict_len);
            (void)ret;
        }
    }

    // Test deflateGetDictionary without setting dictionary
    ret = deflateGetDictionary(&strm, dict_buf, &dict_len);
    (void)ret;

    deflateEnd(&strm);
    return 0;
}

static int test_combined(const uint8_t *data, size_t size) {
    z_stream deflate_strm = {0};
    z_stream inflate_strm = {0};
    Bytef dict_buf[32768];
    uInt dict_len = 0;
    int ret;

    if (deflateInit(&deflate_strm, Z_DEFAULT_COMPRESSION) != Z_OK) return 0;
    if (inflateInit(&inflate_strm) != Z_OK) {
        deflateEnd(&deflate_strm);
        return 0;
    }

    if (size > 0) {
        uInt dict_size = (size < 32768) ? (uInt)size : 32768;
        
        // Set dictionary on deflate
        ret = deflateSetDictionary(&deflate_strm, (const Bytef *)data, dict_size);
        (void)ret;

        // Get dictionary from deflate
        ret = deflateGetDictionary(&deflate_strm, dict_buf, &dict_len);
        if (ret == Z_OK) {
            // Set same dictionary on inflate
            ret = inflateSetDictionary(&inflate_strm, dict_buf, dict_len);
            (void)ret;
        }

        // Get dictionary from inflate
        ret = inflateGetDictionary(&inflate_strm, dict_buf, &dict_len);
        (void)ret;
    }

    // Test inflatePrime
    int bits = (size > 0) ? (int)(data[0] % 17) : 0;
    int value = (size > 1) ? (int)(data[1] & 0xFF) : 0;
    ret = inflatePrime(&inflate_strm, bits, value);
    (void)ret;

    // Test inflateUndermine
    int subvert = (size > 2) ? (int)(data[2] % 2) : 0;
    ret = inflateUndermine(&inflate_strm, subvert);
    (void)ret;

    deflateEnd(&deflate_strm);
    inflateEnd(&inflate_strm);
    return 0;
}

int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Run different test scenarios based on input data
    switch (Data[0] % 3) {
        case 0:
            test_inflate_functions(Data, Size);
            break;
        case 1:
            test_deflate_functions(Data, Size);
            break;
        case 2:
            test_combined(Data, Size);
            break;
    }

    return 0;
}