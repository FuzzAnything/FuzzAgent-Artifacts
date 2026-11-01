// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflate at inflate.c:474:13 in zlib.h
// inflateCopy at inflate.c:1328:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// deflateInit_ at deflate.c:379:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// deflateSetDictionary at deflate.c:559:13 in zlib.h
// deflateSetDictionary at deflate.c:559:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// inflateReset2 at inflate.c:136:13 in zlib.h
// inflateReset2 at inflate.c:136:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflateSetDictionary at deflate.c:559:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateGetDictionary at deflate.c:625:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflateSetDictionary at inflate.c:1187:13 in zlib.h
// inflateGetDictionary at inflate.c:1167:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflateSetDictionary at inflate.c:1187:13 in zlib.h
// inflateSetDictionary at inflate.c:1187:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <zlib.h>

static int fuzz_inflate_reset2(const uint8_t *Data, size_t Size) {
    z_stream strm = {0};
    int ret = Z_OK;

    if (inflateInit(&strm) != Z_OK) return 1;

    if (Size > 0) {
        int windowBits = (int)Data[0];
        ret = inflateReset2(&strm, windowBits);
    } else {
        ret = inflateReset2(&strm, 15);
    }

    inflateEnd(&strm);
    return ret;
}

static int fuzz_deflate_get_dictionary(const uint8_t *Data, size_t Size) {
    z_stream strm = {0};
    Bytef dict[32768];
    uInt dictLength = 0;
    int ret = Z_OK;
    Bytef *out_buf = NULL;

    if (deflateInit(&strm, Z_DEFAULT_COMPRESSION) != Z_OK) return 1;

    if (Size > 0) {
        deflateSetDictionary(&strm, Data, Size);
    }

    out_buf = malloc(100);
    if (out_buf) {
        strm.avail_in = 0;
        strm.next_in = NULL;
        strm.avail_out = 100;
        strm.next_out = out_buf;
        deflate(&strm, Z_NO_FLUSH);
    }

    ret = deflateGetDictionary(&strm, dict, &dictLength);

    if (out_buf) {
        free(out_buf);
    }

    deflateEnd(&strm);
    return ret;
}

static int fuzz_inflate_get_dictionary(const uint8_t *Data, size_t Size) {
    z_stream strm = {0};
    Bytef dict[32768];
    uInt dictLength = 0;
    int ret = Z_OK;

    if (inflateInit(&strm) != Z_OK) return 1;

    if (Size > 0) {
        inflateSetDictionary(&strm, Data, Size);
    }

    ret = inflateGetDictionary(&strm, dict, &dictLength);

    inflateEnd(&strm);
    return ret;
}

static int fuzz_inflate_set_dictionary(const uint8_t *Data, size_t Size) {
    z_stream strm = {0};
    int ret = Z_OK;

    if (inflateInit(&strm) != Z_OK) return 1;

    if (Size > 0) {
        ret = inflateSetDictionary(&strm, Data, Size);
    } else {
        ret = inflateSetDictionary(&strm, NULL, 0);
    }

    inflateEnd(&strm);
    return ret;
}

static int fuzz_inflate_copy(const uint8_t *Data, size_t Size) {
    z_stream source = {0};
    z_stream dest = {0};
    int ret = Z_OK;
    Bytef *out_buf = NULL;

    if (inflateInit(&source) != Z_OK) return 1;

    if (Size > 0) {
        source.next_in = Data;
        source.avail_in = Size;
        out_buf = malloc(1024);
        if (out_buf) {
            source.next_out = out_buf;
            source.avail_out = 1024;
            inflate(&source, Z_NO_FLUSH);
        }
    }

    ret = inflateCopy(&dest, &source);

    if (out_buf) {
        free(out_buf);
    }

    inflateEnd(&source);
    if (ret == Z_OK) {
        inflateEnd(&dest);
    }
    return ret;
}

static int fuzz_deflate_set_dictionary(const uint8_t *Data, size_t Size) {
    z_stream strm = {0};
    int ret = Z_OK;

    if (deflateInit(&strm, Z_DEFAULT_COMPRESSION) != Z_OK) return 1;

    if (Size > 0) {
        ret = deflateSetDictionary(&strm, Data, Size);
    } else {
        ret = deflateSetDictionary(&strm, NULL, 0);
    }

    deflateEnd(&strm);
    return ret;
}

int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size) {
    fuzz_inflate_reset2(Data, Size);
    fuzz_deflate_get_dictionary(Data, Size);
    fuzz_inflate_get_dictionary(Data, Size);
    fuzz_inflate_set_dictionary(Data, Size);
    fuzz_inflate_copy(Data, Size);
    fuzz_deflate_set_dictionary(Data, Size);

    return 0;
}