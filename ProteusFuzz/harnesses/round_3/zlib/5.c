// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// deflateInit2_ at deflate.c:387:13 in zlib.h
// deflateSetDictionary at deflate.c:559:13 in zlib.h
// deflatePrime at deflate.c:745:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// inflateInit2_ at inflate.c:173:13 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// crc32 at crc32.c:946:15 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <zlib.h>

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    z_stream strm = {0};
    uint8_t *input_buf = NULL;
    uint8_t *output_buf = NULL;
    uint8_t *dict_buf = NULL;
    uLong crc;
    int ret;
    int window_bits;
    int bits;
    int value;
    size_t data_len = 0;

    if (Size < 1) {
        return 0;
    }

    // Parse parameters from input to diversify states
    if (Size >= 3) {
        window_bits = (Data[0] % 15) + 1; // 1 to 15
        bits = (Data[1] % 17); // 0 to 16
        value = (Data[2] % 65536); // 0 to 65535
        data_len = Size - 3;
    } else {
        window_bits = 15;
        bits = 0;
        value = 0;
        data_len = 0;
    }

    // Allocate buffers
    if (data_len > 0) {
        input_buf = malloc(data_len);
        output_buf = malloc(data_len);
        dict_buf = malloc(data_len);
        if (!input_buf || !output_buf || !dict_buf) {
            free(input_buf);
            free(output_buf);
            free(dict_buf);
            return 0;
        }
        memcpy(input_buf, Data + 3, data_len);
        memcpy(dict_buf, Data + 3, data_len);
    } else {
        // Allocate minimal buffer if no data available
        input_buf = malloc(1);
        output_buf = malloc(1);
        dict_buf = malloc(1);
        if (!input_buf || !output_buf || !dict_buf) {
            free(input_buf);
            free(output_buf);
            free(dict_buf);
            return 0;
        }
        input_buf[0] = 0;
        dict_buf[0] = 0;
    }

    // 1. inflateInit2_
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    
    ret = inflateInit2_(&strm, window_bits, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret != Z_OK) {
        goto cleanup;
    }

    // 2. crc32 (before inflate)
    crc = crc32(0L, input_buf, data_len > 0 ? data_len : 1);

    // 3. inflate
    strm.next_in = input_buf;
    strm.avail_in = data_len > 0 ? data_len : 1;
    strm.next_out = output_buf;
    strm.avail_out = data_len > 0 ? data_len : 1;

    ret = inflate(&strm, Z_NO_FLUSH);
    // We don't care about the return value for fuzzing, just need to exercise the path

    // 4. crc32 (after inflate)
    // Only compute if we have output
    if (strm.total_out > 0) {
        crc = crc32(crc, output_buf, strm.total_out);
    }

    // 5. inflateEnd
    ret = inflateEnd(&strm);
    (void)ret; // Ignore return for fuzzing stability

    // 6. deflateInit2_
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    strm.avail_out = 0;
    strm.next_out = Z_NULL;

    ret = deflateInit2_(&strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED, 
                        window_bits, 8, Z_DEFAULT_STRATEGY, 
                        ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret != Z_OK) {
        goto cleanup;
    }

    // 7. deflateSetDictionary
    if (dict_buf) {
        ret = deflateSetDictionary(&strm, dict_buf, data_len > 0 ? data_len : 1);
        (void)ret;
    }

    // 8. deflatePrime
    ret = deflatePrime(&strm, bits, value);
    (void)ret;

    // Cleanup deflate stream to prevent memory leaks
    ret = deflateEnd(&strm);
    (void)ret;

cleanup:
    free(input_buf);
    free(output_buf);
    free(dict_buf);
    return 0;
}