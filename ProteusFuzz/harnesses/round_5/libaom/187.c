// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_codec.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_encoder.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>

/*
 * Helper function to initialize a decoder context.
 * Returns 1 on success, 0 on failure.
 */
static int init_decoder_context(aom_codec_ctx_t *ctx) {
    if (!ctx) return 0;

    aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) return 0;

    aom_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.allow_lowbitdepth = 1;
    // Removed allow_highbitdepth as it is not present in this version of the struct

    if (aom_codec_dec_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    return 1;
}

/*
 * Helper function to initialize an encoder context.
 * Returns 1 on success, 0 on failure.
 */
static int init_encoder_context(aom_codec_ctx_t *ctx) {
    if (!ctx) return 0;

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    
    cfg.g_w = 128;
    cfg.g_h = 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.g_error_resilient = 0;
    cfg.g_lag_in_frames = 0;
    cfg.g_threads = 1;
    cfg.g_pass = 0;
    cfg.g_usage = AOM_CQ;
    cfg.rc_min_quantizer = 0;
    cfg.rc_max_quantizer = 63;

    if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    return 1;
}

int LLVMFuzzerTestOneInput_187(const uint8_t *Data, size_t Size) {
    if (Size < 5) return 0;

    // Fuzzing Decoder Controls
    aom_codec_ctx_t dec_ctx;
    memset(&dec_ctx, 0, sizeof(dec_ctx));
    
    if (init_decoder_context(&dec_ctx)) {
        // 1. aom_codec_control_typechecked_AV1D_SET_OUTPUT_ALL_LAYERS
        int val = (Data[0] % 2);
        aom_codec_control(&dec_ctx, AV1D_SET_OUTPUT_ALL_LAYERS, val);

        // 3. aom_codec_control_typechecked_AV1D_SET_SKIP_FILM_GRAIN
        int skip_grain = (Data[1] % 2);
        aom_codec_control(&dec_ctx, AV1D_SET_SKIP_FILM_GRAIN, skip_grain);

        // 4. aom_codec_control_typechecked_AV1D_GET_DISPLAY_SIZE
        aom_image_t display_size;
        memset(&display_size, 0, sizeof(display_size));
        aom_codec_control(&dec_ctx, AV1D_GET_DISPLAY_SIZE, &display_size);

        // 6. aom_codec_control_typechecked_AV1D_SET_OPERATING_POINT
        int op = Data[2] % 16;
        aom_codec_control(&dec_ctx, AV1D_SET_OPERATING_POINT, op);

        // Test Encoder control on Decoder (robustness check)
        int layer_id = Data[3] % 8;
        aom_codec_control(&dec_ctx, AOME_SET_SPATIAL_LAYER_ID, layer_id);
        
        int drop_mode = Data[4] % 4;
        aom_codec_control(&dec_ctx, AV1E_SET_SVC_FRAME_DROP_MODE, drop_mode);

        aom_codec_destroy(&dec_ctx);
    }

    // Fuzzing Encoder Controls
    aom_codec_ctx_t enc_ctx;
    memset(&enc_ctx, 0, sizeof(enc_ctx));

    if (init_encoder_context(&enc_ctx)) {
        // 2. aom_codec_control_typechecked_AOME_SET_SPATIAL_LAYER_ID
        int layer_id = Data[3] % 8;
        aom_codec_control(&enc_ctx, AOME_SET_SPATIAL_LAYER_ID, layer_id);

        // 5. aom_codec_control_typechecked_AV1E_SET_SVC_FRAME_DROP_MODE
        int drop_mode = Data[4] % 4;
        aom_codec_control(&enc_ctx, AV1E_SET_SVC_FRAME_DROP_MODE, drop_mode);

        // Test Decoder controls on Encoder (robustness check)
        int val = (Data[0] % 2);
        aom_codec_control(&enc_ctx, AV1D_SET_OUTPUT_ALL_LAYERS, val);
        
        int skip_grain = (Data[1] % 2);
        aom_codec_control(&enc_ctx, AV1D_SET_SKIP_FILM_GRAIN, skip_grain);
        
        aom_image_t display_size;
        memset(&display_size, 0, sizeof(display_size));
        aom_codec_control(&enc_ctx, AV1D_GET_DISPLAY_SIZE, &display_size);
        
        int op = Data[2] % 16;
        aom_codec_control(&enc_ctx, AV1D_SET_OPERATING_POINT, op);

        aom_codec_destroy(&enc_ctx);
    }

    return 0;
}