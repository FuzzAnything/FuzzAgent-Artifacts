// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aomdx.h>
#include <aom/aom_encoder.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aom_integer.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aom_external_partition.h>

static int fuzz_codec_control(aom_codec_ctx_t *ctx, int id, void *arg) {
    if (!ctx) return -1;
    return aom_codec_control(ctx, id, arg);
}

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_80(const uint8_t *Data, size_t Size) {
    if (Size < 6) return 0;

    aom_codec_ctx_t ctx = {0};
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) return 0;

    // Use input data to configure encoder parameters
    cfg.g_w = (Data[0] % 1024) + 16;
    cfg.g_h = (Data[1] % 1024) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.g_profile = Data[2] % 4;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz various codec control functions
    uint8_t val = Data[3];
    int ret;

    // 1. AV1E_SET_ENABLE_1TO4_PARTITIONS
    ret = fuzz_codec_control(&ctx, AV1E_SET_ENABLE_1TO4_PARTITIONS, &val);
    (void)ret;

    // 2. AV1E_SET_REDUCED_REFERENCE_SET
    ret = fuzz_codec_control(&ctx, AV1E_SET_REDUCED_REFERENCE_SET, &val);
    (void)ret;

    // 3. AV1E_SET_ENABLE_RECT_PARTITIONS
    ret = fuzz_codec_control(&ctx, AV1E_SET_ENABLE_RECT_PARTITIONS, &val);
    (void)ret;

    // 4. AV1E_SET_TILE_ROWS
    int tile_rows = (Data[4] % 8) + 1;
    ret = fuzz_codec_control(&ctx, AV1E_SET_TILE_ROWS, &tile_rows);
    (void)ret;

    // 5. AV1E_SET_CHROMA_SAMPLE_POSITION
    int chroma_pos = Data[5] % 4;
    ret = fuzz_codec_control(&ctx, AV1E_SET_CHROMA_SAMPLE_POSITION, &chroma_pos);
    (void)ret;

    // 6. AV1E_SET_ENABLE_CFL_INTRA
    ret = fuzz_codec_control(&ctx, AV1E_SET_ENABLE_CFL_INTRA, &val);
    (void)ret;

    // Cleanup
    cleanup_codec(&ctx);

    return 0;
}