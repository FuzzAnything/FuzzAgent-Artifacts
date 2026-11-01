// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// deflateParams at deflate.c:774:13 in zlib.h
// deflateTune at deflate.c:819:13 in zlib.h
// deflatePrime at deflate.c:745:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateInit2_ at deflate.c:387:13 in zlib.h
// deflateCopy at deflate.c:1317:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <zlib.h>

static int fuzz_deflate_stream(z_streamp strm, const uint8_t *Data, size_t Size) {
    if (strm == NULL) return 1;

    uint8_t *out_buf = NULL;
    size_t out_size = 0;

    // Initialize output buffer if needed
    if (Size > 0) {
        out_size = Size * 2 + 1024;
        out_buf = (uint8_t *)malloc(out_size);
        if (out_buf == NULL) return 1;
        strm->next_out = out_buf;
        strm->avail_out = (uInt)out_size;
    }

    // Set input data
    if (Size > 0) {
        strm->next_in = Data;
        strm->avail_in = (uInt)Size;
    } else {
        strm->next_in = NULL;
        strm->avail_in = 0;
    }

    // Try deflateParams
    int level = (Data[0] % 10) - 1; // -1 to 8
    int strategy = Data[1] % 3;     // 0, 1, 2
    deflateParams(strm, level, strategy);

    // Try deflateTune
    int good_length = Data[2] % 100;
    int max_lazy = Data[3] % 100;
    int nice_length = Data[4] % 100;
    int max_chain = Data[5] % 100;
    deflateTune(strm, good_length, max_lazy, nice_length, max_chain);

    // Try deflatePrime
    int bits = Data[6] % 17; // 0-16
    int value = Data[7] & 0xFF;
    deflatePrime(strm, bits, value);

    // Perform compression with various flush modes
    // Z_TREES_FLUSH is not available in all zlib versions, so we omit it
    int flush_modes[] = {Z_NO_FLUSH, Z_SYNC_FLUSH, Z_FULL_FLUSH, Z_BLOCK, Z_FINISH};
    int num_modes = 5;
    for (int i = 0; i < num_modes; i++) {
        int flush = flush_modes[i];
        if (strm->avail_out == 0) break;
        deflate(strm, flush);
    }

    // Cleanup
    if (out_buf) {
        free(out_buf);
    }

    return 0;
}

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    if (Size < 8) return 0;

    z_stream source = {0};
    z_stream dest = {0};

    // Initialize source stream
    int ret = deflateInit2_(&source, 
                            Data[0] % 10 - 1, // level
                            Z_DEFLATED,        // method
                            (Data[1] % 16) - 8, // windowBits
                            (Data[2] % 9) + 1,  // memLevel
                            Data[3] % 3,        // strategy
                            ZLIB_VERSION,
                            sizeof(z_stream));
    if (ret != Z_OK) return 0;

    // Run fuzzing logic on source
    fuzz_deflate_stream(&source, Data, Size);

    // Copy stream state to dest
    ret = deflateCopy(&dest, &source);
    if (ret == Z_OK) {
        // Run fuzzing logic on dest
        fuzz_deflate_stream(&dest, Data, Size);
    }

    // Cleanup
    deflateEnd(&source);
    deflateEnd(&dest);

    return 0;
}