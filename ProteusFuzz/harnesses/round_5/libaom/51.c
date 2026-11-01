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
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_integer.h>

static int fuzz_codec_control(aom_codec_ctx_t *ctx, int control_id, int value) {
    if (!ctx) return 0;
    return aom_codec_control(ctx, control_id, value);
}

int LLVMFuzzerTestOneInput_51(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    // Use input data to derive parameters
    int width = (Data[0] % 128) + 16;
    int height = (Data[1] % 128) + 16;
    int bit_depth = (Data[2] % 2) + 8; // 8 or 10
    int color_space = Data[3] % 3; // 0, 1, or 2

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t ret;

    // Get AV1 encoder interface
    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    // Initialize encoder config
    ret = aom_codec_enc_config_default(iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) return 0;

    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_bit_depth = bit_depth;
    // Corrected: aom_codec_enc_cfg_t does not have g_colorspace directly.
    // We use cfg.g_input_colorspace if available, or rely on defaults.
    // For AV1, we often set it via the specific AV1 config or just leave default.
    // To be safe and compile, we skip setting it here as it's not a direct member
    // in older versions or handled differently.
    // However, if the struct has g_input_colorspace (newer aom), we could set it.
    // Assuming standard struct, we omit this line to avoid compilation error.
    
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.rc_target_bitrate = 500;
    cfg.rc_min_quantizer = 0;
    cfg.rc_max_quantizer = 63;

    // Initialize encoder context
    ret = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) return 0;

    // Fuzz control functions based on input data
    // Extract values from input to fuzz control parameters
    int val1 = (Size > 4) ? Data[4] % 2 : 0;
    int val2 = (Size > 5) ? Data[5] % 2 : 0;
    int val3 = (Size > 6) ? Data[6] % 2 : 0;
    int val4 = (Size > 7) ? Data[7] % 2 : 0;
    int val5 = (Size > 8) ? Data[8] % 2 : 0;
    int val6 = (Size > 9) ? Data[9] % 2 : 0;

    // Call control functions if data is sufficient
    if (Size > 4) {
        fuzz_codec_control(&ctx, AV1E_SET_ENABLE_DIFF_WTD_COMP, val1);
    }
    if (Size > 5) {
        fuzz_codec_control(&ctx, AV1E_SET_ENABLE_INTRABC, val2);
    }
    if (Size > 6) {
        fuzz_codec_control(&ctx, AV1E_SET_ENABLE_DUAL_FILTER, val3);
    }
    if (Size > 7) {
        fuzz_codec_control(&ctx, AV1E_SET_ENABLE_ANGLE_DELTA, val4);
    }
    if (Size > 8) {
        fuzz_codec_control(&ctx, AV1E_SET_ENABLE_FLIP_IDTX, val5);
    }
    if (Size > 9) {
        fuzz_codec_control(&ctx, AV1E_SET_ENABLE_DIST_WTD_COMP, val6);
    }

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}