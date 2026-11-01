// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
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
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>

static int init_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg, const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface;
    aom_codec_err_t res;

    iface = aom_codec_av1_cx();
    if (!iface) return -1;

    if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) return -1;

    // Fuzz configuration parameters based on input data if size allows
    if (size > 0) cfg->g_w = (data[0] % 1024) + 16;
    if (size > 1) cfg->g_h = (data[1] % 1024) + 16;
    if (size > 2) cfg->g_timebase.num = (data[2] % 255) + 1;
    if (size > 3) cfg->g_timebase.den = (data[3] % 255) + 1;
    if (size > 4) cfg->g_threads = (data[4] % 8) + 1;
    if (size > 5) cfg->g_lag_in_frames = (data[5] % 256);
    if (size > 6) cfg->rc_target_bitrate = (data[6] % 10000) + 100;

    res = aom_codec_enc_init(ctx, iface, cfg, 0);
    if (res != AOM_CODEC_OK) return -1;

    return 0;
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    aom_codec_destroy(ctx);
}

int LLVMFuzzerTestOneInput_324(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    int ret;

    if (Size < 2) return 0; // Need at least width and height

    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    if (init_encoder(&ctx, &cfg, Data, Size) != 0) {
        return 0;
    }

    // Fuzz AV1E_SET_ENABLE_TX_SIZE_SEARCH
    // Sets a boolean flag (0 or 1)
    int enable_tx_size_search = (Size > 0) ? (Data[0] & 1) : 0;
    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_TX_SIZE_SEARCH, enable_tx_size_search);
    (void)ret; // Ignore return for fuzzing stability, just trigger execution

    // Fuzz AV1E_SET_ENABLE_INTRABC
    // Sets a boolean flag (0 or 1)
    int enable_intrabc = (Size > 1) ? (Data[1] & 1) : 0;
    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_INTRABC, enable_intrabc);
    (void)ret;

    // Fuzz AV1E_GET_ACTIVEMAP
    // Retrieves an active map structure
    aom_active_map_t active_map;
    memset(&active_map, 0, sizeof(active_map));
    ret = aom_codec_control(&ctx, AV1E_GET_ACTIVEMAP, &active_map);
    (void)ret;

    // Fuzz AV1E_SET_INTRA_DEFAULT_TX_ONLY
    // Sets a boolean flag (0 or 1)
    int intra_default_tx_only = (Size > 2) ? (Data[2] & 1) : 0;
    ret = aom_codec_control(&ctx, AV1E_SET_INTRA_DEFAULT_TX_ONLY, intra_default_tx_only);
    (void)ret;

    // Fuzz AV1E_SET_MODE_COST_UPD_FREQ
    // Sets an integer frequency
    int mode_cost_upd_freq = (Size > 3) ? (Data[3] % 100) : 0;
    ret = aom_codec_control(&ctx, AV1E_SET_MODE_COST_UPD_FREQ, mode_cost_upd_freq);
    (void)ret;

    // Fuzz AOME_SET_ACTIVEMAP
    // Sets an active map structure. We construct a minimal valid one for fuzzing.
    aom_active_map_t set_active_map;
    memset(&set_active_map, 0, sizeof(set_active_map));
    if (Size > 4) {
        set_active_map.rows = (Data[4] % 64) + 1;
        set_active_map.cols = (Size > 5) ? (Data[5] % 64) + 1 : 1;
        size_t map_size = set_active_map.rows * set_active_map.cols;
        set_active_map.active_map = (unsigned char *)malloc(map_size);
        if (set_active_map.active_map) {
            // Fill with fuzz data
            size_t data_avail = (Size > 6) ? (Size - 6) : 0;
            size_t copy_len = (data_avail < map_size) ? data_avail : map_size;
            memcpy(set_active_map.active_map, (Size > 6) ? Data + 6 : Data, copy_len);
            // Zero out the rest if needed
            if (copy_len < map_size) {
                memset(set_active_map.active_map + copy_len, 0, map_size - copy_len);
            }
            ret = aom_codec_control(&ctx, AOME_SET_ACTIVEMAP, &set_active_map);
            (void)ret;
            free(set_active_map.active_map);
        }
    }

    cleanup_encoder(&ctx);
    return 0;
}