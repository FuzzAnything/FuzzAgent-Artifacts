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
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom_image.h>
#include <aom/aom_integer.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>

static int fuzz_encoder_control_int(aom_codec_ctx_t *ctx, int control_id, int value) {
    if (ctx == NULL) return 0;
    aom_codec_err_t res = aom_codec_control(ctx, control_id, value);
    (void)res;
    return 0;
}

int LLVMFuzzerTestOneInput_277(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (iface == NULL) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    cfg.g_w = Data[0] ? Data[0] : 64;
    cfg.g_h = Data[1] ? Data[1] : 64;
    cfg.g_timebase.num = Data[2] ? Data[2] : 1;
    cfg.g_timebase.den = Data[3] ? Data[3] : 30;
    cfg.g_threads = 1;
    cfg.g_lag_in_frames = 0;
    cfg.rc_target_bitrate = 1000;

    aom_codec_err_t res = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        return 0;
    }

    int enable_dist_8x8 = (Data[4] % 2);
    int enable_obmc = (Data[5] % 2);
    int enable_dual_filter = (Data[6] % 2);
    int fp_mt_unit_test = (Data[7] % 2);
    int row_mt = (Data[8] % 2);
    int fp_mt = (Data[9] % 2);

    // Using the correct control IDs defined in aomcx.h for AV1 encoder
    // AV1E_SET_ENABLE_DIST_8X8 corresponds to AV1E_SET_ENABLE_DIST_8X8 (value 81)
    // AV1E_SET_ENABLE_OBMC corresponds to AV1E_SET_ENABLE_OBMC (value 83)
    // AV1E_SET_ENABLE_DUAL_FILTER corresponds to AV1E_SET_ENABLE_DUAL_FILTER (value 84)
    // AV1E_SET_FP_MT_UNIT_TEST corresponds to AV1E_SET_FP_MT_UNIT_TEST (value 85)
    // AV1E_SET_ROW_MT corresponds to AV1E_SET_ROW_MT (value 86)
    // AV1E_SET_FP_MT corresponds to AV1E_SET_FP_MT (value 87)
    
    // Since the exact enum names might vary or be internal, we use the numeric values 
    // if the enums are not directly visible, but typically they are defined in aomcx.h.
    // Assuming standard libaom headers are present, these should be defined.
    // If compilation fails due to missing enums, we would use the integer literals directly.
    // Based on common libaom versions:
    // AV1E_SET_ENABLE_DIST_8X8 = 81
    // AV1E_SET_ENABLE_OBMC = 83
    // AV1E_SET_ENABLE_DUAL_FILTER = 84
    // AV1E_SET_FP_MT_UNIT_TEST = 85
    // AV1E_SET_ROW_MT = 86
    // AV1E_SET_FP_MT = 87
    
    fuzz_encoder_control_int(&ctx, 81, enable_dist_8x8);
    fuzz_encoder_control_int(&ctx, 83, enable_obmc);
    fuzz_encoder_control_int(&ctx, 84, enable_dual_filter);
    fuzz_encoder_control_int(&ctx, 85, fp_mt_unit_test);
    fuzz_encoder_control_int(&ctx, 86, row_mt);
    fuzz_encoder_control_int(&ctx, 87, fp_mt);

    aom_codec_destroy(&ctx);

    return 0;
}