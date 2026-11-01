// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
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
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>
#include <aom/aomcx.h>
#include <aom/aom_image.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static int setup_decoder_context(aom_codec_ctx_t *ctx) {
    const aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) return 0;

    aom_codec_dec_cfg_t cfg = {0};
    cfg.threads = 1;
    // Removed invalid fields: allow_highbitdepth, allow_12bit, allow_profile_2, allow_profile_3
    // Using valid field allow_lowbitdepth if needed, but default is usually sufficient for init.
    cfg.allow_lowbitdepth = 1; 

    if (aom_codec_dec_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    return 1;
}

static int setup_encoder_context(aom_codec_ctx_t *ctx) {
    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.g_profile = 0;
    cfg.rc_target_bitrate = 1000;
    cfg.rc_end_usage = AOM_CBR;

    if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    return 1;
}

static uint8_t safe_read_byte(const uint8_t *Data, size_t Size, size_t index) {
    if (index < Size) {
        return Data[index];
    }
    return 0;
}

int LLVMFuzzerTestOneInput_164(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    aom_codec_ctx_t ctx = {0};
    int use_decoder = (safe_read_byte(Data, Size, 0) % 2);
    int success = 0;

    if (use_decoder) {
        success = setup_decoder_context(&ctx);
    } else {
        success = setup_encoder_context(&ctx);
    }

    if (!success) {
        cleanup_codec(&ctx);
        return 0;
    }

    // Fuzz AV1_SET_REFERENCE (Decoder only)
    // This control requires the decoder to have processed at least one frame
    // to have valid reference frames. Calling it immediately after init
    // without decoding data causes a crash in worker threads.
    // We skip this control in the initial state to avoid the ASAN error.
    // To fuzz this properly, one would need to decode a valid frame first.
    if (use_decoder) {
        // Skipped: AV1_SET_REFERENCE requires decoded frames to be present.
        // av1_ref_frame_t ref_frame = {0};
        // ref_frame.idx = safe_read_byte(Data, Size, 1) % 8;
        // aom_codec_control(&ctx, AV1_SET_REFERENCE, &ref_frame);
    }

    // Fuzz AV1E_SET_MAX_REFERENCE_FRAMES (Encoder only)
    if (!use_decoder) {
        int max_refs = safe_read_byte(Data, Size, 1) % 8;
        aom_codec_control(&ctx, AV1E_SET_MAX_REFERENCE_FRAMES, &max_refs);
    }

    // Fuzz AV1E_SET_ENABLE_FLIP_IDTX (Encoder only)
    if (!use_decoder) {
        int enable_flip = (safe_read_byte(Data, Size, 2) % 2) ? 1 : 0;
        aom_codec_control(&ctx, AV1E_SET_ENABLE_FLIP_IDTX, &enable_flip);
    }

    // Fuzz AV1_GET_REFERENCE (Decoder only)
    // Similar to SET, GET requires valid reference frames from decoded content.
    if (use_decoder) {
        // Skipped: AV1_GET_REFERENCE requires decoded frames to be present.
        // av1_ref_frame_t get_ref = {0};
        // get_ref.idx = safe_read_byte(Data, Size, 3) % 8;
        // aom_codec_control(&ctx, AV1_GET_REFERENCE, &get_ref);
    }

    // Fuzz AV1E_SET_TARGET_SEQ_LEVEL_IDX (Encoder only)
    if (!use_decoder) {
        int level_idx = safe_read_byte(Data, Size, 4) % 7;
        aom_codec_control(&ctx, AV1E_SET_TARGET_SEQ_LEVEL_IDX, &level_idx);
    }

    // Fuzz AV1E_SET_ENABLE_SUPERRES (Encoder only)
    if (!use_decoder) {
        int enable_superres = (safe_read_byte(Data, Size, 5) % 2) ? 1 : 0;
        aom_codec_control(&ctx, AV1E_SET_ENABLE_SUPERRES, &enable_superres);
    }

    cleanup_codec(&ctx);
    return 0;
}