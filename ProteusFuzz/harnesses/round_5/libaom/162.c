// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include "aom/aomcx.h"
#include "aom/aomdx.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_decoder.h"
#include "aom/aom_image.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_encoder.h"
#include "aom/aom_integer.h"

/* 
 * Target API Functions (signatures inferred from libaom standard control API):
 * 1. aom_codec_control(ctx, AV1E_SET_DISABLE_TRELLIS_QUANT, val)
 * 2. aom_codec_control(ctx, AOME_SET_ARNR_MAXFRAMES, val)
 * 3. aom_codec_control(ctx, AV1E_SET_MODE_COST_UPD_FREQ, val)
 * 4. aom_codec_control(ctx, AV1E_SET_AQ_MODE, val)
 * 5. aom_codec_control(ctx, AOME_SET_ARNR_STRENGTH, val)
 * 6. aom_codec_control(ctx, AV1E_SET_DELTAQ_STRENGTH, val)
 */

static int init_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg, 
                        const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    /* Use fuzz input to set width/height if possible, but clamp to safe limits */
    if (size >= 8) {
        uint32_t w = *(const uint32_t *)data;
        uint32_t h = *(const uint32_t *)(data + 4);
        /* Clamp to reasonable limits to avoid OOM or invalid config */
        if (w > 4096) w = 4096;
        if (h > 4096) h = 4096;
        if (w == 0) w = 64;
        if (h == 0) h = 64;
        cfg->g_w = w;
        cfg->g_h = h;
    } else {
        cfg->g_w = 64;
        cfg->g_h = 64;
    }
    
    cfg->g_threads = 1;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_lag_in_frames = 0;
    /* Removed invalid members gc_arnr_max_frames and gc_arnr_strength 
       as they are not part of aom_codec_enc_cfg in this version. 
       They are set via aom_codec_control later. */

    if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    return 1;
}

int LLVMFuzzerTestOneInput_162(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    if (!init_encoder(&ctx, &cfg, Data, Size)) {
        return 0;
    }

    /* Extract control values from input data */
    int val1 = 0, val2 = 0, val3 = 0, val4 = 0, val5 = 0, val6 = 0;
    
    if (Size >= 1) val1 = Data[0];
    if (Size >= 2) val2 = Data[1];
    if (Size >= 3) val3 = Data[2];
    if (Size >= 4) val4 = Data[3];
    if (Size >= 5) val5 = Data[4];
    if (Size >= 6) val6 = Data[5];

    /* 
     * Invoke target API functions via aom_codec_control.
     * These correspond to the requested "typechecked" operations.
     */

    /* 1. AV1E_SET_DISABLE_TRELLIS_QUANT */
    aom_codec_control(&ctx, AV1E_SET_DISABLE_TRELLIS_QUANT, val1);

    /* 2. AOME_SET_ARNR_MAXFRAMES */
    aom_codec_control(&ctx, AOME_SET_ARNR_MAXFRAMES, val2);

    /* 3. AV1E_SET_MODE_COST_UPD_FREQ */
    aom_codec_control(&ctx, AV1E_SET_MODE_COST_UPD_FREQ, val3);

    /* 4. AV1E_SET_AQ_MODE */
    aom_codec_control(&ctx, AV1E_SET_AQ_MODE, val4);

    /* 5. AOME_SET_ARNR_STRENGTH */
    aom_codec_control(&ctx, AOME_SET_ARNR_STRENGTH, val5);

    /* 6. AV1E_SET_DELTAQ_STRENGTH */
    aom_codec_control(&ctx, AV1E_SET_DELTAQ_STRENGTH, val6);

    /* Cleanup */
    aom_codec_destroy(&ctx);

    return 0;
}