// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control_typechecked_AV1E_SET_FILM_GRAIN_TEST_VECTOR at aomcx.h:2177:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_RTC_EXTERNAL_RC at aomcx.h:2306:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_ENABLE_FILTER_INTRA at aomcx.h:2135:1 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_TIMING_INFO_TYPE at aomcx.h:2174:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_ENABLE_REF_FRAME_MVS at aomcx.h:2093:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_ENABLE_INTRA_EDGE_FILTER at aomcx.h:2075:1 in aomcx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aom_codec.h"
#include "aom/aomdx.h"
#include "aom/aom_integer.h"
#include "aom/aom_image.h"
#include "aom/aomcx.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom.h"

/* Helper function to initialize an AV1 encoder context */
static aom_codec_ctx_t *init_encoder(aom_codec_iface_t *iface, int width, int height) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t ret;

    if (iface == NULL) return NULL;

    ctx = malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    ret = aom_codec_enc_config_default(iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.g_threads = 0;
    cfg.g_error_resilient = 0;

    ret = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

/* Helper function to clean up encoder context */
static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

/* Helper function to write dummy data to a file if needed */
static void create_dummy_file(void) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        const char dummy[] = "dummy_data_for_fuzzing";
        fwrite(dummy, 1, sizeof(dummy) - 1, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_77(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    create_dummy_file();

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    /* Use input data to derive width and height, ensuring they are valid */
    int width = (Data[0] % 64) + 16;
    int height = (Data[1] % 64) + 16;
    if (Size > 2) {
        width = (Data[2] % 256) + 16;
        height = (Data[3] % 256) + 16;
    }

    aom_codec_ctx_t *ctx = init_encoder(iface, width, height);
    if (!ctx) return 0;

    /* Fuzz AV1E_SET_TIMING_INFO_TYPE: expects (ctx, ctrl, data) */
    aom_codec_control_typechecked_AV1E_SET_TIMING_INFO_TYPE(ctx, AV1E_SET_TIMING_INFO_TYPE, Data[0] % 100);

    /* Fuzz AV1E_SET_ENABLE_REF_FRAME_MVS: expects (ctx, ctrl, data) */
    aom_codec_control_typechecked_AV1E_SET_ENABLE_REF_FRAME_MVS(ctx, AV1E_SET_ENABLE_REF_FRAME_MVS, Data[0] % 2);

    /* Fuzz AV1E_SET_ENABLE_INTRA_EDGE_FILTER: expects (ctx, ctrl, data) */
    aom_codec_control_typechecked_AV1E_SET_ENABLE_INTRA_EDGE_FILTER(ctx, AV1E_SET_ENABLE_INTRA_EDGE_FILTER, Data[1] % 2);

    /* Fuzz AV1E_SET_FILM_GRAIN_TEST_VECTOR: expects (ctx, ctrl, data) */
    /* Assuming the data parameter is an int or pointer cast to int for fuzzing purposes */
    if (Size >= 4) {
        int film_grain_data = (Data[2] << 8) | Data[3];
        aom_codec_control_typechecked_AV1E_SET_FILM_GRAIN_TEST_VECTOR(ctx, AV1E_SET_FILM_GRAIN_TEST_VECTOR, film_grain_data);
    }

    /* Fuzz AV1E_SET_RTC_EXTERNAL_RC: expects (ctx, ctrl, data) */
    aom_codec_control_typechecked_AV1E_SET_RTC_EXTERNAL_RC(ctx, AV1E_SET_RTC_EXTERNAL_RC, Data[2] % 2);

    /* Fuzz AV1E_SET_ENABLE_FILTER_INTRA: expects (ctx, ctrl, data) */
    aom_codec_control_typechecked_AV1E_SET_ENABLE_FILTER_INTRA(ctx, AV1E_SET_ENABLE_FILTER_INTRA, Data[3] % 2);

    cleanup_encoder(ctx);

    return 0;
}