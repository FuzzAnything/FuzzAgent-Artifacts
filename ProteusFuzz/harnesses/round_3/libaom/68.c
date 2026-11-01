// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aom_integer.h>
#include <aom/aomcx.h>

/*
 * Fuzz driver for libaom encoder control functions.
 * Target functions:
 * - AOME_SET_SHARPNESS
 * - AV1E_SET_COEFF_COST_UPD_FREQ
 * - AV1E_SET_ENABLE_ADAPTIVE_SHARPNESS
 * - AV1E_SET_ENABLE_TPL_MODEL
 * - AV1E_SET_QM_MAX
 * - AOME_SET_ENABLEAUTOBWDREF
 */

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_68(const uint8_t *Data, size_t Size) {
    if (Size < 9) return 0;

    // Initialize codec context
    aom_codec_ctx_t ctx;
    
    // Get the AV1 encoder interface (aom_codec_av1_cx returns the interface)
    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    // Initialize encoder configuration
    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use fuzz data to set some basic config parameters
    uint16_t width = (Data[0] % 512) + 16;
    uint16_t height = (Data[1] % 512) + 16;
    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = (Data[2] % 1000) + 100;

    // Initialize the encoder
    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // aom_codec_control returns aom_codec_err_t.
    // The function signature is: aom_codec_err_t aom_codec_control(aom_codec_ctx_t *ctx, int ctrl, ...);

    // 1. AOME_SET_SHARPNESS
    int sharpness = (Data[3] % 11);
    aom_codec_control(&ctx, AOME_SET_SHARPNESS, &sharpness);

    // 2. AV1E_SET_COEFF_COST_UPD_FREQ
    int coeff_cost_upd_freq = (Data[4] % 256);
    aom_codec_control(&ctx, AV1E_SET_COEFF_COST_UPD_FREQ, &coeff_cost_upd_freq);

    // 3. AV1E_SET_ENABLE_ADAPTIVE_SHARPNESS
    int enable_adaptive_sharpness = (Data[5] % 2);
    aom_codec_control(&ctx, AV1E_SET_ENABLE_ADAPTIVE_SHARPNESS, &enable_adaptive_sharpness);

    // 4. AV1E_SET_ENABLE_TPL_MODEL
    int enable_tpl_model = (Data[6] % 2);
    aom_codec_control(&ctx, AV1E_SET_ENABLE_TPL_MODEL, &enable_tpl_model);

    // 5. AV1E_SET_QM_MAX
    int qm_max = (Data[7] % 256);
    aom_codec_control(&ctx, AV1E_SET_QM_MAX, &qm_max);

    // 6. AOME_SET_ENABLEAUTOBWDREF
    int enable_auto_bwd_ref = (Data[8] % 2);
    aom_codec_control(&ctx, AOME_SET_ENABLEAUTOBWDREF, &enable_auto_bwd_ref);

    // Optional: Try to encode a dummy frame to ensure state consistency
    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, width, height, 1);
    memset(img.planes[0], 0, width * height); 

    aom_enc_frame_flags_t flags = 0;
    aom_codec_cx_pkt_t pkt;
    
    // Corrected function call: aom_codec_encode takes 5 arguments
    if (aom_codec_encode(&ctx, &img, 0, 1, flags) == AOM_CODEC_OK) {
        while (aom_codec_get_cx_data(&ctx, &pkt) == AOM_CODEC_OK) {
            // In libaom, the packet data is owned by the codec context until the next call to get_cx_data or destroy.
            // There is no explicit free function for the packet structure itself in the public API.
            // We simply continue the loop. The data inside pkt.data (if it's a frame) is managed internally.
            // If pkt.data is a frame, we should not free it manually here.
            // The loop naturally exits when no more data is available.
        }
    }

    // Cleanup
    aom_img_free(&img);
    cleanup_encoder(&ctx);

    return 0;
}