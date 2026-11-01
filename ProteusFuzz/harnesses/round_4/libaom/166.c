// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aomdx.h"
#include "aom_encoder.h"
#include "aom_frame_buffer.h"
#include "aom_decoder.h"
#include "aom_image.h"
#include "aom.h"
#include "aom_integer.h"
#include "aom_codec.h"
#include "aomcx.h"
#include "aom_external_partition.h"

/*
 * Fuzz driver for libaom AV1 encoder control functions.
 * Targets:
 * - AV1E_SET_TILE_COLUMNS
 * - AV1E_SET_QM_V
 * - AV1E_SET_ENABLE_QM
 * - AV1E_SET_MV_COST_UPD_FREQ
 * - AV1_SET_TILE_MODE
 * - AV1E_SET_REDUCED_TX_TYPE_SET
 */

static int init_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg) {
    aom_codec_iface_t *iface;
    aom_codec_err_t ret;

    iface = aom_codec_av1_cx();
    if (!iface) return -1;

    ret = aom_codec_enc_config_default(iface, cfg, 0);
    if (ret != AOM_CODEC_OK) return -1;

    /* Set minimal config to allow initialization */
    cfg->g_w = 64;
    cfg->g_h = 64;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_threads = 1;
    /* Removed gc_arnr_max_frames as it may not exist in all versions */

    ret = aom_codec_enc_init(ctx, iface, cfg, 0);
    if (ret != AOM_CODEC_OK) return -1;

    return 0;
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    aom_codec_destroy(ctx);
}

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_166(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    if (init_encoder(&ctx, &cfg) != 0) {
        return 0;
    }

    /* Use input data to drive control parameters */
    uint32_t val = 0;
    if (Size >= 4) {
        val = Data[0] | (Data[1] << 8) | (Data[2] << 16) | (Data[3] << 24);
    }

    /* Limit values to reasonable ranges for AV1 controls */
    uint8_t tile_cols = (val % 16) + 1; /* 1 to 16 */
    int enable_qm = (val & 0x10000000) ? 1 : 0;
    int mv_cost_upd_freq = (val % 100) + 1; /* 1 to 100 */
    /* Use standard integer values for tile mode instead of undefined macros */
    int tile_mode = (val & 0x1) ? 1 : 0; 
    int reduced_tx_set = (val & 0x2) ? 1 : 0;

    /* AV1E_SET_TILE_COLUMNS */
    aom_codec_control(&ctx, AV1E_SET_TILE_COLUMNS, tile_cols);

    /* AV1E_SET_ENABLE_QM */
    aom_codec_control(&ctx, AV1E_SET_ENABLE_QM, enable_qm);

    /* AV1E_SET_MV_COST_UPD_FREQ */
    aom_codec_control(&ctx, AV1E_SET_MV_COST_UPD_FREQ, mv_cost_upd_freq);

    /* AV1E_SET_REDUCED_TX_TYPE_SET */
    aom_codec_control(&ctx, AV1E_SET_REDUCED_TX_TYPE_SET, reduced_tx_set);

    /* AV1_SET_TILE_MODE */
    aom_codec_control(&ctx, AV1_SET_TILE_MODE, tile_mode);

    /* AV1E_SET_QM_V: This control expects a pointer to quantization matrix data.
     * We simulate this by allocating a small buffer and using input data.
     */
    if (Size > 0) {
        uint8_t *qm_v = malloc(64); /* Safe small allocation */
        if (qm_v) {
            size_t copy_size = (Size < 64) ? Size : 64;
            memcpy(qm_v, Data, copy_size);
            /* Pad rest with zeros */
            if (copy_size < 64) {
                memset(qm_v + copy_size, 0, 64 - copy_size);
            }
            aom_codec_control(&ctx, AV1E_SET_QM_V, qm_v);
            free(qm_v);
        }
    }

    /* Write input to dummy file as per requirement */
    write_dummy_file(Data, Size);

    cleanup_encoder(&ctx);
    return 0;
}