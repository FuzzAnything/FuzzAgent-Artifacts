// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
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

static int get_int(const uint8_t *data, size_t *offset, size_t max, int default_val) {
    if (*offset + sizeof(int) > max) return default_val;
    int val;
    memcpy(&val, data + *offset, sizeof(int));
    *offset += sizeof(int);
    return val;
}

static int fuzz_encoder_controls(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx) return 0;

    size_t offset = 0;

    // AV1E_SET_ENABLE_RECT_TX
    if (offset < size) {
        int val = get_int(data, &offset, size, 0);
        aom_codec_control(ctx, AV1E_SET_ENABLE_RECT_TX, val);
    }

    // AV1E_SET_ENABLE_INTERINTRA_COMP
    if (offset < size) {
        int val = get_int(data, &offset, size, 0);
        aom_codec_control(ctx, AV1E_SET_ENABLE_INTERINTRA_COMP, val);
    }

    // AV1E_SET_ENABLE_INTERINTRA_WEDGE
    if (offset < size) {
        int val = get_int(data, &offset, size, 0);
        aom_codec_control(ctx, AV1E_SET_ENABLE_INTERINTRA_WEDGE, val);
    }

    // AV1E_SET_TUNE_CONTENT
    if (offset < size) {
        int val = get_int(data, &offset, size, 0);
        // Clamp to valid range if necessary, though API usually handles it
        if (val < 0 || val > 2) val = 0; 
        aom_codec_control(ctx, AV1E_SET_TUNE_CONTENT, val);
    }

    // AV1E_SET_ENABLE_DUAL_FILTER
    if (offset < size) {
        int val = get_int(data, &offset, size, 0);
        aom_codec_control(ctx, AV1E_SET_ENABLE_DUAL_FILTER, val);
    }

    // AV1E_SET_ALLOW_WARPED_MOTION
    if (offset < size) {
        int val = get_int(data, &offset, size, 0);
        aom_codec_control(ctx, AV1E_SET_ALLOW_WARPED_MOTION, val);
    }

    return 0;
}

int LLVMFuzzerTestOneInput_159(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    // Initialize encoder configuration
    aom_codec_enc_cfg_t cfg;
    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface;
    aom_codec_err_t ret;

    // Get AV1 encoder interface (using the correct function name)
    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    // Set default config
    ret = aom_codec_enc_config_default(iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) return 0;

    // Configure basic parameters from fuzz data
    cfg.g_w = (Data[0] % 128) + 16;
    cfg.g_h = (Data[1] % 128) + 16;
    cfg.g_timebase.num = Data[2] + 1;
    cfg.g_timebase.den = Data[3] + 1;
    cfg.g_threads = (Data[4] % 4) + 1;
    cfg.rc_target_bitrate = (Data[5] % 100) + 100;
    cfg.g_pass = AOM_RC_ONE_PASS;
    cfg.g_profile = (Data[6] % 4);
    cfg.g_usage = AOM_USAGE_REALTIME;
    cfg.rc_end_usage = AOM_CBR;

    // Initialize codec context
    ret = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    // Invoke the fuzzing logic for encoder controls
    fuzz_encoder_controls(&ctx, Data, Size);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}