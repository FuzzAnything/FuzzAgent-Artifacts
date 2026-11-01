// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
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
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aomdx.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aom_integer.h>
#include <aom/aomcx.h>
#include <aom/aom_external_partition.h>

static int initialize_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg, const uint8_t *data, size_t size) {
    if (ctx == NULL || cfg == NULL) {
        return 0;
    }

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (iface == NULL) {
        return 0;
    }

    if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    if (size >= 2) {
        cfg->g_w = (data[0] % 128) + 16;
        cfg->g_h = (data[1] % 128) + 16;
    } else {
        cfg->g_w = 16;
        cfg->g_h = 16;
    }

    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_threads = 1;
    cfg->rc_overshoot_pct = 0;

    if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    return 1;
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx != NULL) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_55(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    aom_codec_ctx_t ctx = {0};
    aom_codec_enc_cfg_t cfg = {0};

    if (!initialize_encoder(&ctx, &cfg, Data, Size)) {
        return 0;
    }

    // Fuzz AV1E_SET_DENOISE_NOISE_LEVEL
    if (Size >= 1) {
        int noise_level = Data[0];
        aom_codec_control(&ctx, AV1E_SET_DENOISE_NOISE_LEVEL, &noise_level);
    }

    // Fuzz AV1E_SET_DENOISE_BLOCK_SIZE
    if (Size >= 2) {
        int block_size = Data[1];
        aom_codec_control(&ctx, AV1E_SET_DENOISE_BLOCK_SIZE, &block_size);
    }

    // Fuzz AV1E_SET_ENABLE_RESTORATION
    if (Size >= 3) {
        int enable_restoration = Data[2] % 2;
        aom_codec_control(&ctx, AV1E_SET_ENABLE_RESTORATION, &enable_restoration);
    }

    // Fuzz AV1E_SET_ENABLE_CDEF
    if (Size >= 4) {
        int enable_cdef = Data[3] % 2;
        aom_codec_control(&ctx, AV1E_SET_ENABLE_CDEF, &enable_cdef);
    }

    // Fuzz AV1E_SET_NOISE_SENSITIVITY
    if (Size >= 5) {
        int noise_sensitivity = Data[4];
        aom_codec_control(&ctx, AV1E_SET_NOISE_SENSITIVITY, &noise_sensitivity);
    }

    // Fuzz AV1E_SET_SKIP_POSTPROC_FILTERING
    if (Size >= 6) {
        int skip_postproc = Data[5] % 2;
        aom_codec_control(&ctx, AV1E_SET_SKIP_POSTPROC_FILTERING, &skip_postproc);
    }

    cleanup_encoder(&ctx);
    return 0;
}