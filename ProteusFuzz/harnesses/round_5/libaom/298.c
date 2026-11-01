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
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_integer.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_298(const uint8_t *Data, size_t Size) {
    if (Size < 16) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 500;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    uint32_t val1, val2, val3, val4;
    
    // Safely extract 4 uint32_t values from the input buffer
    // We need at least 16 bytes (4 * 4) to safely read 4 integers
    if (Size >= 16) {
        memcpy(&val1, Data, sizeof(uint32_t));
        memcpy(&val2, Data + 4, sizeof(uint32_t));
        memcpy(&val3, Data + 8, sizeof(uint32_t));
        memcpy(&val4, Data + 12, sizeof(uint32_t));
    } else {
        // Fallback if size is between 4 and 16, though the check above handles < 16
        val1 = 0; val2 = 0; val3 = 0; val4 = 0;
    }

    // Fuzz AV1E_SET_ENABLE_INTRABC
    aom_codec_control(&ctx, AV1E_SET_ENABLE_INTRABC, val1);

    // Fuzz AV1E_SET_TILE_ROWS
    aom_codec_control(&ctx, AV1E_SET_TILE_ROWS, val2);

    // Fuzz AV1E_SET_MAX_GF_INTERVAL
    aom_codec_control(&ctx, AV1E_SET_MAX_GF_INTERVAL, val3);

    // Fuzz AV1E_SET_DELTAQ_MODE
    aom_codec_control(&ctx, AV1E_SET_DELTAQ_MODE, val4);

    // Fuzz AV1E_SET_SUPERBLOCK_SIZE
    aom_codec_control(&ctx, AV1E_SET_SUPERBLOCK_SIZE, val1);

    // Fuzz AV1E_SET_TIMING_INFO_TYPE
    aom_codec_control(&ctx, AV1E_SET_TIMING_INFO_TYPE, val2);

    cleanup_codec(&ctx);
    return 0;
}