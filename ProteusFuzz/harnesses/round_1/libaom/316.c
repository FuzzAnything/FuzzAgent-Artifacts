// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
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

/*
 * Fuzz driver for libaom codec control functions.
 * Target functions:
 * - AV1E_SET_SKIP_POSTPROC_FILTERING
 * - AV1E_SET_DENOISE_NOISE_LEVEL
 * - AV1_SET_SKIP_LOOP_FILTER
 * - AV1E_SET_ENABLE_INTRABC
 * - AV1E_SET_LOOPFILTER_CONTROL
 * - AOME_GET_LOOPFILTER_LEVEL
 */

static int init_encoder(aom_codec_ctx_t *codec, const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t ret;
    aom_img_fmt_t fmt;
    int w, h;

    // Determine width and height from input data if possible, otherwise default
    if (size >= 4) {
        w = data[0] + 1;
        h = data[1] + 1;
        if (w > 4096 || h > 4096) { w = 192; h = 192; } // Sanity check
    } else {
        w = 192;
        h = 192;
    }

    // Determine format
    if (size > 2) {
        fmt = (aom_img_fmt_t)(data[2] % 4); // AOM_IMG_FMT_YV12, etc.
    } else {
        fmt = AOM_IMG_FMT_YV12;
    }

    iface = aom_codec_av1_cx();
    if (!iface) return -1;

    ret = aom_codec_enc_config_default(iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) return -1;

    cfg.g_w = w;
    cfg.g_h = h;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_usage = AOM_USAGE_REALTIME;

    ret = aom_codec_enc_init(codec, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        aom_codec_destroy(codec);
        return -1;
    }

    return 0;
}

static int init_decoder(aom_codec_ctx_t *codec, const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface;
    aom_codec_dec_cfg_t cfg;
    aom_codec_err_t ret;

    iface = aom_codec_av1_dx();
    if (!iface) return -1;

    ret = aom_codec_dec_init(codec, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        aom_codec_destroy(codec);
        return -1;
    }

    return 0;
}

int LLVMFuzzerTestOneInput_316(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t codec;
    aom_codec_err_t ret;
    int i;

    if (Size < 1) return 0;

    // Initialize codec context
    memset(&codec, 0, sizeof(codec));

    // Decide to test encoder or decoder based on first byte
    if (Data[0] % 2 == 0) {
        // Encoder path
        if (init_encoder(&codec, Data, Size) != 0) return 0;
    } else {
        // Decoder path
        if (init_decoder(&codec, Data, Size) != 0) return 0;
    }

    // Iterate through the target control functions
    // Using the correct enum values from aomcx.h (e.g., AV1E_SET_...)
    // without the AOM_CODEC_ prefix which was causing the error.

    // 1. AV1E_SET_SKIP_POSTPROC_FILTERING
    // Value: 0 or 1 (boolean)
    int val1 = (Size > 1) ? (Data[1] % 2) : 0;
    ret = aom_codec_control(&codec, AV1E_SET_SKIP_POSTPROC_FILTERING, val1);
    (void)ret;

    // 2. AV1E_SET_DENOISE_NOISE_LEVEL
    // Value: int (noise level)
    int val2 = (Size > 2) ? (Data[2] * 10) : 0;
    ret = aom_codec_control(&codec, AV1E_SET_DENOISE_NOISE_LEVEL, val2);
    (void)ret;

    // 3. AV1_SET_SKIP_LOOP_FILTER
    // Value: 0 or 1 (boolean)
    int val3 = (Size > 3) ? (Data[3] % 2) : 0;
    ret = aom_codec_control(&codec, AV1_SET_SKIP_LOOP_FILTER, val3);
    (void)ret;

    // 4. AV1E_SET_ENABLE_INTRABC
    // Value: 0 or 1 (boolean)
    int val4 = (Size > 4) ? (Data[4] % 2) : 0;
    ret = aom_codec_control(&codec, AV1E_SET_ENABLE_INTRABC, val4);
    (void)ret;

    // 5. AV1E_SET_LOOPFILTER_CONTROL
    // Value: int (loop filter level)
    int val5 = (Size > 5) ? (Data[5] * 5) : 0;
    ret = aom_codec_control(&codec, AV1E_SET_LOOPFILTER_CONTROL, val5);
    (void)ret;

    // 6. AOME_GET_LOOPFILTER_LEVEL
    // This is a GET operation.
    // We pass a dummy pointer if needed.
    int dummy_val = 0;
    ret = aom_codec_control(&codec, AOME_GET_LOOPFILTER_LEVEL, &dummy_val);
    (void)ret;

    // Cleanup
    aom_codec_destroy(&codec);

    return 0;
}