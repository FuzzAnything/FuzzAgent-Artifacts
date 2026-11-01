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
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aomcx.h>
#include <aom/aom_external_partition.h>

static int setup_codec_context(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use fuzzing data to vary configuration parameters slightly
    if (size > 0) {
        cfg.g_w = (data[0] % 1024) + 16;
        cfg.g_h = (data[1] % 1024) + 16;
        cfg.g_timebase.num = (data[2] % 100) + 1;
        cfg.g_timebase.den = (data[3] % 100) + 1;
        cfg.rc_target_bitrate = (data[4] % 10000) + 100;
        cfg.g_threads = (data[5] % 8) + 1;
    } else {
        cfg.g_w = 128;
        cfg.g_h = 128;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 30;
        cfg.rc_target_bitrate = 1000;
        cfg.g_threads = 1;
    }

    if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    return 1;
}

static void cleanup_codec_context(aom_codec_ctx_t *ctx) {
    if (ctx && ctx->priv) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_245(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    if (!setup_codec_context(&ctx, Data, Size)) {
        return 0;
    }

    // Fuzz AV1E_SET_DENOISE_BLOCK_SIZE
    if (Size > 0) {
        int denoise_block_size = Data[0] % 16; // Valid range typically 0-15
        aom_codec_control(&ctx, AV1E_SET_DENOISE_BLOCK_SIZE, denoise_block_size);
    }

    // Fuzz AV1E_SET_DENOISE_NOISE_LEVEL
    if (Size > 1) {
        int noise_level = Data[1] % 100; // Valid range typically 0-99
        aom_codec_control(&ctx, AV1E_SET_DENOISE_NOISE_LEVEL, noise_level);
    }

    // Fuzz AV1E_SET_ENABLE_CFL_INTRA
    if (Size > 2) {
        int enable_cfl_intra = (Data[2] % 2) ? 1 : 0;
        aom_codec_control(&ctx, AV1E_SET_ENABLE_CFL_INTRA, enable_cfl_intra);
    }

    // Fuzz AV1E_SET_ENABLE_INTRABC
    if (Size > 3) {
        int enable_intrabc = (Data[3] % 2) ? 1 : 0;
        aom_codec_control(&ctx, AV1E_SET_ENABLE_INTRABC, enable_intrabc);
    }

    // Fuzz AV1E_SET_SKIP_POSTPROC_FILTERING
    if (Size > 4) {
        int skip_postproc = (Data[4] % 2) ? 1 : 0;
        aom_codec_control(&ctx, AV1E_SET_SKIP_POSTPROC_FILTERING, skip_postproc);
    }

    // Fuzz AV1E_SET_ENABLE_RESTORATION
    if (Size > 5) {
        int enable_restoration = (Data[5] % 2) ? 1 : 0;
        aom_codec_control(&ctx, AV1E_SET_ENABLE_RESTORATION, enable_restoration);
    }

    // Cleanup
    cleanup_codec_context(&ctx);

    return 0;
}