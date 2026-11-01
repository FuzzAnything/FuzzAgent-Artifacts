// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflateSetDictionary at deflate.c:559:13 in zlib.h
// deflateGetDictionary at deflate.c:625:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflateSetDictionary at inflate.c:1187:13 in zlib.h
// inflateGetDictionary at inflate.c:1167:13 in zlib.h
// inflateUndermine at inflate.c:1370:13 in zlib.h
// inflateUndermine at inflate.c:1370:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

static int test_deflate_dictionary(const uint8_t *data, size_t size) {
    z_stream strm = {0};
    Bytef dict_buf[4096];
    uInt dict_len;
    int ret;

    if (deflateInit(&strm, Z_DEFAULT_COMPRESSION) != Z_OK) {
        return 0;
    }

    // Set dictionary if data is available
    if (size > 0) {
        size_t dict_size = (size > sizeof(dict_buf)) ? sizeof(dict_buf) : size;
        if (deflateSetDictionary(&strm, data, (uInt)dict_size) == Z_OK) {
            // Retrieve dictionary
            dict_len = sizeof(dict_buf);
            ret = deflateGetDictionary(&strm, dict_buf, &dict_len);
            if (ret == Z_OK) {
                // Trigger some compression to ensure state is valid
                strm.next_in = data;
                strm.avail_in = (uInt)size;
                Bytef out_buf[1024];
                strm.next_out = out_buf;
                strm.avail_out = sizeof(out_buf);
                deflate(&strm, Z_NO_FLUSH);
            }
        }
    }

    deflateEnd(&strm);
    return 0;
}

static int test_inflate_dictionary(const uint8_t *data, size_t size) {
    z_stream strm = {0};
    Bytef dict_buf[4096];
    uInt dict_len;
    int ret;

    // Initialize with default window bits
    if (inflateInit2_(&strm, 15, ZLIB_VERSION, (int)sizeof(z_stream)) != Z_OK) {
        return 0;
    }

    // Set dictionary if data is available
    if (size > 0) {
        size_t dict_size = (size > sizeof(dict_buf)) ? sizeof(dict_buf) : size;
        // Note: inflateSetDictionary usually requires Z_NEED_DICT state,
        // but calling it here tests error handling for invalid states.
        ret = inflateSetDictionary(&strm, data, (uInt)dict_size);
        (void)ret; // Ignore return for fuzzing state exploration

        // Try to get dictionary
        dict_len = sizeof(dict_buf);
        ret = inflateGetDictionary(&strm, dict_buf, &dict_len);
        (void)ret;
    }

    // Test inflateUndermine
    // This requires INFLATE_ALLOW_INVALID_DISTANCE_TOOFAR_ARRR to be defined in zlib build
    // We call it to test the API path regardless of the macro.
    inflateUndermine(&strm, 1); // Try to subvert
    inflateUndermine(&strm, 0); // Try to restore

    inflateEnd(&strm);
    return 0;
}

int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Test deflate dictionary functions
    test_deflate_dictionary(Data, Size);

    // Test inflate dictionary and undermine functions
    test_inflate_dictionary(Data, Size);

    return 0;
}