// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
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

static aom_codec_ctx_t *init_encoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *codec = NULL;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t ret;

    iface = aom_codec_av1_cx();
    if (!iface) return NULL;

    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return NULL;
    }

    /* Use fuzz input to configure width/height within reasonable bounds to avoid OOM */
    /* Ensure we have enough data before accessing indices 2 and 3 */
    if (size >= 4) {
        uint16_t w = data[0] | (data[1] << 8);
        uint16_t h = data[2] | (data[3] << 8);
        if (w == 0) w = 64;
        if (h == 0) h = 64;
        /* Cap dimensions to prevent excessive memory allocation */
        if (w > 4096) w = 4096;
        if (h > 4096) h = 4096;
        cfg.g_w = w;
        cfg.g_h = h;
    } else {
        cfg.g_w = 64;
        cfg.g_h = 64;
    }

    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.rc_target_bitrate = 1000;

    codec = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!codec) return NULL;
    memset(codec, 0, sizeof(aom_codec_ctx_t));

    ret = aom_codec_enc_init(codec, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        free(codec);
        return NULL;
    }

    return codec;
}

int LLVMFuzzerTestOneInput_121(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    aom_codec_ctx_t *codec = init_encoder(Data, Size);
    if (!codec) return 0;

    int value;
    aom_codec_err_t ret;

    /* 
     * We attempt to set various AV1 encoder options using aom_codec_control.
     * The specific constants correspond to the flags mentioned in the task:
     * AV1E_SET_ENABLE_FLIP_IDTX
     * AV1E_SET_ENABLE_DIST_WTD_COMP
     * AV1E_SET_ENABLE_INTRABC
     * AV1E_SET_ENABLE_DIFF_WTD_COMP
     * AV1E_SET_ENABLE_ANGLE_DELTA
     * AV1E_SET_ENABLE_DUAL_FILTER
     */

    /* Use input data to determine the value (0 or 1) for the flags */
    value = Data[0] & 0x01;

    /* 1. AV1E_SET_ENABLE_FLIP_IDTX */
    ret = aom_codec_control(codec, AV1E_SET_ENABLE_FLIP_IDTX, value);
    (void)ret; /* Ignore return for fuzzing, just trigger code path */

    /* 2. AV1E_SET_ENABLE_DIST_WTD_COMP */
    if (Size > 1) value = Data[1] & 0x01;
    ret = aom_codec_control(codec, AV1E_SET_ENABLE_DIST_WTD_COMP, value);
    (void)ret;

    /* 3. AV1E_SET_ENABLE_INTRABC */
    if (Size > 2) value = Data[2] & 0x01;
    ret = aom_codec_control(codec, AV1E_SET_ENABLE_INTRABC, value);
    (void)ret;

    /* 4. AV1E_SET_ENABLE_DIFF_WTD_COMP */
    if (Size > 3) value = Data[3] & 0x01;
    ret = aom_codec_control(codec, AV1E_SET_ENABLE_DIFF_WTD_COMP, value);
    (void)ret;

    /* 5. AV1E_SET_ENABLE_ANGLE_DELTA */
    if (Size > 4) value = Data[4] & 0x01;
    ret = aom_codec_control(codec, AV1E_SET_ENABLE_ANGLE_DELTA, value);
    (void)ret;

    /* 6. AV1E_SET_ENABLE_DUAL_FILTER */
    if (Size > 5) value = Data[5] & 0x01;
    ret = aom_codec_control(codec, AV1E_SET_ENABLE_DUAL_FILTER, value);
    (void)ret;

    /* Cleanup */
    aom_codec_destroy(codec);
    free(codec);

    return 0;
}