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

static int init_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg) {
    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) return 0;

    cfg->g_w = 64;
    cfg->g_h = 64;
    cfg->g_bit_depth = 8;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_profile = 0;

    if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) return 0;

    return 1;
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx && ctx->iface) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_147(const uint8_t *Data, size_t Size) {
    if (Size < 6) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    if (!init_encoder(&ctx, &cfg)) {
        return 0;
    }

    // Extract values from input data to control parameters
    int val1 = (Data[0] % 2); // 0 or 1
    int val2 = (Data[1] % 16) + 1; // 1 to 16
    int val3 = (Data[2] % 2); // 0 or 1
    int val4 = (Data[3] % 2); // 0 or 1
    int val5 = (Data[4] % 8) + 1; // 1 to 8
    int val6 = (Data[5] % 3); // 0, 1, or 2

    // Fuzz AV1E_SET_ENABLE_1TO4_PARTITIONS
    aom_codec_control(&ctx, AV1E_SET_ENABLE_1TO4_PARTITIONS, val1);

    // Fuzz AV1E_SET_MIN_PARTITION_SIZE
    aom_codec_control(&ctx, AV1E_SET_MIN_PARTITION_SIZE, val2);

    // Fuzz AV1E_SET_REDUCED_REFERENCE_SET
    aom_codec_control(&ctx, AV1E_SET_REDUCED_REFERENCE_SET, val3);

    // Fuzz AV1E_SET_ENABLE_RECT_PARTITIONS
    aom_codec_control(&ctx, AV1E_SET_ENABLE_RECT_PARTITIONS, val4);

    // Fuzz AV1E_SET_TILE_ROWS
    aom_codec_control(&ctx, AV1E_SET_TILE_ROWS, val5);

    // Fuzz AV1E_SET_CHROMA_SAMPLE_POSITION
    aom_codec_control(&ctx, AV1E_SET_CHROMA_SAMPLE_POSITION, val6);

    // Cleanup
    cleanup_encoder(&ctx);

    return 0;
}