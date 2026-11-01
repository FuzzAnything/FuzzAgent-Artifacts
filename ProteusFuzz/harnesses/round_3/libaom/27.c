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
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_image.h>
#include <aom/aom_external_partition.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_27(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use input data to configure some parameters if possible
    if (Size > 0) {
        cfg.g_w = (Data[0] % 1024) + 16;
        cfg.g_h = ((Size > 1) ? (Data[1] % 1024) + 16 : 16);
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = (Size > 2) ? (Data[2] % 255) + 1 : 1;
        cfg.rc_target_bitrate = (Size > 3) ? (Data[3] % 1000) + 100 : 100;
    }

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz the target API functions
    // AV1E_SET_ENABLE_TX_SIZE_SEARCH
    int enable_tx_size_search = (Size > 4) ? (Data[4] % 2) : 0;
    aom_codec_control(&ctx, AV1E_SET_ENABLE_TX_SIZE_SEARCH, enable_tx_size_search);

    // AV1E_SET_INTRA_DCT_ONLY
    int intra_dct_only = (Size > 5) ? (Data[5] % 2) : 0;
    aom_codec_control(&ctx, AV1E_SET_INTRA_DCT_ONLY, intra_dct_only);

    // AV1E_SET_ENABLE_GLOBAL_MOTION
    int enable_global_motion = (Size > 6) ? (Data[6] % 2) : 0;
    aom_codec_control(&ctx, AV1E_SET_ENABLE_GLOBAL_MOTION, enable_global_motion);

    // AV1E_SET_ENABLE_INTERINTRA_COMP
    int enable_interintra_comp = (Size > 7) ? (Data[7] % 2) : 0;
    aom_codec_control(&ctx, AV1E_SET_ENABLE_INTERINTRA_COMP, enable_interintra_comp);

    // AV1E_SET_ENABLE_RECT_PARTITIONS
    int enable_rect_partitions = (Size > 8) ? (Data[8] % 2) : 0;
    aom_codec_control(&ctx, AV1E_SET_ENABLE_RECT_PARTITIONS, enable_rect_partitions);

    // AV1E_SET_ENABLE_PALETTE
    int enable_palette = (Size > 9) ? (Data[9] % 2) : 0;
    aom_codec_control(&ctx, AV1E_SET_ENABLE_PALETTE, enable_palette);

    // Cleanup
    cleanup_codec(&ctx);

    return 0;
}