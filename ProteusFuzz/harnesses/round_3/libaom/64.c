// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aom_codec.h"
#include "aom/aomdx.h"
#include "aom/aom_integer.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_image.h"
#include "aom/aom.h"
#include "aom/aomcx.h"
#include "aom/aom_external_partition.h"

/*
 * Note: The specific typechecked control functions listed in the prompt
 * (e.g., aom_codec_control_typechecked_AV1E_SET_REDUCED_REFERENCE_SET)
 * are internal or generated macros often mapping to aom_codec_control with
 * specific enum values. Since the prompt states signatures are empty,
 * this driver uses the standard aom_codec_control API with the corresponding
 * AV1 encoder control enums to exercise the same code paths.
 */

static aom_codec_ctx_t *init_encoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t res;
    unsigned int width, height;

    if (size < 4) return NULL;

    /* Fuzz width and height from input, clamp to reasonable limits */
    width = (unsigned int)(data[0] | (data[1] << 8));
    height = (unsigned int)(data[2] | (data[3] << 8));

    if (width == 0 || height == 0) {
        width = 16;
        height = 16;
    }
    if (width > 4096 || height > 4096) {
        width = 128;
        height = 128;
    }

    iface = aom_codec_av1_cx();
    if (!iface) return NULL;

    ctx = malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    res = aom_codec_enc_config_default(iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.g_profile = 0;

    res = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_64(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t *ctx = NULL;
    aom_roi_map_t roi_map;
    int val;
    unsigned char roi_data[256];

    if (Size == 0) return 0;

    /* Initialize encoder context */
    ctx = init_encoder(Data, Size);
    if (!ctx) return 0;

    /*
     * 1. AV1E_SET_REDUCED_REFERENCE_SET
     * Sets the reduced reference set mode (0: disable, 1: enable)
     */
    val = (int)(Data[0] % 2);
    aom_codec_control(ctx, AV1E_SET_REDUCED_REFERENCE_SET, val);

    /*
     * 2. AV1E_SET_ENABLE_CFL_INTRA
     * Enables/Disables Chroma From Luma intra prediction
     */
    val = (int)(Data[1] % 2);
    aom_codec_control(ctx, AV1E_SET_ENABLE_CFL_INTRA, val);

    /*
     * 3. AV1E_SET_ENABLE_1TO4_PARTITIONS
     * Enables/Disables 1:4 partitions
     */
    val = (int)(Data[2] % 2);
    aom_codec_control(ctx, AV1E_SET_ENABLE_1TO4_PARTITIONS, val);

    /*
     * 4. AV1E_SET_ENABLE_RECT_PARTITIONS
     * Enables/Disables rectangular partitions
     */
    val = (int)(Data[3] % 2);
    aom_codec_control(ctx, AV1E_SET_ENABLE_RECT_PARTITIONS, val);

    /*
     * 5. AOME_SET_ROI_MAP
     * Sets the Region of Interest map.
     * We construct a valid ROI map structure from fuzz input.
     */
    memset(&roi_map, 0, sizeof(roi_map));
    memset(roi_data, 0, sizeof(roi_data));

    if (Size > 4) {
        roi_map.enabled = (uint8_t)(Data[4] % 2);
        roi_map.rows = (unsigned int)(Data[5] % 16) + 1;
        roi_map.cols = (unsigned int)(Data[6] % 16) + 1;
        
        /* Ensure ROI map size is sufficient */
        size_t map_size = roi_map.rows * roi_map.cols;
        if (map_size <= sizeof(roi_data)) {
            for (size_t i = 0; i < map_size; i++) {
                roi_data[i] = (unsigned char)(Data[(7 + i) % Size] % 8);
            }
            roi_map.roi_map = roi_data;
        } else {
            /* Fallback if calculated size is too large for buffer */
            roi_map.rows = 1;
            roi_map.cols = 1;
            roi_map.roi_map = &roi_data[0];
            roi_data[0] = 0;
        }

        /* Populate delta arrays with fuzz data */
        for (int i = 0; i < AOM_MAX_SEGMENTS; i++) {
            int idx = (7 + i) % Size;
            roi_map.delta_q[i] = (int)(Data[idx] - 128);
            roi_map.delta_lf[i] = (int)(Data[(idx + 1) % Size] - 128);
            roi_map.skip[i] = (int)(Data[(idx + 2) % Size] % 2);
            roi_map.ref_frame[i] = (int)(Data[(idx + 3) % Size] % 7);
        }

        roi_map.delta_qp_enabled = (int)(Data[(7 + AOM_MAX_SEGMENTS * 4) % Size] % 2);
        roi_map.reference_enabled = (int)(Data[(7 + AOM_MAX_SEGMENTS * 4 + 1) % Size] % 2);
        roi_map.rdmult_delta_qp = (int)(Data[(7 + AOM_MAX_SEGMENTS * 4 + 2) % Size]);
    }

    aom_codec_control(ctx, AOME_SET_ROI_MAP, &roi_map);

    /*
     * 6. AV1E_SET_ENABLE_INTRABC
     * Enables/Disables Intra Block Copy
     */
    val = (int)(Data[Size - 1] % 2);
    aom_codec_control(ctx, AV1E_SET_ENABLE_INTRABC, val);

    /* Cleanup */
    cleanup_encoder(ctx);

    return 0;
}