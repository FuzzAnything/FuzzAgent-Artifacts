// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_decoder.h>
#include <vpx/vpx_encoder.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

static int initialize_encoder_config(vpx_codec_enc_cfg_t *cfg, const uint8_t *data, size_t size) {
    if (!cfg) return -1;
    memset(cfg, 0, sizeof(vpx_codec_enc_cfg_t));
    
    if (vpx_codec_enc_config_default(cfg, &vpx_codec_vp9_cx, 0) != 0) return -1;

    if (size >= 1) cfg->g_w = data[0] + 16;
    if (size >= 2) cfg->g_h = data[1] + 16;
    if (size >= 3) cfg->g_timebase.num = data[2] + 1;
    if (size >= 4) cfg->g_timebase.den = data[3] + 1;
    if (size >= 5) cfg->g_error_resilient = data[4] % 2;
    if (size >= 6) cfg->rc_target_bitrate = (data[6] * 256 + (size > 7 ? data[7] : 0)) * 1000;
    if (size >= 8) cfg->g_lag_in_frames = data[8] % 32;
    if (size >= 9) cfg->g_threads = (data[9] % 8) + 1;
    
    // Attempt to use the provided macros if they are defined
    #ifdef VP9E_SET_MAX_GF_INTERVAL
    if (size >= 10) cfg->kf_max_dist = data[10];
    #endif

    #ifdef VP9E_ENABLE_MOTION_VECTOR_UNIT_TEST
    // This is typically a debug flag
    #endif

    #ifdef VP9E_SET_SVC_GF_TEMPORAL_REF
    cfg->svc_mode = 1;
    if (size >= 11) cfg->svc_tlayer = data[11] % 4;
    #endif

    #ifdef VP9E_SET_TARGET_LEVEL
    if (size >= 12) cfg->g_profile = data[12] % 4;
    #endif

    #ifdef VP9E_SET_SVC_INTER_LAYER_PRED
    if (size >= 13) cfg->svc_inter_layer_pred = data[13] % 2;
    #endif

    return 0;
}

static int initialize_decoder_context(vpx_codec_ctx_t *ctx, vpx_codec_dec_cfg_t *cfg, const uint8_t *data, size_t size) {
    if (!ctx || !cfg) return -1;
    memset(cfg, 0, sizeof(vpx_codec_dec_cfg_t));
    
    if (size >= 1) cfg->threads = (data[0] % 8) + 1;
    // Removed 'error_resilient' as it's not a member of vpx_codec_dec_cfg in recent versions
    // Using 'deinterlace' if available, otherwise just skip
    #ifdef VpxCodecDecCfgHasDeinterlace
    if (size >= 2) cfg->deinterlace = data[2] % 2;
    #endif
    
    return 0;
}

int LLVMFuzzerTestOneInput_117(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Encoder Fuzzing
    vpx_codec_enc_cfg_t cfg;
    if (initialize_encoder_config(&cfg, Data, Size) == 0) {
        vpx_codec_ctx_t encoder;
        if (vpx_codec_enc_config_default(&cfg, &vpx_codec_vp9_cx, 0) == 0) {
            if (vpx_codec_enc_init(&encoder, &vpx_codec_vp9_cx, &cfg, 0) == 0) {
                vpx_codec_destroy(&encoder);
            }
        }
    }

    // Decoder Fuzzing
    vpx_codec_dec_cfg_t dec_cfg;
    vpx_codec_ctx_t decoder;
    if (initialize_decoder_context(&decoder, &dec_cfg, Data, Size) == 0) {
        if (vpx_codec_dec_init(&decoder, &vpx_codec_vp9_dx, &dec_cfg, 0) == 0) {
            vpx_codec_iter_t iter = NULL;
            const vpx_image_t *frame;
            
            // Attempt to decode dummy data
            vpx_codec_decode(&decoder, Data, Size, NULL, 0);
            while ((frame = vpx_codec_get_frame(&decoder, &iter))) {
                // VP9D_GET_BIT_DEPTH might be accessible here
                #ifdef VP9D_GET_BIT_DEPTH
                int bit_depth = VP9D_GET_BIT_DEPTH(frame);
                (void)bit_depth; // Suppress unused warning
                #endif
            }
            vpx_codec_destroy(&decoder);
        }
    }

    return 0;
}