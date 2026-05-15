// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// inflatePrime at inflate.c:219:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// deflateInit2_ at deflate.c:387:13 in zlib.h
// deflateSetHeader at deflate.c:714:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflateReset2 at inflate.c:136:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflateGetHeader at inflate.c:1219:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateInit2_ at inflate.c:173:13 in zlib.h
// inflateSetDictionary at inflate.c:1187:13 in zlib.h
// inflateEnd at inflate.c:1155:13 in zlib.h
// inflateInit2_ at inflate.c:173:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "zlib.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void init_z_stream(z_streamp strm) {
    strm->zalloc = Z_NULL;
    strm->zfree = Z_NULL;
    strm->opaque = Z_NULL;
    strm->next_in = Z_NULL;
    strm->avail_in = 0;
}

static void init_gz_header(gz_headerp head) {
    head->text = 0;
    head->time = 0;
    head->xflags = 0;
    head->os = 0;
    head->extra = Z_NULL;
    head->extra_len = 0;
    head->extra_max = 0;
    head->name = Z_NULL;
    head->name_max = 0;
    head->comment = Z_NULL;
    head->comm_max = 0;
    head->hcrc = 0;
    head->done = 0;
}

int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    z_stream strm;
    gz_header head;
    int ret;
    uint8_t func_selector = Data[0];
    const uint8_t *func_data = Data + 1;
    size_t func_size = Size - 1;

    init_z_stream(&strm);
    init_gz_header(&head);

    switch (func_selector % 6) {
        case 0: { /* deflateSetHeader */
            ret = deflateInit2(&strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
                               15 + 16, 8, Z_DEFAULT_STRATEGY);
            if (ret != Z_OK) break;

            if (func_size > 0) {
                head.text = func_data[0] & 1;
                head.time = (func_size > 4) ? *(uint32_t*)(func_data + 1) : 0;
                head.os = (func_size > 5) ? func_data[5] : 0;
            }
            ret = deflateSetHeader(&strm, &head);
            deflateEnd(&strm);
            break;
        }
        case 1: { /* inflateReset2 */
            ret = inflateInit2(&strm, 15 + 16);
            if (ret != Z_OK) break;

            int windowBits = (func_size > 0) ? (int)(func_data[0] % 32) : 15;
            ret = inflateReset2(&strm, windowBits);
            inflateEnd(&strm);
            break;
        }
        case 2: { /* inflateGetHeader */
            ret = inflateInit2(&strm, 15 + 16);
            if (ret != Z_OK) break;

            if (func_size > 0) {
                head.extra_max = (func_size > 1) ? func_data[1] : 0;
                head.name_max = (func_size > 2) ? func_data[2] : 0;
                head.comm_max = (func_size > 3) ? func_data[3] : 0;
            }
            ret = inflateGetHeader(&strm, &head);
            inflateEnd(&strm);
            break;
        }
        case 3: { /* inflateSetDictionary */
            ret = inflateInit2(&strm, 15);
            if (ret != Z_OK) break;

            uInt dictLength = (func_size > 1024) ? 1024 : (uInt)func_size;
            ret = inflateSetDictionary(&strm, func_data, dictLength);
            inflateEnd(&strm);
            break;
        }
        case 4: { /* inflatePrime */
            ret = inflateInit2(&strm, 15);
            if (ret != Z_OK) break;

            int bits = (func_size > 0) ? (func_data[0] % 17) : 0;
            int value = (func_size > 1) ? func_data[1] : 0;
            ret = inflatePrime(&strm, bits, value);
            inflateEnd(&strm);
            break;
        }
        case 5: { /* inflateInit2_ */
            int windowBits = (func_size > 0) ? (int)(func_data[0] % 32) : 15;
            ret = inflateInit2_(&strm, windowBits, ZLIB_VERSION, sizeof(z_stream));
            if (ret == Z_OK) inflateEnd(&strm);
            break;
        }
    }

    return 0;
}