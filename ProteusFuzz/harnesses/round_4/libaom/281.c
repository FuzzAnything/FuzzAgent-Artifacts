// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
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

#include "aom/aomdx.h"
#include "aom/aom_encoder.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_decoder.h"
#include "aom/aom_image.h"
#include "aom/aom.h"
#include "aom/aom_integer.h"
#include "aom/aom_codec.h"
#include "aom/aomcx.h"
#include "aom/aom_external_partition.h"

static int test_encoder_controls(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx || !ctx->iface) return 0;

    int idx = 0;

    // AV1E_GET_LUMA_CDEF_STRENGTH
    if (idx + sizeof(int) <= size) {
        int strength;
        memcpy(&strength, &data[idx], sizeof(int));
        idx += sizeof(int);
        aom_codec_control(ctx, AV1E_GET_LUMA_CDEF_STRENGTH, &strength);
    }

    // AV1E_SET_FILM_GRAIN_TEST_VECTOR
    if (idx + sizeof(int) <= size) {
        int enable;
        memcpy(&enable, &data[idx], sizeof(int));
        idx += sizeof(int);
        aom_codec_control(ctx, AV1E_SET_FILM_GRAIN_TEST_VECTOR, &enable);
    }

    // AV1E_SET_FILM_GRAIN_TABLE
    if (idx + sizeof(int) <= size) {
        int dummy_val;
        memcpy(&dummy_val, &data[idx], sizeof(int));
        idx += sizeof(int);
        aom_codec_control(ctx, AV1E_SET_FILM_GRAIN_TABLE, (void*)&data[idx]);
    }

    // AV1E_SET_ENABLE_TX64
    if (idx + sizeof(int) <= size) {
        int enable;
        memcpy(&enable, &data[idx], sizeof(int));
        idx += sizeof(int);
        aom_codec_control(ctx, AV1E_SET_ENABLE_TX64, &enable);
    }

    // AV1E_SET_MATRIX_COEFFICIENTS
    if (idx + sizeof(int) <= size) {
        int coeffs;
        memcpy(&coeffs, &data[idx], sizeof(int));
        idx += sizeof(int);
        aom_codec_control(ctx, AV1E_SET_MATRIX_COEFFICIENTS, &coeffs);
    }

    return 1;
}

static int test_decoder_controls(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx || !ctx->iface) return 0;

    int idx = 0;

    // AV1D_SET_SKIP_FILM_GRAIN
    if (idx + sizeof(int) <= size) {
        int skip;
        memcpy(&skip, &data[idx], sizeof(int));
        idx += sizeof(int);
        aom_codec_control(ctx, AV1D_SET_SKIP_FILM_GRAIN, &skip);
    }

    return 1;
}

int LLVMFuzzerTestOneInput_281(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    int mode = Data[0] % 2;
    aom_codec_ctx_t ctx = {0};
    aom_codec_iface_t *iface = NULL;
    aom_codec_err_t ret;

    if (mode == 0) {
        // Encoder
        iface = aom_codec_av1_cx();
        if (!iface) return 0;

        aom_codec_enc_cfg_t cfg;
        ret = aom_codec_enc_config_default(iface, &cfg, 0);
        if (ret != AOM_CODEC_OK) return 0;

        cfg.g_w = 64;
        cfg.g_h = 64;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 30;
        cfg.g_threads = 1;
        cfg.g_usage = 0;
        cfg.rc_target_bitrate = 100;

        ret = aom_codec_enc_init(&ctx, iface, &cfg, 0);
        if (ret != AOM_CODEC_OK) {
            return 0;
        }

        aom_image_t img;
        aom_img_alloc(&img, AOM_IMG_FMT_I420, 64, 64, 1);
        aom_enc_frame_flags_t flags = 0;
        aom_codec_encode(&ctx, &img, 0, 1, flags);

        test_encoder_controls(&ctx, Data + 1, Size > 1 ? Size - 1 : 0);

        aom_img_free(&img);
    } else {
        // Decoder
        iface = aom_codec_av1_dx();
        if (!iface) return 0;

        aom_codec_dec_cfg_t cfg;
        memset(&cfg, 0, sizeof(cfg));
        cfg.threads = 1;
        // Removed invalid members: allow_high_bitdepth, allow_multiref, allow_screen_content_tools
        // Using only valid members available in standard aom_codec_dec_cfg_t

        ret = aom_codec_dec_init(&ctx, iface, &cfg, 0);
        if (ret != AOM_CODEC_OK) {
            return 0;
        }

        test_decoder_controls(&ctx, Data + 1, Size > 1 ? Size - 1 : 0);
    }

    aom_codec_destroy(&ctx);
    return 0;
}