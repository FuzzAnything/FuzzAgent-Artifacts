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
    if (ctx && ctx->iface) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_243(const uint8_t *Data, size_t Size) {
    if (Size < 10) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t res;

    // Get the AV1 encoder interface
    iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    // Initialize encoder configuration
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use fuzzer data to configure some parameters safely
    cfg.g_w = (Data[0] % 32) + 16; // Width between 16 and 47
    cfg.g_h = (Data[1] % 32) + 16; // Height between 16 and 47
    cfg.g_timebase.num = Data[2] % 30 + 1;
    cfg.g_timebase.den = Data[3] % 30 + 1;
    cfg.g_threads = 1;
    cfg.rc_target_bitrate = 1000;

    // Initialize codec context
    res = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        cleanup_codec(&ctx);
        return 0;
    }

    // Fuzz AV1E_SET_ENABLE_DIRECTIONAL_INTRA
    int enable_dir = Data[4] % 2;
    aom_codec_control(&ctx, AV1E_SET_ENABLE_DIRECTIONAL_INTRA, enable_dir);

    // Fuzz AV1E_GET_SEQ_LEVEL_IDX
    int seq_level_idx;
    aom_codec_control(&ctx, AV1E_GET_SEQ_LEVEL_IDX, &seq_level_idx);

    // Fuzz AV1E_SET_MIN_PARTITION_SIZE
    int min_part_size = Data[5] % 4 + 1; // 1 to 4
    aom_codec_control(&ctx, AV1E_SET_MIN_PARTITION_SIZE, min_part_size);

    // Fuzz AV1E_SET_ENABLE_PAETH_INTRA
    int enable_paeth = Data[6] % 2;
    aom_codec_control(&ctx, AV1E_SET_ENABLE_PAETH_INTRA, enable_paeth);

    // Fuzz AV1E_SET_ENABLE_DIAGONAL_INTRA
    int enable_diag = Data[7] % 2;
    aom_codec_control(&ctx, AV1E_SET_ENABLE_DIAGONAL_INTRA, enable_diag);

    // Fuzz AV1E_SET_RENDER_SIZE
    int render_w = (Data[8] % 32) + 16;
    int render_h = (Data[9] % 32) + 16;
    aom_codec_control(&ctx, AV1E_SET_RENDER_SIZE, &render_w);
    aom_codec_control(&ctx, AV1E_SET_RENDER_SIZE, &render_h);

    // Cleanup
    cleanup_codec(&ctx);

    return 0;
}