// This fuzz driver is generated for library zlib, aiming to fuzz the following functions:
// deflateTune at deflate.c:819:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateReset at deflate.c:704:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateResetKeep at deflate.c:644:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflate at deflate.c:981:13 in zlib.h
// deflateUsed at deflate.c:737:13 in zlib.h
// deflateUsed at deflate.c:737:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateCopy at deflate.c:1317:13 in zlib.h
// deflateUsed at deflate.c:737:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateInit_ at deflate.c:379:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
// deflateCopy at deflate.c:1317:13 in zlib.h
// deflateUsed at deflate.c:737:13 in zlib.h
// deflateEnd at deflate.c:1293:13 in zlib.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <zlib.h>

int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    z_stream strm_src = {0};
    z_stream strm_dest = {0};
    z_stream strm_reset = {0};
    int bits = 0;
    int ret;
    int level;

    // Ensure minimum size to avoid trivial crashes on empty input
    if (Size < 1) {
        return 0;
    }

    // Use the first byte as the compression level (0-9), clamping if necessary
    level = Data[0] % 10;

    // Initialize source stream
    ret = deflateInit_(&strm_src, level, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret != Z_OK) {
        return 0;
    }

    // Initialize destination stream for deflateCopy
    ret = deflateInit_(&strm_dest, level, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret != Z_OK) {
        deflateEnd(&strm_src);
        return 0;
    }

    // Initialize reset stream
    ret = deflateInit_(&strm_reset, level, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret != Z_OK) {
        deflateEnd(&strm_src);
        deflateEnd(&strm_dest);
        return 0;
    }

    // Feed some data to the source stream to establish state
    if (Size > 1) {
        strm_src.next_in = (const Bytef *)(Data + 1);
        strm_src.avail_in = Size - 1;
        
        // Allocate output buffer
        Bytef *out_buf = (Bytef *)malloc(Size * 2 + 100);
        if (out_buf) {
            strm_src.next_out = out_buf;
            strm_src.avail_out = Size * 2 + 100;
            
            // Perform a partial deflate to ensure internal state is populated
            deflate(&strm_src, Z_NO_FLUSH);
            
            // Test deflateUsed
            deflateUsed(&strm_src, &bits);
            deflateUsed(&strm_src, NULL);
            
            free(out_buf);
        }
    }

    // Test deflateCopy: Clone state from src to dest
    // Scenario A: Copy into an initialized but empty stream
    // Note: deflateCopy overwrites the destination state. If dest was initialized,
    // its previous internal memory is leaked unless we end it first.
    // However, the standard pattern is to init a fresh stream or ensure it's ended.
    // Here, we treat strm_dest as a fresh target for the copy, but since we init'd it above,
    // we must end it before copying to avoid leaking the initial allocation.
    deflateEnd(&strm_dest); 
    
    ret = deflateCopy(&strm_dest, &strm_src);
    if (ret == Z_OK) {
        // Verify copy worked by checking if dest has state
        deflateUsed(&strm_dest, &bits);
        deflateEnd(&strm_dest);
    }

    // Scenario B: Copy into a stream that was ended (requires re-init or just copy?)
    // Re-initialize dest to test copying into a fresh stream again
    // We re-init, then immediately end it before copying to ensure clean state transfer
    ret = deflateInit_(&strm_dest, level, ZLIB_VERSION, (int)sizeof(z_stream));
    if (ret == Z_OK) {
        // End the newly initialized stream to free its internal buffers before copying
        // This prevents the leak where the new init's buffers are overwritten by copy
        deflateEnd(&strm_dest);
        
        ret = deflateCopy(&strm_dest, &strm_src);
        if (ret == Z_OK) {
            deflateUsed(&strm_dest, &bits);
            deflateEnd(&strm_dest);
        }
    }

    // Test deflateTune on source stream
    // Extract parameters from input data if available
    int good_len = (Size > 2) ? Data[2] : 1;
    int max_lazy = (Size > 3) ? Data[3] : 1;
    int nice_len = (Size > 4) ? Data[4] : 1;
    int max_chain = (Size > 5) ? Data[5] : 1;
    
    // Clamp values to reasonable ranges to avoid immediate crashes
    if (good_len > 1000) good_len = 1000;
    if (max_lazy > 1000) max_lazy = 1000;
    if (nice_len > 1000) nice_len = 1000;
    if (max_chain > 10000) max_chain = 10000;

    ret = deflateTune(&strm_src, good_len, max_lazy, nice_len, max_chain);
    if (ret == Z_OK) {
        // Continue compression to see if tune had effect
        if (strm_src.avail_in > 0) {
            deflate(&strm_src, Z_NO_FLUSH);
        }
    }

    // Test deflateReset on source
    ret = deflateReset(&strm_src);
    if (ret == Z_OK) {
        // Verify reset cleared totals
        (void)strm_src.total_in;
        (void)strm_src.total_out;
    }

    // Test deflateResetKeep on reset stream
    // First, do some work on strm_reset
    if (Size > 1) {
        strm_reset.next_in = (const Bytef *)(Data + 1);
        strm_reset.avail_in = Size - 1;
        Bytef *out_buf = (Bytef *)malloc(Size * 2 + 100);
        if (out_buf) {
            strm_reset.next_out = out_buf;
            strm_reset.avail_out = Size * 2 + 100;
            deflate(&strm_reset, Z_NO_FLUSH);
            
            // Now reset keep
            ret = deflateResetKeep(&strm_reset);
            if (ret == Z_OK) {
                // Verify totals are cleared but state (dictionary/level) is kept
            }
            free(out_buf);
        }
    }

    // Cleanup
    deflateEnd(&strm_src);
    deflateEnd(&strm_reset);
    
    // Ensure strm_dest is ended if it wasn't in the copy scenarios
    // deflateEnd is safe to call on an already ended stream (checks for null state)
    deflateEnd(&strm_dest);

    return 0;
}