// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflate at inflate.c:474:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflate at inflate.c:474:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflateSetDictionary at deflate.c:559:13 in zlib.h
// deflateGetDictionary at deflate.c:625:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflateSetDictionary at inflate.c:1187:13 in zlib.h
// inflateGetDictionary at inflate.c:1167:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflateUndermine at inflate.c:1370:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateInit_ at inflate.c:214:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <zlib.h>

static int fuzz_deflate_dict(const uint8_t *Data, size_t Size) {
    z_stream strm = {0};
    Bytef *dictionary = NULL;
    uInt dictLength = 0;
    int ret = Z_OK;

    if (deflateInit(&strm, Z_DEFAULT_COMPRESSION) != Z_OK) {
        return 0;
    }

    if (Size > 0) {
        dictionary = (Bytef *)malloc(Size);
        if (dictionary) {
            memcpy(dictionary, Data, Size);
            deflateSetDictionary(&strm, dictionary, (uInt)Size);
        }
    }

    if (Size > 0 && dictionary) {
        ret = deflateGetDictionary(&strm, dictionary, &dictLength);
    }

    deflateEnd(&strm);
    free(dictionary);
    return ret == Z_OK ? 1 : 0;
}

static int fuzz_inflate_dict(const uint8_t *Data, size_t Size) {
    z_stream strm = {0};
    Bytef *dictionary = NULL;
    uInt dictLength = 0;
    int ret = Z_OK;

    if (inflateInit(&strm) != Z_OK) {
        return 0;
    }

    if (Size > 0) {
        dictionary = (Bytef *)malloc(Size);
        if (dictionary) {
            memcpy(dictionary, Data, Size);
            ret = inflateSetDictionary(&strm, dictionary, (uInt)Size);
        }
    }

    if (Size > 0 && dictionary) {
        ret = inflateGetDictionary(&strm, dictionary, &dictLength);
    }

    inflateEnd(&strm);
    free(dictionary);
    return ret == Z_OK ? 1 : 0;
}

static int fuzz_inflate_undermine(const uint8_t *Data, size_t Size) {
    z_stream strm = {0};
    int ret = Z_OK;
    int subvert = 0;

    if (inflateInit(&strm) != Z_OK) {
        return 0;
    }

    if (Size > 0) {
        subvert = Data[0] & 1;
    }

    ret = inflateUndermine(&strm, subvert);

    inflateEnd(&strm);
    return ret == Z_OK ? 1 : 0;
}

static int fuzz_inflate_process(const uint8_t *Data, size_t Size) {
    z_stream strm = {0};
    Bytef *out_buf = NULL;
    int ret = Z_OK;
    size_t out_size = Size + 256;

    if (inflateInit(&strm) != Z_OK) {
        return 0;
    }

    out_buf = (Bytef *)malloc(out_size);
    if (!out_buf) {
        inflateEnd(&strm);
        return 0;
    }

    strm.next_in = (const Bytef *)Data;
    strm.avail_in = (uInt)Size;
    strm.next_out = out_buf;
    strm.avail_out = (uInt)out_size;

    ret = inflate(&strm, Z_NO_FLUSH);
    
    // Only proceed if we have more input or if the stream is not finished
    if (ret == Z_OK || ret == Z_STREAM_END) {
        // Reset input pointer to try different flush modes with same data
        strm.next_in = (const Bytef *)Data;
        strm.avail_in = (uInt)Size;
        strm.next_out = out_buf;
        strm.avail_out = (uInt)out_size;
        
        ret = inflate(&strm, Z_SYNC_FLUSH);
    }
    
    if (ret == Z_OK || ret == Z_STREAM_END) {
        // Reset input pointer again for Z_FINISH
        strm.next_in = (const Bytef *)Data;
        strm.avail_in = (uInt)Size;
        strm.next_out = out_buf;
        strm.avail_out = (uInt)out_size;
        
        ret = inflate(&strm, Z_FINISH);
    }

    inflateEnd(&strm);
    free(out_buf);
    return ret == Z_STREAM_END ? 1 : 0;
}

int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Fuzz deflate dictionary functions
    fuzz_deflate_dict(Data, Size);

    // Fuzz inflate dictionary functions
    fuzz_inflate_dict(Data, Size);

    // Fuzz inflateUndermine
    fuzz_inflate_undermine(Data, Size);

    // Fuzz inflate processing
    fuzz_inflate_process(Data, Size);

    return 0;
}