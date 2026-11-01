// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
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

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx || !data || size == 0) return 0;

    int ret = 0;
    int val;
    int32_t val32;
    aom_codec_err_t err;

    // Helper to read int from fuzz data
    #define READ_INT(idx) ((idx < size) ? (int)data[idx] : 0)
    #define READ_INT32(idx) ((idx + 3 < size) ? (int32_t)(data[idx] | (data[idx+1]<<8) | (data[idx+2]<<16) | (data[idx+3]<<24)) : 0)

    // 1. AV1E_SET_ENABLE_DIRECTIONAL_INTRA
    if (size >= 1) {
        val = READ_INT(0);
        err = aom_codec_control(ctx, AV1E_SET_ENABLE_DIRECTIONAL_INTRA, val);
        (void)err;
    }

    // 2. AV1E_SET_MIN_PARTITION_SIZE
    if (size >= 1) {
        val = READ_INT(1);
        // Ensure valid range for partition size if needed, but fuzzing allows edge cases
        err = aom_codec_control(ctx, AV1E_SET_MIN_PARTITION_SIZE, val);
        (void)err;
    }

    // 3. AV1E_SET_RENDER_SIZE
    if (size >= 4) {
        int32_t width = READ_INT32(2);
        int32_t height = READ_INT32(6);
        if (width > 0 && height > 0 && width < 16384 && height < 16384) {
            err = aom_codec_control(ctx, AV1E_SET_RENDER_SIZE, width, height);
            (void)err;
        }
    }

    // 4. AV1E_GET_SEQ_LEVEL_IDX
    if (size >= 1) {
        val = 0;
        err = aom_codec_control(ctx, AV1E_GET_SEQ_LEVEL_IDX, &val);
        (void)err;
    }

    // 5. AV1E_SET_ENABLE_PAETH_INTRA
    if (size >= 1) {
        val = READ_INT(0);
        err = aom_codec_control(ctx, AV1E_SET_ENABLE_PAETH_INTRA, val);
        (void)err;
    }

    // 6. AV1E_SET_ENABLE_SUPERRES
    if (size >= 1) {
        val = READ_INT(0);
        err = aom_codec_control(ctx, AV1E_SET_ENABLE_SUPERRES, val);
        (void)err;
    }

    return ret;
}

int LLVMFuzzerTestOneInput_177(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t err;
    int ret = 0;

    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    // Initialize encoder config
    err = aom_codec_enc_config_default(aom_codec_av1_cx(), &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    // Set some basic config to avoid immediate failures
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.g_lag_in_frames = 0;

    // Initialize codec context
    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    err = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    // Run the fuzzing logic on the control functions
    ret = fuzz_encoder_control(&ctx, Data, Size);

    // Cleanup
    aom_codec_destroy(&ctx);

    return ret;
}