// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aomcx.h>

/* 
 * Fuzz driver for libaom AV1 encoder control functions.
 * Target functions:
 * - AV1E_SET_ENABLE_QM
 * - AV1E_SET_INTRA_DCT_ONLY
 * - AV1E_SET_DENOISE_BLOCK_SIZE
 * - AV1E_SET_FORCE_VIDEO_MODE
 * - AV1E_SET_ENABLE_CDEF
 * - AV1E_SET_ENABLE_KEYFRAME_FILTERING
 */

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_139(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    aom_codec_ctx_t ctx = {0};
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use input data to configure encoder settings
    cfg.g_w = (Data[0] % 1024) + 16; // Width between 16 and 1039
    cfg.g_h = (Data[1] % 1024) + 16; // Height between 16 and 1039
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = (Data[2] % 100) + 1;
    cfg.rc_target_bitrate = (Data[3] % 1000) + 100;
    cfg.g_profile = Data[4] % 4;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Prepare dummy image for encoding to ensure codec is in a valid state
    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
    memset(img.planes[0], 128, img.stride[0] * img.d_h);
    memset(img.planes[1], 128, img.stride[1] * img.d_h / 2);
    memset(img.planes[2], 128, img.stride[2] * img.d_h / 2);

    // Encode one frame to initialize internal state
    aom_codec_encode(&ctx, &img, 0, 1, 0);
    
    aom_codec_iter_t iter = NULL;
    aom_codec_cx_pkt_t *pkt;
    while ((pkt = aom_codec_get_cx_data(&ctx, &iter)) != NULL) {
        // Consume output packets
    }

    // Now fuzz the control functions using input data
    int val;
    int ctrl_val;

    // Helper to extract int from data safely
    #define GET_INT(idx) ((idx < Size) ? Data[idx] : 0)

    // 1. AV1E_SET_ENABLE_QM (expects int)
    val = GET_INT(5);
    aom_codec_control(&ctx, AV1E_SET_ENABLE_QM, val);

    // 2. AV1E_SET_INTRA_DCT_ONLY (expects int)
    val = GET_INT(6);
    aom_codec_control(&ctx, AV1E_SET_INTRA_DCT_ONLY, val);

    // 3. AV1E_SET_DENOISE_BLOCK_SIZE (expects int)
    val = GET_INT(7);
    aom_codec_control(&ctx, AV1E_SET_DENOISE_BLOCK_SIZE, val);

    // 4. AV1E_SET_FORCE_VIDEO_MODE (expects int)
    val = GET_INT(8);
    aom_codec_control(&ctx, AV1E_SET_FORCE_VIDEO_MODE, val);

    // 5. AV1E_SET_ENABLE_CDEF (expects int)
    val = GET_INT(9);
    aom_codec_control(&ctx, AV1E_SET_ENABLE_CDEF, val);

    // 6. AV1E_SET_ENABLE_KEYFRAME_FILTERING (expects int)
    val = GET_INT(10);
    aom_codec_control(&ctx, AV1E_SET_ENABLE_KEYFRAME_FILTERING, val);

    // Additional fuzzing: try multiple calls with different values
    for (size_t i = 11; i < Size; i += 1) {
        ctrl_val = Data[i];
        aom_codec_control(&ctx, AV1E_SET_ENABLE_QM, ctrl_val);
        aom_codec_control(&ctx, AV1E_SET_INTRA_DCT_ONLY, ctrl_val);
        aom_codec_control(&ctx, AV1E_SET_DENOISE_BLOCK_SIZE, ctrl_val);
        aom_codec_control(&ctx, AV1E_SET_FORCE_VIDEO_MODE, ctrl_val);
        aom_codec_control(&ctx, AV1E_SET_ENABLE_CDEF, ctrl_val);
        aom_codec_control(&ctx, AV1E_SET_ENABLE_KEYFRAME_FILTERING, ctrl_val);
    }

    // Cleanup
    aom_img_free(&img);
    cleanup_codec(&ctx);

    return 0;
}