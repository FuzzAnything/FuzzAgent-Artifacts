// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
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
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
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

static int fuzz_encoder_controls(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t res;
    int ret = 0;

    if (Size < 2) return 0;

    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use fuzzing data to configure encoder settings
    cfg.g_w = (Data[0] % 1024) + 16;
    cfg.g_h = (Data[1] % 1024) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = (Data[2] % 60) + 1;
    cfg.rc_target_bitrate = (Data[3] % 10000) + 100;

    res = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz AV1E_SET_TILE_COLUMNS
    int tile_cols = (Data[4] % 64) + 1;
    res = aom_codec_control(&ctx, AV1E_SET_TILE_COLUMNS, &tile_cols);
    if (res != AOM_CODEC_OK) {
        // Ignore errors, just continue
    }

    // Fuzz AV1E_SET_SUPERBLOCK_SIZE
    int sb_size = (Data[5] % 3); // 0, 1, or 2
    res = aom_codec_control(&ctx, AV1E_SET_SUPERBLOCK_SIZE, &sb_size);
    if (res != AOM_CODEC_OK) {
        // Ignore errors
    }

    // Fuzz AV1E_SET_ENABLE_QM
    int enable_qm = (Data[6] % 2);
    res = aom_codec_control(&ctx, AV1E_SET_ENABLE_QM, &enable_qm);
    if (res != AOM_CODEC_OK) {
        // Ignore errors
    }

    // Fuzz AV1E_SET_QM_V
    if (Size > 7) {
        int qm_v = (Data[7] % 256);
        res = aom_codec_control(&ctx, AV1E_SET_QM_V, &qm_v);
        if (res != AOM_CODEC_OK) {
            // Ignore errors
        }
    }

    // Fuzz AV1E_SET_QM_U
    if (Size > 8) {
        int qm_u = (Data[8] % 256);
        res = aom_codec_control(&ctx, AV1E_SET_QM_U, &qm_u);
        if (res != AOM_CODEC_OK) {
            // Ignore errors
        }
    }

    // Fuzz AV1_SET_TILE_MODE (if applicable)
    if (Size > 9) {
        int tile_mode = (Data[9] % 4);
        res = aom_codec_control(&ctx, AV1_SET_TILE_MODE, &tile_mode);
        if (res != AOM_CODEC_OK) {
            // Ignore errors
        }
    }

    aom_codec_destroy(&ctx);
    return ret;
}

int LLVMFuzzerTestOneInput_192(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    // Call the fuzzing function for encoder controls
    fuzz_encoder_controls(Data, Size);

    return 0;
}