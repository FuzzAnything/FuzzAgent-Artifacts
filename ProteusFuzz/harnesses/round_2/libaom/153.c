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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aom_decoder.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>
#include <aom/aomcx.h>
#include <aom/aom_image.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom.h>

static int test_codec_control(aom_codec_ctx_t *ctx, int control_id, int value) {
    if (!ctx) return 0;
    return aom_codec_control(ctx, control_id, value);
}

int LLVMFuzzerTestOneInput_153(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    aom_codec_ctx_t ctx = {0};
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);
    cfg.g_w = 128;
    cfg.g_h = 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;

    aom_codec_err_t err = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    int32_t val = 0;
    if (Size >= 4) {
        val = *(int32_t *)Data;
    }

    // Test AV1E_SET_ENABLE_DIFF_WTD_COMP
    if (val & 1) {
        test_codec_control(&ctx, AV1E_SET_ENABLE_DIFF_WTD_COMP, val & 0xFF);
    }

    // Test AV1E_SET_TRANSFER_CHARACTERISTICS
    if (val & 2) {
        test_codec_control(&ctx, AV1E_SET_TRANSFER_CHARACTERISTICS, val & 0xFF);
    }

    // Test AV1E_SET_COLOR_RANGE
    if (val & 4) {
        test_codec_control(&ctx, AV1E_SET_COLOR_RANGE, val & 0xFF);
    }

    // Test AV1E_SET_CDF_UPDATE_MODE
    if (val & 8) {
        test_codec_control(&ctx, AV1E_SET_CDF_UPDATE_MODE, val & 0xFF);
    }

    // Test AV1E_SET_INTRA_DEFAULT_TX_ONLY
    if (val & 16) {
        test_codec_control(&ctx, AV1E_SET_INTRA_DEFAULT_TX_ONLY, val & 0xFF);
    }

    // Test AV1E_SET_ENABLE_GLOBAL_MOTION
    if (val & 32) {
        test_codec_control(&ctx, AV1E_SET_ENABLE_GLOBAL_MOTION, val & 0xFF);
    }

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}