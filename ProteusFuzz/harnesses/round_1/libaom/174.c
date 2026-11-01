// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aomdx.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static void cleanup_decoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_174(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Use data to determine behavior or seed values
    int seed = 0;
    if (Size > 0) {
        seed = Data[0];
    }

    // Fuzz Encoder Controls
    {
        aom_codec_ctx_t encoder_ctx;
        aom_codec_iface_t *encoder_iface = aom_codec_av1_cx();
        aom_codec_enc_cfg_t cfg;

        if (encoder_iface == NULL) {
            return 0;
        }

        if (aom_codec_enc_config_default(encoder_iface, &cfg, 0) != AOM_CODEC_OK) {
            return 0;
        }

        // Set some basic config from fuzz data
        cfg.g_w = 16 + (seed % 100);
        cfg.g_h = 16 + (seed % 100);
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 30;

        if (aom_codec_enc_init(&encoder_ctx, encoder_iface, &cfg, 0) != AOM_CODEC_OK) {
            return 0;
        }

        // AV1E_SET_ENABLE_INTERINTRA_WEDGE
        int enable_interintra_wedge = (seed % 2);
        aom_codec_control(&encoder_ctx, AV1E_SET_ENABLE_INTERINTRA_WEDGE, enable_interintra_wedge);

        // AV1E_SET_MAX_REFERENCE_FRAMES
        int max_ref_frames = 1 + (seed % 8);
        aom_codec_control(&encoder_ctx, AV1E_SET_MAX_REFERENCE_FRAMES, max_ref_frames);

        // AV1E_SET_TARGET_SEQ_LEVEL_IDX
        int target_seq_level_idx = seed % 7; // Valid levels 0-6 typically
        aom_codec_control(&encoder_ctx, AV1E_SET_TARGET_SEQ_LEVEL_IDX, target_seq_level_idx);

        cleanup_encoder(&encoder_ctx);
    }

    // Fuzz Decoder Controls
    {
        aom_codec_ctx_t decoder_ctx;
        aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
        aom_codec_dec_cfg_t dec_cfg;

        if (decoder_iface == NULL) {
            return 0;
        }

        dec_cfg.threads = 0;
        dec_cfg.allow_lowbitdepth = 1;
        // Note: allow_intrabc and allow_screen_content_tools are not standard members of aom_codec_dec_cfg
        // in all versions, so we omit them to ensure compilation.

        if (aom_codec_dec_init(&decoder_ctx, decoder_iface, &dec_cfg, 0) != AOM_CODEC_OK) {
            return 0;
        }

        // AOMD_GET_STILL_PICTURE
        aom_still_picture_info still_info;
        aom_codec_control(&decoder_ctx, AOMD_GET_STILL_PICTURE, &still_info);

        // AOMD_GET_ORDER_HINT
        int order_hint = 0;
        aom_codec_control(&decoder_ctx, AOMD_GET_ORDER_HINT, &order_hint);

        // AOMD_GET_FWD_KF_PRESENT
        int fwd_kf_present = 0;
        aom_codec_control(&decoder_ctx, AOMD_GET_FWD_KF_PRESENT, &fwd_kf_present);

        cleanup_decoder(&decoder_ctx);
    }

    return 0;
}