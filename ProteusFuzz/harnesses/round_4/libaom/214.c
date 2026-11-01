// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
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

static int init_encoder(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t res;

    // Use the correct function to get the AV1 encoder interface
    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    res = aom_codec_enc_config_default(iface, &cfg, 0);
    if (res != AOM_CODEC_OK) return 0;

    cfg.g_w = (size_t)(data[0] % 256) + 1;
    cfg.g_h = (size_t)(data[1] % 256) + 1;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;

    res = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) return 0;

    return 1;
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    aom_codec_destroy(ctx);
}

int LLVMFuzzerTestOneInput_214(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    if (!init_encoder(&ctx, Data, Size)) {
        return 0;
    }

    int8_t val8;
    int val32;
    aom_codec_err_t res;

    // 1. aom_codec_control_typechecked_AV1E_SET_ENABLE_ONESIDED_COMP
    if (Size > 2) {
        val8 = (int8_t)(Data[2] % 2);
        res = aom_codec_control(&ctx, AV1E_SET_ENABLE_ONESIDED_COMP, val8);
        (void)res;
    }

    // 2. aom_codec_control_typechecked_AV1E_GET_NUM_OPERATING_POINTS
    if (Size > 3) {
        res = aom_codec_control(&ctx, AV1E_GET_NUM_OPERATING_POINTS, &val32);
        (void)res;
    }

    // 3. aom_codec_control_typechecked_AV1E_SET_ENABLE_WARPED_MOTION
    if (Size > 4) {
        val8 = (int8_t)(Data[4] % 2);
        res = aom_codec_control(&ctx, AV1E_SET_ENABLE_WARPED_MOTION, val8);
        (void)res;
    }

    // 4. aom_codec_control_typechecked_AV1E_SET_FILM_GRAIN_TEST_VECTOR
    // Passing a pointer to the data buffer as a simulated test vector
    if (Size > 5) {
        res = aom_codec_control(&ctx, AV1E_SET_FILM_GRAIN_TEST_VECTOR, (void*)(Data + 5));
        (void)res;
    }

    // 5. aom_codec_control_typechecked_AV1E_GET_BASELINE_GF_INTERVAL
    if (Size > 6) {
        res = aom_codec_control(&ctx, AV1E_GET_BASELINE_GF_INTERVAL, &val32);
        (void)res;
    }

    // 6. aom_codec_control_typechecked_AV1E_SET_INTRA_DEFAULT_TX_ONLY
    if (Size > 7) {
        val8 = (int8_t)(Data[7] % 2);
        res = aom_codec_control(&ctx, AV1E_SET_INTRA_DEFAULT_TX_ONLY, val8);
        (void)res;
    }

    cleanup_encoder(&ctx);
    return 0;
}