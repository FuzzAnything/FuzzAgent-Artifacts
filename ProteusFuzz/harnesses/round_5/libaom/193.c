// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aomcx.h>

static int fuzzer_init_encoder(aom_codec_ctx_t *ctx, const aom_codec_iface_t *iface,
                               const uint8_t *data, size_t size) {
    aom_codec_enc_cfg_t cfg;
    int width = 0;
    int height = 0;

    if (size >= 4) {
        width = data[0] + 1;
        height = data[1] + 1;
    } else if (size == 2) {
        width = data[0] + 1;
        height = data[1] + 1;
    } else {
        width = 64;
        height = 64;
    }

    if (width < 16) width = 16;
    if (height < 16) height = 16;
    if (width > 4096) width = 4096;
    if (height > 4096) height = 4096;

    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.g_lag_in_frames = 0;
    cfg.rc_target_bitrate = 1000;

    if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    return 1;
}

int LLVMFuzzerTestOneInput_193(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (iface == NULL) {
        return 0;
    }

    if (!fuzzer_init_encoder(&ctx, iface, Data, Size)) {
        return 0;
    }

    int enable_intrabc = (Size > 0) ? (Data[0] % 2) : 0;
    int enable_restoration = (Size > 1) ? (Data[1] % 2) : 0;
    int skip_postproc = (Size > 2) ? (Data[2] % 2) : 0;
    int enable_cdef = (Size > 3) ? (Data[3] % 2) : 0;
    int denoise_noise_level = (Size > 4) ? (Data[4] % 64) : 0;
    int denoise_block_size = (Size > 5) ? (Data[5] % 64) : 0;

    aom_codec_control(&ctx, AV1E_SET_ENABLE_INTRABC, enable_intrabc);
    aom_codec_control(&ctx, AV1E_SET_ENABLE_RESTORATION, enable_restoration);
    aom_codec_control(&ctx, AV1E_SET_SKIP_POSTPROC_FILTERING, skip_postproc);
    aom_codec_control(&ctx, AV1E_SET_ENABLE_CDEF, enable_cdef);
    aom_codec_control(&ctx, AV1E_SET_DENOISE_NOISE_LEVEL, denoise_noise_level);
    aom_codec_control(&ctx, AV1E_SET_DENOISE_BLOCK_SIZE, denoise_block_size);

    aom_codec_destroy(&ctx);

    return 0;
}