// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_integer.h>

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, int control_id, int value) {
    if (!ctx) return 0;
    return aom_codec_control(ctx, control_id, value);
}

int LLVMFuzzerTestOneInput_180(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    // Use the standard interface retrieval function for AV1 encoder
    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) return 0;

    // Use input data to configure basic parameters with bounds checking
    cfg.g_w = (Data[0] % 256) + 16; // Minimum width 16
    cfg.g_h = (Data[1] % 256) + 16; // Minimum height 16
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = (Data[2] % 100) + 1;
    cfg.rc_target_bitrate = (Data[3] % 100) + 100;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz various control functions with values derived from input
    int value = (Data[4] % 2);
    
    // AV1E_SET_INTRA_DCT_ONLY
    fuzz_encoder_control(&ctx, AV1E_SET_INTRA_DCT_ONLY, value);
    
    // AV1E_SET_COLOR_PRIMARIES (values 0-14 typically)
    int color_primaries = (Data[5] % 15);
    fuzz_encoder_control(&ctx, AV1E_SET_COLOR_PRIMARIES, color_primaries);
    
    // AV1E_SET_ALLOW_WARPED_MOTION
    int allow_warped = (Data[6] % 2);
    fuzz_encoder_control(&ctx, AV1E_SET_ALLOW_WARPED_MOTION, allow_warped);
    
    // AV1E_SET_ENABLE_SUPERRES
    int enable_superres = (Data[7] % 2);
    fuzz_encoder_control(&ctx, AV1E_SET_ENABLE_SUPERRES, enable_superres);
    
    // AV1E_SET_ENABLE_WARPED_MOTION
    int enable_warped = (Data[8] % 2);
    fuzz_encoder_control(&ctx, AV1E_SET_ENABLE_WARPED_MOTION, enable_warped);
    
    // AV1E_SET_ENABLE_CHROMA_DELTAQ
    int enable_chroma_deltaq = (Data[9] % 2);
    fuzz_encoder_control(&ctx, AV1E_SET_ENABLE_CHROMA_DELTAQ, enable_chroma_deltaq);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}