// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
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
#include <stdio.h>
#include <aom/aomdx.h>
#include <aom/aom_encoder.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aom_integer.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aom_external_partition.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_183(const uint8_t *Data, size_t Size) {
    if (Size < 9) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface = aom_codec_av1_cx();

    if (!iface) {
        return 0;
    }

    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use fuzzing data to set some configuration parameters safely
    cfg.g_w = (Data[0] % 1024) + 16;
    cfg.g_h = (Data[1] % 1024) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.g_error_resilient = 0;
    cfg.rc_target_bitrate = (Data[2] * 256 + Data[3]) + 1;
    cfg.rc_end_usage = AOM_CBR;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz AV1E_SET_ENABLE_DIRECTIONAL_INTRA
    int enable_dir_intra = Data[4] % 2;
    aom_codec_control(&ctx, AV1E_SET_ENABLE_DIRECTIONAL_INTRA, enable_dir_intra);

    // Fuzz AV1E_SET_NOISE_SENSITIVITY
    int noise_sensitivity = Data[5] % 10;
    aom_codec_control(&ctx, AV1E_SET_NOISE_SENSITIVITY, noise_sensitivity);

    // Fuzz AV1E_SET_AUTO_INTRA_TOOLS_OFF
    int auto_intra_off = Data[6] % 2;
    aom_codec_control(&ctx, AV1E_SET_AUTO_INTRA_TOOLS_OFF, auto_intra_off);

    // Fuzz AV1E_SET_MAX_REFERENCE_FRAMES
    int max_ref_frames = (Data[7] % 8) + 1;
    aom_codec_control(&ctx, AV1E_SET_MAX_REFERENCE_FRAMES, max_ref_frames);

    // Fuzz AV1E_SET_MAX_CONSEC_FRAME_DROP_CBR
    int max_drop = Data[8] % 10;
    aom_codec_control(&ctx, AV1E_SET_MAX_CONSEC_FRAME_DROP_CBR, max_drop);

    // Cleanup
    cleanup_codec(&ctx);

    return 0;
}