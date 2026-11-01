// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aomdx.h"
#include "aom_encoder.h"
#include "aom_frame_buffer.h"
#include "aom_decoder.h"
#include "aom_image.h"
#include "aom.h"
#include "aom_integer.h"
#include "aom_codec.h"
#include "aomcx.h"
#include "aom_external_partition.h"

/*
 * Dummy inspection callback for fuzzing purposes.
 * The signature must match aom_inspect_cb: void (*)(void *, void *)
 */
static void dummy_inspect_cb(void *ctx, void *ctx_data) {
    (void)ctx;
    (void)ctx_data;
    return;
}

static void cleanup_codec(aom_codec_ctx_t *codec) {
    if (codec) {
        aom_codec_destroy(codec);
        free(codec);
    }
}

static int init_encoder(aom_codec_ctx_t **codec_out, const uint8_t *data, size_t size) {
    aom_codec_ctx_t *codec = NULL;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t ret;

    if (size < 2) return -1;

    iface = aom_codec_av1_cx();
    if (!iface) return -1;

    codec = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!codec) return -1;
    memset(codec, 0, sizeof(aom_codec_ctx_t));

    ret = aom_codec_enc_config_default(iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        cleanup_codec(codec);
        return -1;
    }

    /* Use fuzzing data to tweak config slightly to explore states */
    cfg.g_w = (data[0] % 1024) + 16;
    cfg.g_h = (data[1] % 1024) + 16;
    cfg.rc_target_bitrate = 1000 + (data[0] * data[1]);

    ret = aom_codec_enc_init(codec, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        cleanup_codec(codec);
        return -1;
    }

    *codec_out = codec;
    return 0;
}

int LLVMFuzzerTestOneInput_279(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t *codec = NULL;
    aom_inspect_init inspect_init;
    int result = 0;

    if (Size < 3) return 0;

    /* Initialize encoder context */
    if (init_encoder(&codec, Data, Size) != 0) {
        return 0;
    }

    /* 1. AV1_SET_INSPECTION_CALLBACK */
    inspect_init.inspect_cb = dummy_inspect_cb;
    inspect_init.inspect_ctx = (void *)(uintptr_t)Data;
    result = aom_codec_control(codec, AV1_SET_INSPECTION_CALLBACK, &inspect_init);
    (void)result;

    /* 2. AV1E_ENABLE_MOTION_VECTOR_UNIT_TEST */
    result = aom_codec_control(codec, AV1E_ENABLE_MOTION_VECTOR_UNIT_TEST, Data);
    (void)result;

    /* 3. AV1E_SET_FILM_GRAIN_TEST_VECTOR */
    result = aom_codec_control(codec, AV1E_SET_FILM_GRAIN_TEST_VECTOR, Data);
    (void)result;

    /* 4. AV1E_SET_ENABLE_DUAL_FILTER */
    int dual_filter_flag = (Data[0] % 2);
    result = aom_codec_control(codec, AV1E_SET_ENABLE_DUAL_FILTER, &dual_filter_flag);
    (void)result;

    /* 5. AV1E_SET_FP_MT_UNIT_TEST */
    int fp_mt_flag = (Data[1] % 2);
    result = aom_codec_control(codec, AV1E_SET_FP_MT_UNIT_TEST, &fp_mt_flag);
    (void)result;

    /* 6. AV1E_SET_POSTENCODE_DROP_RTC */
    int drop_rtc_flag = (Data[2] % 2);
    result = aom_codec_control(codec, AV1E_SET_POSTENCODE_DROP_RTC, &drop_rtc_flag);
    (void)result;

    /* Cleanup */
    cleanup_codec(codec);

    return 0;
}