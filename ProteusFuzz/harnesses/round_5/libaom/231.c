// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aomdx.h>
#include <aom/aom_codec.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_encoder.h>
#include <aom/aom_integer.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static int init_encoder_context(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg) {
    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return -1;

    if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }
    
    // Set minimal config to allow initialization
    cfg->g_w = 64;
    cfg->g_h = 64;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->rc_end_usage = AOM_CBR;
    cfg->rc_target_bitrate = 1000;
    cfg->rc_undershoot_pct = 0;
    cfg->rc_overshoot_pct = 0;
    // Fixed field names based on compiler error
    cfg->rc_min_quantizer = 0;
    cfg->rc_max_quantizer = 63;

    if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }
    return 0;
}

static int init_decoder_context(aom_codec_ctx_t *ctx, aom_codec_dec_cfg_t *cfg) {
    const aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) return -1;

    // Initialize config manually as aom_codec_dec_config_default may not be available in all versions
    memset(cfg, 0, sizeof(aom_codec_dec_cfg_t));
    cfg->threads = 1;
    // Removed invalid fields: deinterlace, low_delay, error_resilient
    // These fields do not exist in the current aom_codec_dec_cfg_t struct

    if (aom_codec_dec_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }
    return 0;
}

int LLVMFuzzerTestOneInput_231(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    aom_codec_ctx_t ctx = {0};
    int ret;

    // Use first byte to decide encoder vs decoder
    int use_encoder = Data[0] & 1;

    if (use_encoder) {
        aom_codec_enc_cfg_t cfg;
        if (init_encoder_context(&ctx, &cfg) != 0) return 0;
    } else {
        aom_codec_dec_cfg_t cfg;
        if (init_decoder_context(&ctx, &cfg) != 0) return 0;
    }

    // Fuzz AOME_SET_ENABLEAUTOALTREF (Encoder only)
    if (use_encoder) {
        int val = (Data[1] & 0xFF) ? 1 : 0;
        aom_codec_control(&ctx, AOME_SET_ENABLEAUTOALTREF, val);
    }

    // Fuzz AV1E_SET_ENABLE_PAETH_INTRA (Encoder only)
    if (use_encoder) {
        int val = (Data[1] & 0xFF) ? 1 : 0;
        aom_codec_control(&ctx, AV1E_SET_ENABLE_PAETH_INTRA, val);
    }

    // Fuzz AV1E_SET_SVC_PARAMS
    if (use_encoder && Size > 10) {
        aom_svc_params_t svc_params = {0};
        // Extract some fuzz data for SVC params
        svc_params.number_spatial_layers = (Data[2] % 3) + 1;
        svc_params.number_temporal_layers = (Data[3] % 3) + 1;
        for (int i = 0; i < AOM_MAX_LAYERS && i < 10; i++) {
            svc_params.max_quantizers[i] = Data[4 + i];
            svc_params.min_quantizers[i] = Data[5 + i];
        }
        for (int i = 0; i < AOM_MAX_SS_LAYERS && i < 3; i++) {
            svc_params.scaling_factor_num[i] = Data[6 + i];
            svc_params.scaling_factor_den[i] = Data[7 + i];
        }
        for (int i = 0; i < AOM_MAX_LAYERS && i < 10; i++) {
            svc_params.layer_target_bitrate[i] = (Data[8 + i] * 100);
        }
        for (int i = 0; i < AOM_MAX_TS_LAYERS && i < 3; i++) {
            svc_params.framerate_factor[i] = Data[9 + i];
        }
        aom_codec_control(&ctx, AV1E_SET_SVC_PARAMS, &svc_params);
    }

    // Fuzz AV1 reference frame operations (Decoder only)
    if (!use_encoder) {
        av1_ref_frame_t ref_frame = {0};
        if (Size > 4) {
            ref_frame.idx = Data[2] & 0x7F;
            ref_frame.use_external_ref = (Data[3] & 0x01) ? 1 : 0;
            
            // Try to get reference
            aom_codec_control(&ctx, AV1_GET_REFERENCE, &ref_frame);
            
            // Try to set reference
            aom_codec_control(&ctx, AV1_SET_REFERENCE, &ref_frame);
            
            // Try to copy reference
            av1_ref_frame_t copy_ref = {0};
            copy_ref.idx = Data[4] & 0x7F;
            aom_codec_control(&ctx, AV1_COPY_REFERENCE, &copy_ref);
        }
    }

    cleanup_codec(&ctx);
    return 0;
}