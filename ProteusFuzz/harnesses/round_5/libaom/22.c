// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aom_codec.h"
#include "aom/aomcx.h"
#include "aom/aomdx.h"
#include "aom/aom.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_image.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_integer.h"

static int fuzz_decoder_control(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    aom_codec_dec_cfg_t cfg;
    aom_codec_err_t ret;
    aom_codec_iface_t *iface;
    aom_image_t *img;
    
    // Initialize config with valid fields for the current libaom version
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    
    // Get AV1 decoder interface
    iface = aom_codec_av1_dx();
    if (!iface) return 0;

    // Initialize decoder context
    ret = aom_codec_dec_init(&ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) return 0;

    // Prepare dummy data to decode
    if (Size > 0) {
        aom_codec_iter_t iter = NULL;
        
        // Attempt to decode the fuzzed input
        ret = aom_codec_decode(&ctx, Data, Size, NULL);
        if (ret == AOM_CODEC_OK) {
            // Iterate over decoded frames
            while ((img = aom_codec_get_frame(&ctx, &iter)) != NULL) {
                // Try various control IDs on the decoded frame context
                
                // 1. AOMD_GET_ALTREF_PRESENT
                int altref_present = 0;
                aom_codec_control(&ctx, AOMD_GET_ALTREF_PRESENT, &altref_present);

                // 2. AOMD_GET_ORDER_HINT
                int order_hint = 0;
                aom_codec_control(&ctx, AOMD_GET_ORDER_HINT, &order_hint);

                // 5. AOMD_GET_LAST_REF_USED
                int last_ref_used[8]; 
                memset(last_ref_used, 0, sizeof(last_ref_used));
                aom_codec_control(&ctx, AOMD_GET_LAST_REF_USED, last_ref_used);

                // 6. AOMD_GET_S_FRAME_INFO
                aom_s_frame_info s_frame_info = {0};
                aom_codec_control(&ctx, AOMD_GET_S_FRAME_INFO, &s_frame_info);
            }
        }
    }

    // Cleanup
    aom_codec_destroy(&ctx);
    return 1;
}

static int fuzz_encoder_control(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t ret;
    aom_codec_iface_t *iface;
    aom_image_t img;
    
    // Initialize config
    memset(&cfg, 0, sizeof(cfg));
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.g_threads = 1;

    // Get AV1 encoder interface
    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    // Initialize encoder context
    ret = aom_codec_enc_config_default(&ctx, iface, 0);
    if (ret != AOM_CODEC_OK) return 0;
    
    // Apply custom config
    ret = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) return 0;

    // Prepare a dummy image using aom_img_alloc
    aom_img_alloc(&img, AOM_IMG_FMT_I420, 64, 64, 1);
    
    if (img.planes[0] && img.planes[1] && img.planes[2]) {
        // Fill with fuzz data if available
        if (Size > 0) {
            size_t fill_size = 64 * 64;
            if (Size < fill_size) fill_size = Size;
            memcpy(img.planes[0], Data, fill_size);
            // Fill chroma with zeros or pattern
            memset(img.planes[1], 0, 64 * 64 / 4);
            memset(img.planes[2], 0, 64 * 64 / 4);
        }

        // Encode one frame (Fixed: 5 arguments)
        ret = aom_codec_encode(&ctx, &img, 0, 1, 0);
        
        // Try encoder specific controls
        // AV1E_GET_LUMA_CDEF_STRENGTH (Fixed: Correct ID name)
        // Buffer size: 16 integers as per prompt warning
        int cdef_strengths[16]; 
        memset(cdef_strengths, 0, sizeof(cdef_strengths));
        
        // Attempt to get CDEF strengths
        aom_codec_control(&ctx, AV1E_GET_LUMA_CDEF_STRENGTH, cdef_strengths);

        // Cleanup image
        aom_img_free(&img);
    }

    // Cleanup
    aom_codec_destroy(&ctx);
    return 1;
}

int LLVMFuzzerTestOneInput_22(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Run decoder fuzzing
    fuzz_decoder_control(Data, Size);

    // Run encoder fuzzing
    fuzz_encoder_control(Data, Size);

    return 0;
}