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

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, int cmd, int val) {
    if (!ctx) return 0;
    return aom_codec_control(ctx, cmd, val);
}

int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) return 0;

    // Use input data to configure basic settings safely
    cfg.g_w = (Data[0] % 512) + 16;
    cfg.g_h = (Data[1] % 512) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.g_threads = 1;
    cfg.g_error_resilient = 0;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Extract values from input for fuzzing control commands
    int val1 = (Size > 2) ? (Data[2] % 2) : 0;
    int val2 = (Size > 3) ? (Data[3] % 2) : 0;
    int val3 = (Size > 4) ? (Data[4] % 2) : 0;
    int val4 = (Size > 5) ? (Data[5] % 2) : 0;
    int val5 = (Size > 6) ? (Data[6] % 2) : 0;
    int val6 = (Size > 7) ? (Data[7] % 2) : 0;

    // Fuzz the target API functions using correct enum names from aomcx.h
    fuzz_encoder_control(&ctx, AV1E_SET_ENABLE_AB_PARTITIONS, val1);
    fuzz_encoder_control(&ctx, AV1E_SET_COEFF_COST_UPD_FREQ, val2);
    fuzz_encoder_control(&ctx, AV1E_SET_ENABLE_INTERINTER_WEDGE, val3);
    fuzz_encoder_control(&ctx, AV1E_SET_ENABLE_REF_FRAME_MVS, val4);
    fuzz_encoder_control(&ctx, AV1E_SET_RTC_EXTERNAL_RC, val5);
    fuzz_encoder_control(&ctx, AV1E_SET_ENABLE_SMOOTH_INTERINTRA, val6);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}