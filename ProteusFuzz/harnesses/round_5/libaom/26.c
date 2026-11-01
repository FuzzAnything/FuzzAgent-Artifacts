// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control_typechecked_AV1E_SET_ENABLE_INTRABC at aomcx.h:2156:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_ENABLE_TX_SIZE_SEARCH at aomcx.h:2285:1 in aomcx.h
// aom_codec_control_typechecked_AOME_SET_ACTIVEMAP at aomcx.h:1881:1 in aomcx.h
// aom_codec_control_typechecked_AOME_SET_ROI_MAP at aomcx.h:1878:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_ENABLE_INTERINTRA_COMP at aomcx.h:2111:1 in aomcx.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "aom.h"
#include "aom_codec.h"
#include "aom_encoder.h"
#include "aomcx.h"
#include "aomdx.h"
#include "aom_external_partition.h"
#include "aom_decoder.h"
#include "aom_image.h"
#include "aom_frame_buffer.h"
#include "aom_integer.h"

static int try_fuzz_encoder(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx) return 0;

    // Fuzz AV1E_SET_ENABLE_INTRABC
    int enable_intrabc = 0;
    if (size > 0) enable_intrabc = (int)data[0] % 2;
    AOM_CODEC_CONTROL_TYPECHECKED(ctx, AV1E_SET_ENABLE_INTRABC, enable_intrabc);

    // Fuzz AV1E_SET_ENABLE_TX_SIZE_SEARCH
    int enable_tx_size_search = 0;
    if (size > 1) enable_tx_size_search = (int)data[1] % 2;
    AOM_CODEC_CONTROL_TYPECHECKED(ctx, AV1E_SET_ENABLE_TX_SIZE_SEARCH, enable_tx_size_search);

    // Fuzz AOME_SET_ACTIVEMAP
    if (size > 3) {
        unsigned int rows = (unsigned int)data[2] % 64;
        unsigned int cols = (unsigned int)data[3] % 64;
        if (rows > 0 && cols > 0) {
            size_t map_size = rows * cols;
            if (size > 4 + map_size) {
                aom_active_map_t active_map;
                active_map.active_map = (unsigned char *)&data[4];
                active_map.rows = rows;
                active_map.cols = cols;
                AOM_CODEC_CONTROL_TYPECHECKED(ctx, AOME_SET_ACTIVEMAP, &active_map);
            }
        }
    }

    // Fuzz AOME_SET_ROI_MAP
    if (size > 6) {
        unsigned int roi_rows = (unsigned int)data[5] % 64;
        unsigned int roi_cols = (unsigned int)data[6] % 64;
        if (roi_rows > 0 && roi_cols > 0) {
            size_t roi_map_size = roi_rows * roi_cols;
            // Ensure we have enough data for the map and potential extra fields
            if (size > 7 + roi_map_size) {
                aom_roi_map_t roi_map;
                memset(&roi_map, 0, sizeof(roi_map));
                roi_map.enabled = (uint8_t)data[7] % 2;
                roi_map.roi_map = (unsigned char *)&data[8];
                roi_map.rows = roi_rows;
                roi_map.cols = roi_cols;
                
                // Fill delta_q, delta_lf, skip, ref_frame with fuzz data if available
                // AOM_MAX_SEGMENTS is typically 8
                size_t extra_needed = 4 * 8; // delta_q, delta_lf, skip, ref_frame
                if (size > 8 + roi_map_size + extra_needed) {
                    for (int i = 0; i < 8; i++) {
                        roi_map.delta_q[i] = (int)data[8 + roi_map_size + i];
                        roi_map.delta_lf[i] = (int)data[8 + roi_map_size + 8 + i];
                        roi_map.skip[i] = (int)data[8 + roi_map_size + 16 + i];
                        roi_map.ref_frame[i] = (int)data[8 + roi_map_size + 24 + i];
                    }
                }
                AOM_CODEC_CONTROL_TYPECHECKED(ctx, AOME_SET_ROI_MAP, &roi_map);
            }
        }
    }

    // Fuzz AV1E_SET_ENABLE_INTERINTRA_COMP
    int enable_interintra = 0;
    if (size > 8) enable_interintra = (int)data[8] % 2;
    AOM_CODEC_CONTROL_TYPECHECKED(ctx, AV1E_SET_ENABLE_INTERINTRA_COMP, enable_interintra);

    // Fuzz AV1E_GET_LUMA_CDEF_STRENGTH with safe buffer size
    int cdef_strengths[16]; // Ensure buffer is large enough (16 ints = 64 bytes)
    aom_codec_err_t res = aom_codec_control(ctx, AV1E_GET_LUMA_CDEF_STRENGTH, cdef_strengths);
    (void)res; // Ignore return for fuzzing, but call is made

    return 1;
}

int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    // Get default encoder config
    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(aom_codec_av1_cx(), &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz config values slightly
    if (Size > 0) cfg.g_w = (unsigned int)(Data[0] % 1024) + 16;
    if (Size > 1) cfg.g_h = (unsigned int)(Data[1] % 1024) + 16;
    if (Size > 2) cfg.g_timebase.num = (uint32_t)(Data[2] % 100) + 1;
    if (Size > 3) cfg.g_timebase.den = (uint32_t)(Data[3] % 100) + 1;
    if (Size > 4) cfg.g_threads = (unsigned int)(Data[4] % 16) + 1;

    // Initialize encoder
    if (aom_codec_enc_init(&ctx, aom_codec_av1_cx(), &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Run fuzz logic
    try_fuzz_encoder(&ctx, Data, Size);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}