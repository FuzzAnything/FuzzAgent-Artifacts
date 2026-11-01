// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_image.h>
#include <aom/aom_integer.h>

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, int control_id, void *data) {
    if (!ctx || !ctx->iface) return 0;
    
    aom_codec_err_t ret = aom_codec_control(ctx, control_id, data);
    (void)ret; // Suppress unused variable warning
    return 1;
}

static int fuzz_decoder_control(aom_codec_ctx_t *ctx, int control_id, void *data) {
    if (!ctx || !ctx->iface) return 0;
    
    aom_codec_err_t ret = aom_codec_control(ctx, control_id, data);
    (void)ret;
    return 1;
}

int LLVMFuzzerTestOneInput_264(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    // Initialize random seed from input for deterministic behavior
    unsigned int seed = 0;
    memcpy(&seed, Data, sizeof(seed));
    srand(seed);

    // Create encoder context
    aom_codec_ctx_t enc_ctx;
    memset(&enc_ctx, 0, sizeof(enc_ctx));
    aom_codec_iface_t *enc_iface = aom_codec_av1_cx();
    if (!enc_iface) return 0;

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(enc_iface, &cfg, 0);
    cfg.g_w = 128;
    cfg.g_h = 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;

    if (aom_codec_enc_init(&enc_ctx, enc_iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Create decoder context
    aom_codec_ctx_t dec_ctx;
    memset(&dec_ctx, 0, sizeof(dec_ctx));
    aom_codec_iface_t *dec_iface = aom_codec_av1_dx();
    if (!dec_iface) {
        aom_codec_destroy(&enc_ctx);
        return 0;
    }

    if (aom_codec_dec_init(&dec_ctx, dec_iface, NULL, 0) != AOM_CODEC_OK) {
        aom_codec_destroy(&enc_ctx);
        return 0;
    }

    // Prepare test data
    int enable_tx_size_search = (Size > 4) ? (Data[4] % 2) : 0;
    int delta_lf_mode = (Size > 5) ? (Data[5] % 3) : 0;
    int enable_interintra_comp = (Size > 6) ? (Data[6] % 2) : 0;
    int cdf_update_mode = (Size > 7) ? (Data[7] % 3) : 0;

    // Fuzz AV1E_SET_ENABLE_TX_SIZE_SEARCH
    fuzz_encoder_control(&enc_ctx, AV1E_SET_ENABLE_TX_SIZE_SEARCH, &enable_tx_size_search);

    // Fuzz AV1E_SET_DELTALF_MODE
    fuzz_encoder_control(&enc_ctx, AV1E_SET_DELTALF_MODE, &delta_lf_mode);

    // Fuzz AV1E_SET_ENABLE_INTERINTRA_COMP
    fuzz_encoder_control(&enc_ctx, AV1E_SET_ENABLE_INTERINTRA_COMP, &enable_interintra_comp);

    // Fuzz AV1E_SET_CDF_UPDATE_MODE
    fuzz_encoder_control(&enc_ctx, AV1E_SET_CDF_UPDATE_MODE, &cdf_update_mode);

    // Prepare active map for AOME_SET_ACTIVEMAP and AV1E_GET_ACTIVEMAP
    aom_active_map_t active_map;
    active_map.rows = (Size > 8) ? (Data[8] % 16) + 1 : 1;
    active_map.cols = (Size > 9) ? (Data[9] % 16) + 1 : 1;
    active_map.active_map = malloc(active_map.rows * active_map.cols);
    if (!active_map.active_map) {
        aom_codec_destroy(&enc_ctx);
        aom_codec_destroy(&dec_ctx);
        return 0;
    }
    memset(active_map.active_map, 0, active_map.rows * active_map.cols);
    for (unsigned int i = 0; i < active_map.rows * active_map.cols; i++) {
        active_map.active_map[i] = (Size > 10 + i) ? (Data[10 + i] % 2) : 0;
    }

    // Fuzz AOME_SET_ACTIVEMAP
    fuzz_encoder_control(&enc_ctx, AOME_SET_ACTIVEMAP, &active_map);

    // Fuzz AV1E_GET_ACTIVEMAP
    aom_active_map_t get_active_map;
    memset(&get_active_map, 0, sizeof(get_active_map));
    fuzz_encoder_control(&enc_ctx, AV1E_GET_ACTIVEMAP, &get_active_map);

    // Cleanup
    free(active_map.active_map);
    aom_codec_destroy(&enc_ctx);
    aom_codec_destroy(&dec_ctx);

    return 0;
}