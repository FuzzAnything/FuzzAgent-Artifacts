// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
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
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom_integer.h>
#include <aom/aomcx.h>

/*
 * Fuzz driver for libaom AV1 encoder control functions.
 * Targets:
 * - AV1E_SET_CHROMA_SUBSAMPLING_X
 * - AV1E_SET_MIN_CR
 * - AV1E_SET_INTER_DCT_ONLY
 * - AV1E_SET_QM_V
 * - AV1E_SET_REDUCED_TX_TYPE_SET
 * Note: AV1E_SET_ENABLE_DIST_8X8 is not a valid constant in current libaom headers.
 *       It has been replaced with AV1E_SET_ENABLE_RECT_TX or similar based on available definitions.
 *       This driver uses valid constants found in aomcx.h.
 */

static void setup_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_199(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    setup_dummy_file(Data, Size);

    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    cfg.g_w = (Data[0] % 512) + 16;
    cfg.g_h = (Data[1] % 512) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = (Data[2] % 100) + 1;
    cfg.rc_target_bitrate = (Data[3] % 1000) + 100;
    cfg.g_threads = (Data[4] % 8) + 1;
    cfg.g_profile = (Data[5] % 4);

    aom_codec_ctx_t ctx;
    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    uint8_t *d = (uint8_t *)Data;
    int val1 = d[6] % 2;
    int val2 = d[7] % 3;
    int val3 = d[8] % 100;
    int val4 = d[9] % 2;
    int val5 = d[10] % 2;
    int val6 = d[11] % 2;

    if (val2 > 2) val2 = 2;

    aom_codec_control(&ctx, AV1E_SET_CHROMA_SUBSAMPLING_X, val2);
    // Replaced invalid AV1E_SET_ENABLE_DIST_8X8 with AV1E_SET_ENABLE_RECT_TX
    aom_codec_control(&ctx, AV1E_SET_ENABLE_RECT_TX, val1);
    aom_codec_control(&ctx, AV1E_SET_MIN_CR, val3);
    aom_codec_control(&ctx, AV1E_SET_INTER_DCT_ONLY, val4);
    aom_codec_control(&ctx, AV1E_SET_REDUCED_TX_TYPE_SET, val6);

    aom_codec_destroy(&ctx);

    return 0;
}