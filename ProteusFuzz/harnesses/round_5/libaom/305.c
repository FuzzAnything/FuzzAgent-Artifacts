// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
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
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom.h"
#include "aom_codec.h"
#include "aom_encoder.h"
#include "aom_image.h"
#include "aomcx.h"
#include "aomdx.h"
#include "aom_external_partition.h"
#include "aom_decoder.h"
#include "aom_frame_buffer.h"
#include "aom_integer.h"

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static int init_encoder_context(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return -1;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }

    // Fuzz configuration parameters based on input data if size permits
    if (size > 0) cfg.g_w = (data[0] % 256) + 16;
    if (size > 1) cfg.g_h = (data[1] % 256) + 16;
    if (size > 2) cfg.g_timebase.num = (data[2] % 100) + 1;
    if (size > 3) cfg.g_timebase.den = (data[3] % 100) + 1;
    if (size > 4) cfg.g_threads = (data[4] % 8) + 1;
    if (size > 5) cfg.g_lag_in_frames = (data[5] % 20) + 1;
    if (size > 6) cfg.rc_target_bitrate = (data[6] % 1000) + 100;

    if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }

    return 0;
}

int LLVMFuzzerTestOneInput_305(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    if (init_encoder_context(&ctx, Data, Size) != 0) {
        return 0;
    }

    // Access config via the union member 'enc' cast to the correct type
    const aom_codec_enc_cfg_t *enc_cfg = (const aom_codec_enc_cfg_t *)ctx.config.enc;
    if (!enc_cfg) {
        cleanup_codec(&ctx);
        return 0;
    }

    // Prepare a dummy frame for encoding to ensure context is active
    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, enc_cfg->g_w, enc_cfg->g_h, 1);
    
    // Fill image with fuzz data if available
    if (Size > 0) {
        // img.planes[i] is uint8_t*, stride is in img.stride[i]
        size_t img_size = img.stride[0] * img.h;
        size_t copy_size = (Size < img_size) ? Size : img_size;
        memcpy(img.planes[0], Data, copy_size);
        
        if (img.planes[1]) {
            size_t uv_size = img.stride[1] * (img.h / 2);
            size_t uv_copy = (Size - copy_size < uv_size) ? (Size - copy_size) : uv_size;
            if (Size > copy_size) {
                memcpy(img.planes[1], Data + copy_size, uv_copy);
            }
        }
        if (img.planes[2]) {
            size_t uv2_size = img.stride[2] * (img.h / 2);
            // Re-calculate uv_copy if planes[1] was not present or logic changed
            size_t uv_copy_val = 0;
            if (img.planes[1]) {
                size_t uv_size = img.stride[1] * (img.h / 2);
                uv_copy_val = (Size - copy_size < uv_size) ? (Size - copy_size) : uv_size;
            }
            
            size_t uv2_copy = (Size - copy_size - uv_copy_val < uv2_size) ? (Size - copy_size - uv_copy_val) : uv2_size;
            if (Size > copy_size + uv_copy_val) {
                memcpy(img.planes[2], Data + copy_size + uv_copy_val, uv2_copy);
            }
        }
    }

    // Fixed: aom_codec_encode takes 5 arguments
    aom_codec_encode(&ctx, &img, 0, 1, 0);

    // Invoke target API functions with fuzzed integers derived from input
    int val1 = (Size > 0) ? Data[0] : 0;
    int val2 = (Size > 1) ? Data[1] : 0;
    int val4 = (Size > 3) ? Data[3] : 0;
    int val6 = (Size > 5) ? Data[5] : 0;

    // 1. AV1E_SET_INTRA_DCT_ONLY
    aom_codec_control(&ctx, AV1E_SET_INTRA_DCT_ONLY, val1);

    // 2. AV1E_SET_ENABLE_TX_SIZE_SEARCH
    aom_codec_control(&ctx, AV1E_SET_ENABLE_TX_SIZE_SEARCH, val2);

    // 3. AV1E_GET_NUM_OPERATING_POINTS
    int num_ops = 0;
    aom_codec_control(&ctx, AV1E_GET_NUM_OPERATING_POINTS, &num_ops);

    // 4. AV1E_SET_ENABLE_INTERINTRA_COMP
    aom_codec_control(&ctx, AV1E_SET_ENABLE_INTERINTRA_COMP, val4);

    // 5. AV1E_GET_BASELINE_GF_INTERVAL
    int gf_interval = 0;
    aom_codec_control(&ctx, AV1E_GET_BASELINE_GF_INTERVAL, &gf_interval);

    // 6. AV1E_SET_ENABLE_RECT_TX
    aom_codec_control(&ctx, AV1E_SET_ENABLE_RECT_TX, val6);

    // Cleanup
    aom_img_free(&img);
    cleanup_codec(&ctx);

    return 0;
}