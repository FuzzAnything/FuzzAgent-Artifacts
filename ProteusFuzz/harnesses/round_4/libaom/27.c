// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
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
#include <limits.h>

#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aom_integer.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aom_external_partition.h>

// Helper to safely initialize a codec context
static int init_encoder_context(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg) {
    if (!ctx || !cfg) return 0;

    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Set some basic config to ensure valid state
    cfg->g_w = 64;
    cfg->g_h = 64;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_bit_depth = 8;
    cfg->g_input_bit_depth = 8;
    cfg->g_profile = 0;
    cfg->rc_target_bitrate = 500;
    
    // Fixed: Use correct member names from aom_codec_enc_cfg
    cfg->g_lag_in_frames = 0;
    cfg->rc_undershoot_pct = 0;
    cfg->rc_overshoot_pct = 0;

    if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    return 1;
}

// Helper to create a dummy image
static aom_image_t* create_dummy_image(unsigned int w, unsigned int h, unsigned int bit_depth) {
    aom_image_t *img = aom_img_alloc(NULL, AOM_IMG_FMT_I420, w, h, 1);
    if (!img) return NULL;

    img->bit_depth = bit_depth;
    img->d_w = w;
    img->d_h = h;
    img->r_w = w;
    img->r_h = h;

    // Fill planes with deterministic data based on size to avoid UB
    // Corrected: Calculate plane size using the actual allocated dimensions for each plane
    // The height for chroma planes (U, V) is reduced by y_chroma_shift (usually 1 for I420)
    for (int i = 0; i < 3; i++) {
        if (img->planes[i]) {
            // Calculate actual height for this plane considering subsampling
            unsigned int plane_h = h >> img->y_chroma_shift;
            // Calculate the actual size for this plane: stride * plane_height
            // This matches the allocation logic in aom_img_alloc
            size_t plane_size = (size_t)img->stride[i] * plane_h;
            memset(img->planes[i], 0x80, plane_size); // Mid-gray
        }
    }

    return img;
}

int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    // Extract parameters from fuzz data
    uint8_t mode = Data[0] % 5; // 0-4 for different test scenarios
    uint8_t width = (Data[1] % 32) + 16; // 16-47
    uint8_t height = (Data[2 % Size] % 32) + 16; // 16-47
    uint8_t bit_depth = (Data[3 % Size] % 5) + 8; // 8, 10, 12
    int level_idx = (int)(Data[4 % Size]);
    int enable_intrabc = (Data[5 % Size] % 2);
    
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    if (!init_encoder_context(&ctx, &cfg)) {
        return 0;
    }

    aom_image_t *img = create_dummy_image(width, height, bit_depth);
    if (!img) {
        aom_codec_destroy(&ctx);
        return 0;
    }

    aom_codec_err_t err;

    switch (mode) {
        case 0:
            // Test AV1E_SET_TARGET_SEQ_LEVEL_IDX
            err = aom_codec_control(&ctx, AV1E_SET_TARGET_SEQ_LEVEL_IDX, level_idx);
            if (err != AOM_CODEC_OK) {
                // Expected for invalid levels, but we continue
            }
            break;
        case 1:
            // Test AV1E_GET_TARGET_SEQ_LEVEL_IDX
            int get_level;
            err = aom_codec_control(&ctx, AV1E_GET_TARGET_SEQ_LEVEL_IDX, &get_level);
            if (err != AOM_CODEC_OK) {
                // Expected for invalid state
            }
            break;
        case 2:
            // Test AV1E_SET_ENABLE_INTRABC
            err = aom_codec_control(&ctx, AV1E_SET_ENABLE_INTRABC, enable_intrabc);
            if (err != AOM_CODEC_OK) {
                // Expected for invalid state
            }
            break;
        case 3:
            // Test AV1E_SET_SVC_PARAMS
            aom_svc_params_t svc_params;
            memset(&svc_params, 0, sizeof(svc_params));
            svc_params.number_spatial_layers = 1;
            svc_params.number_temporal_layers = 1;
            svc_params.max_quantizers[0] = 63;
            svc_params.min_quantizers[0] = 0;
            svc_params.scaling_factor_num[0] = 1;
            svc_params.scaling_factor_den[0] = 1;
            svc_params.layer_target_bitrate[0] = 500;
            svc_params.framerate_factor[0] = 1;

            err = aom_codec_control(&ctx, AV1E_SET_SVC_PARAMS, &svc_params);
            if (err != AOM_CODEC_OK) {
                // Expected for invalid state
            }
            break;
        case 4:
            // Test aom_codec_encode
            aom_codec_pts_t pts = (aom_codec_pts_t)(Data[6 % Size]);
            unsigned long duration = (unsigned long)(Data[7 % Size] + 1);
            aom_enc_frame_flags_t flags = (aom_enc_frame_flags_t)(Data[8 % Size]);

            err = aom_codec_encode(&ctx, img, pts, duration, flags);
            if (err != AOM_CODEC_OK) {
                // Expected for invalid state
            }
            
            // Flush the encoder
            while (aom_codec_encode(&ctx, NULL, 0, 0, 0) == AOM_CODEC_OK) {
                aom_codec_cx_pkt_t *pkt;
                while ((pkt = aom_codec_get_cx_data(&ctx, NULL)) != NULL) {
                    // No specific destroy_pkt function in standard API, 
                    // just let the codec manage memory or ignore for fuzzing
                    // aom_codec_destroy_pkt is not a standard public API function
                    // We just consume the packet
                }
                // Check if there is more data
                if (aom_codec_get_cx_data(&ctx, NULL) == NULL) break;
            }
            break;
    }

    // Cleanup
    aom_img_free(img);
    aom_codec_destroy(&ctx);

    return 0;
}