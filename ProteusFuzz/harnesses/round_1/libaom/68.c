// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
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

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_68(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t err;

    if (!iface) {
        return 0;
    }

    ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) {
        return 0;
    }

    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    err = aom_codec_enc_config_default(iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        cleanup_codec(ctx);
        return 0;
    }

    cfg.g_w = 128;
    cfg.g_h = 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;

    err = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        cleanup_codec(ctx);
        return 0;
    }

    // Use input data to diversify control values
    uint32_t val = 0;
    if (Size >= 4) {
        val = *(const uint32_t *)Data;
    }

    // Fuzz AV1E_SET_SCREEN_CONTENT_DETECTION_MODE
    aom_codec_control(ctx, AV1E_SET_SCREEN_CONTENT_DETECTION_MODE, val & 0xFF);

    // Fuzz AV1E_SET_ENABLE_CFL_INTRA
    aom_codec_control(ctx, AV1E_SET_ENABLE_CFL_INTRA, (val >> 8) & 0xFF);

    // Fuzz AV1E_SET_FILM_GRAIN_TEST_VECTOR
    // This typically expects a pointer or struct, we pass a truncated value or NULL
    if (val & 0x100) {
        // Attempt to pass a pointer derived from data if size allows
        if (Size >= 16) {
            aom_codec_control(ctx, AV1E_SET_FILM_GRAIN_TEST_VECTOR, (void *)(uintptr_t)Data);
        }
    } else {
        aom_codec_control(ctx, AV1E_SET_FILM_GRAIN_TEST_VECTOR, NULL);
    }

    // Fuzz AV1E_SET_ENABLE_ANGLE_DELTA
    aom_codec_control(ctx, AV1E_SET_ENABLE_ANGLE_DELTA, (val >> 16) & 0xFF);

    // Fuzz AV1E_SET_ENABLE_DUAL_FILTER
    aom_codec_control(ctx, AV1E_SET_ENABLE_DUAL_FILTER, (val >> 24) & 0xFF);

    // Fuzz AV1E_SET_ENABLE_1TO4_PARTITIONS
    aom_codec_control(ctx, AV1E_SET_ENABLE_1TO4_PARTITIONS, val & 0xFF);

    // Cleanup
    cleanup_codec(ctx);

    return 0;
}