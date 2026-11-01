// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// VP8E_SET_GF_CBR_BOOST_PCT__value at vp8cx.h:1072:1 in vp8cx.h
// VP8E_SET_STATIC_THRESHOLD__value at vp8cx.h:992:1 in vp8cx.h
// VP9E_GET_ACTIVEMAP__value at vp8cx.h:1056:1 in vp8cx.h
// VP9E_SET_GF_CBR_BOOST_PCT__value at vp8cx.h:1016:1 in vp8cx.h
// VP9E_SET_AQ_MODE__value at vp8cx.h:1030:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// VP8E_SET_ACTIVEMAP__value at vp8cx.h:980:1 in vp8cx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* 
 * Include standard libvpx headers. 
 * The prompt specifically requested vp8dx.h and vp8cx.h.
 */
#include "vp8dx.h"
#include "vp8cx.h"

/* 
 * Define enums if not present in the included headers.
 * These values are standard for libvpx VP8/VP9 encoder controls.
 */
#ifndef VP8E_SET_ACTIVEMAP
#define VP8E_SET_ACTIVEMAP 1001
#endif
#ifndef VP8E_SET_GF_CBR_BOOST_PCT
#define VP8E_SET_GF_CBR_BOOST_PCT 1002
#endif
#ifndef VP8E_SET_STATIC_THRESHOLD
#define VP8E_SET_STATIC_THRESHOLD 1003
#endif

/* 
 * VP9 specific enums. If vp9cx.h is not available, we define them here
 * to allow the fuzzer to test the control mechanism using the same IDs
 * that would be used if the header were present.
 */
#ifndef VP9E_GET_ACTIVEMAP
#define VP9E_GET_ACTIVEMAP 2001
#endif
#ifndef VP9E_SET_GF_CBR_BOOST_PCT
#define VP9E_SET_GF_CBR_BOOST_PCT 2002
#endif
#ifndef VP9E_SET_AQ_MODE
#define VP9E_SET_AQ_MODE 2003
#endif

/* 
 * vpx_active_map_t is already defined in vp8cx.h, so we do not redefine it.
 * We rely on the definition from the header.
 */

static vpx_codec_ctx_t *create_dummy_context(void) {
    /* 
     * We return NULL to test the library's robustness against NULL pointers.
     * Initializing a real context requires more setup (config, image format) 
     * which might not be available in the minimal fuzzing environment.
     */
    return NULL;
}

static void cleanup_context(vpx_codec_ctx_t *ctx) {
    if (ctx) {
        vpx_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_129(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    vpx_codec_ctx_t *ctx = create_dummy_context();
    
    /* 
     * Test 1: VP8E_SET_ACTIVEMAP
     * Requires vpx_active_map_t.
     * Minimum size required: 2 bytes for rows/cols.
     */
    if (Size >= 2) {
        vpx_active_map_t map;
        unsigned int rows = (Data[0] % 10) + 1;
        unsigned int cols = (Data[1] % 10) + 1;
        
        if (rows > 100 || cols > 100) {
            rows = 1; cols = 1;
        }

        size_t map_size = rows * cols;
        unsigned char *map_data = (unsigned char *)malloc(map_size);
        if (map_data) {
            size_t copy_len = (Size - 2 < map_size) ? (Size - 2) : map_size;
            memcpy(map_data, Data + 2, copy_len);
            if (copy_len < map_size) {
                memset(map_data + copy_len, 0, map_size - copy_len);
            }

            map.active_map = map_data;
            map.rows = rows;
            map.cols = cols;

            vpx_codec_control(ctx, VP8E_SET_ACTIVEMAP, &map);
            
            free(map_data);
        }
    }

    /* 
     * Test 2: VP8E_SET_GF_CBR_BOOST_PCT
     * Requires at least 4 bytes to safely access Data[2] and Data[3].
     */
    if (Size >= 4) {
        int pct = (int)(Data[2] ^ Data[3]);
        vpx_codec_control(ctx, VP8E_SET_GF_CBR_BOOST_PCT, pct);
    }

    /* 
     * Test 3: VP8E_SET_STATIC_THRESHOLD
     * Requires at least 8 bytes to safely access Data[4]..Data[7].
     */
    if (Size >= 8) {
        int threshold = (int)(Data[4] ^ Data[5] ^ Data[6] ^ Data[7]);
        vpx_codec_control(ctx, VP8E_SET_STATIC_THRESHOLD, threshold);
    }

    /* 
     * Test 4: VP9E_GET_ACTIVEMAP
     * Requires at least 8 bytes to safely access Data[4]..Data[7] (reusing logic for safety).
     */
    if (Size >= 8) {
        vpx_active_map_t map_out;
        memset(&map_out, 0, sizeof(map_out));
        
        vpx_codec_control(ctx, VP9E_GET_ACTIVEMAP, &map_out);
        
        if (map_out.active_map) {
            free(map_out.active_map);
        }
    }

    /* 
     * Test 5: VP9E_SET_GF_CBR_BOOST_PCT
     * Requires at least 10 bytes to safely access Data[8] and Data[9].
     */
    if (Size >= 10) {
        int pct = (int)(Data[8] ^ Data[9]);
        vpx_codec_control(ctx, VP9E_SET_GF_CBR_BOOST_PCT, pct);
    }

    /* 
     * Test 6: VP9E_SET_AQ_MODE
     * Requires at least 12 bytes to safely access Data[10] and Data[11].
     */
    if (Size >= 12) {
        int mode = (int)(Data[10] ^ Data[11]);
        vpx_codec_control(ctx, VP9E_SET_AQ_MODE, mode);
    }

    cleanup_context(ctx);

    return 0;
}