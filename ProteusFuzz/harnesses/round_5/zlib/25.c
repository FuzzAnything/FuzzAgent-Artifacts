// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// deflateGetDictionary at deflate.c:625:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateSetDictionary at inflate.c:1187:13 in zlib.h
// inflateGetDictionary at inflate.c:1167:13 in zlib.h
// inflatePrime at inflate.c:219:13 in zlib.h
// inflateUndermine at inflate.c:1370:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateSetDictionary at deflate.c:559:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <zlib.h>

static int test_inflate_functions(const uint8_t *data, size_t size) {
    z_stream strm = {0};
    Bytef *out_buf;
    uInt dict_len = 0;
    Bytef dict_buf[32768];
    int ret;

    if (inflateInit(&strm) != Z_OK) return 0;

    // Allocate output buffer
    size_t out_size = size > 0 ? size * 2 : 1024;
    out_buf = (Bytef *)malloc(out_size);
    if (!out_buf) { inflateEnd(&strm); return 0; }

    strm.next_out = out_buf;
    strm.avail_out = (uInt)out_size;

    // Use fuzz data as input
    if (size > 0) {
        strm.next_in = (const Bytef *)data;
        strm.avail_in = (uInt)size;
    }

    // Test inflateSetDictionary
    if (size > 0 && size < sizeof(dict_buf)) {
        // Try to set dictionary
        ret = inflateSetDictionary(&strm, (const Bytef *)data, (uInt)size);
        (void)ret;
    }

    // Test inflateGetDictionary
    dict_len = sizeof(dict_buf);
    ret = inflateGetDictionary(&strm, dict_buf, &dict_len);
    (void)ret;

    // Test inflatePrime with various bit values
    int bits = 0;
    int value = 0;
    if (size > 0) {
        bits = data[0] % 17;
    }
    if (size > 2) {
        value = (data[1] << 8) | data[2];
    }
    ret = inflatePrime(&strm, bits, value);
    (void)ret;

    // Test inflateUndermine
    int subvert = 0;
    if (size > 3) {
        subvert = data[3] & 1;
    }
    ret = inflateUndermine(&strm, subvert);
    (void)ret;

    // Perform some inflate steps to exercise state
    while (strm.avail_in > 0 || strm.avail_out > 0) {
        ret = inflate(&strm, Z_NO_FLUSH);
        if (ret == Z_STREAM_END || ret < 0) break;
    }

    inflateEnd(&strm);
    free(out_buf);
    return 0;
}

static int test_deflate_functions(const uint8_t *data, size_t size) {
    z_stream strm = {0};
    Bytef *out_buf;
    uInt dict_len = 0;
    Bytef dict_buf[32768];
    int ret;

    if (deflateInit(&strm, Z_DEFAULT_COMPRESSION) != Z_OK) return 0;

    // Allocate output buffer
    size_t out_size = size > 0 ? size * 2 : 1024;
    out_buf = (Bytef *)malloc(out_size);
    if (!out_buf) { deflateEnd(&strm); return 0; }

    strm.next_out = out_buf;
    strm.avail_out = (uInt)out_size;

    // Use fuzz data as input
    if (size > 0) {
        strm.next_in = (const Bytef *)data;
        strm.avail_in = (uInt)size;
    }

    // Test deflateSetDictionary
    if (size > 0 && size < sizeof(dict_buf)) {
        ret = deflateSetDictionary(&strm, (const Bytef *)data, (uInt)size);
        (void)ret;
    }

    // Test deflateGetDictionary
    dict_len = sizeof(dict_buf);
    ret = deflateGetDictionary(&strm, dict_buf, &dict_len);
    (void)ret;

    // Perform some deflate steps
    while (strm.avail_in > 0) {
        ret = deflate(&strm, Z_NO_FLUSH);
        if (ret < 0) break;
    }
    deflateEnd(&strm);
    free(out_buf);
    return 0;
}

int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Fuzz inflate related functions
    test_inflate_functions(Data, Size);

    // Fuzz deflate related functions
    test_deflate_functions(Data, Size);

    return 0;
}