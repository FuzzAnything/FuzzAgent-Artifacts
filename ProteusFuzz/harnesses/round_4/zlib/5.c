// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflateReset2 at inflate.c:136:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflateGetHeader at inflate.c:1219:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateSetDictionary at inflate.c:1187:13 in zlib.h
// inflateSetDictionary at inflate.c:1187:13 in zlib.h
// inflateSetDictionary at inflate.c:1187:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateCopy at inflate.c:1328:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateInit2_ at inflate.c:173:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <zlib.h>

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    z_stream strm = {0};
    z_stream strm_copy = {0};
    gz_header header = {0};
    int ret;

    // Allocate buffers
    size_t input_len = Size;
    size_t output_len = Size + 1024;
    size_t dict_len = Size > 0 ? Size : 1;

    uint8_t *input_buf = (uint8_t *)malloc(input_len);
    uint8_t *output_buf = (uint8_t *)malloc(output_len);
    uint8_t *dict_buf = (uint8_t *)malloc(dict_len);

    if (!input_buf || !output_buf || !dict_buf) {
        free(input_buf);
        free(output_buf);
        free(dict_buf);
        return 0;
    }

    if (Size > 0) {
        memcpy(input_buf, Data, Size);
        memcpy(dict_buf, Data, Size);
    } else {
        dict_buf[0] = 0;
    }

    // 1. inflateInit2_
    int windowBits = -15;
    if (Size >= 1) {
        int wb_val = (Data[0] % 32) - 15;
        if (wb_val < -15) wb_val = -15;
        if (wb_val > 15) wb_val = 15;
        windowBits = wb_val;
    }
    ret = inflateInit2_(&strm, windowBits, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret != Z_OK) {
        goto cleanup;
    }

    // 2. inflateGetHeader
    ret = inflateGetHeader(&strm, &header);

    // 3. inflate
    strm.next_in = input_buf;
    strm.avail_in = (uInt)input_len;
    strm.next_out = output_buf;
    strm.avail_out = (uInt)output_len;
    ret = inflate(&strm, Z_NO_FLUSH);

    // 4. inflateSetDictionary
    ret = inflateSetDictionary(&strm, dict_buf, (uInt)dict_len);

    // 5. inflateSetDictionary
    ret = inflateSetDictionary(&strm, dict_buf, (uInt)dict_len);

    // 6. inflateSetDictionary
    ret = inflateSetDictionary(&strm, dict_buf, (uInt)dict_len);

    // 7. inflate
    strm.next_in = input_buf;
    strm.avail_in = (uInt)input_len;
    strm.next_out = output_buf;
    strm.avail_out = (uInt)output_len;
    ret = inflate(&strm, Z_NO_FLUSH);

    // 8. inflateCopy
    memset(&strm_copy, 0, sizeof(z_stream));
    ret = inflateCopy(&strm_copy, &strm);
    if (ret == Z_OK) {
        strm_copy.next_in = input_buf;
        strm_copy.avail_in = (uInt)input_len;
        strm_copy.next_out = output_buf;
        strm_copy.avail_out = (uInt)output_len;
        inflate(&strm_copy, Z_NO_FLUSH);
        inflateEnd(&strm_copy);
    }

    // 9. inflateEnd
    inflateEnd(&strm);

    // 10. inflateReset2
    // Re-init to allow reset
    ret = inflateInit2_(&strm, windowBits, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret == Z_OK) {
        int reset_wb = (Data[0] % 32) - 15;
        if (reset_wb < -15) reset_wb = -15;
        if (reset_wb > 15) reset_wb = 15;
        ret = inflateReset2(&strm, reset_wb);
        inflateEnd(&strm);
    }

    // 11. inflateEnd
    inflateEnd(&strm);

cleanup:
    free(input_buf);
    free(output_buf);
    free(dict_buf);

    return 0;
}