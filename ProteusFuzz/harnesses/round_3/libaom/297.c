// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom_integer.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_image.h>
#include <aom/aom_external_partition.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static int init_encoder_context(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    int w = 64, h = 64;

    if (size >= 4) {
        w = data[0] ? data[0] : 64;
        h = data[1] ? data[1] : 64;
        if (w > 4096 || h > 4096) { w = 64; h = 64; }
    }

    iface = aom_codec_av1_cx();
    if (!iface) return -1;

    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }

    cfg.g_w = w;
    cfg.g_h = h;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_error_resilient = 0;
    cfg.rc_end_usage = AOM_CQ;
    cfg.rc_target_bitrate = 100;
    cfg.g_threads = 1;
    cfg.g_profile = 0;

    if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return -1;
    }

    return 0;
}

int LLVMFuzzerTestOneInput_297(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    aom_roi_map_t roi_map;
    aom_active_map_t active_map;
    int enable_tx_size_search;
    int enable_cfl_intra;
    int enable_intrabc;
    int transfer_char;
    int ret;

    memset(&ctx, 0, sizeof(ctx));
    memset(&roi_map, 0, sizeof(roi_map));
    memset(&active_map, 0, sizeof(active_map));

    if (Size < 1) return 0;

    if (init_encoder_context(&ctx, Data, Size) != 0) {
        return 0;
    }

    // Parse input for parameters
    if (Size > 1) enable_tx_size_search = Data[0];
    if (Size > 2) enable_cfl_intra = Data[1];
    if (Size > 3) enable_intrabc = Data[2];
    if (Size > 4) transfer_char = Data[3] % 22; // Valid range for transfer characteristics

    // Prepare ROI Map
    if (Size > 5) {
        roi_map.enabled = Data[4];
        roi_map.rows = (Size > 5) ? (Data[5] % 100) + 1 : 1;
        roi_map.cols = (Size > 6) ? (Data[6] % 100) + 1 : 1;
        size_t map_size = roi_map.rows * roi_map.cols;
        roi_map.roi_map = (unsigned char *)malloc(map_size);
        if (roi_map.roi_map) {
            if (Size > 7) {
                size_t copy_len = (Size - 7) < map_size ? (Size - 7) : map_size;
                memcpy(roi_map.roi_map, Data + 7, copy_len);
                if (copy_len < map_size) {
                    memset(roi_map.roi_map + copy_len, 0, map_size - copy_len);
                }
            } else {
                memset(roi_map.roi_map, 0, map_size);
            }
            // Initialize other fields to safe defaults
            memset(roi_map.delta_q, 0, sizeof(roi_map.delta_q));
            memset(roi_map.delta_lf, 0, sizeof(roi_map.delta_lf));
            memset(roi_map.skip, 0, sizeof(roi_map.skip));
            memset(roi_map.ref_frame, 0, sizeof(roi_map.ref_frame));
            roi_map.delta_qp_enabled = 0;
            roi_map.reference_enabled = 0;
            roi_map.rdmult_delta_qp = 0;
        }
    }

    // Prepare Active Map
    if (Size > 8) {
        active_map.rows = (Data[7] % 100) + 1;
        active_map.cols = (Data[8] % 100) + 1;
        size_t map_size = active_map.rows * active_map.cols;
        active_map.active_map = (unsigned char *)malloc(map_size);
        if (active_map.active_map) {
            if (Size > 9) {
                size_t copy_len = (Size - 9) < map_size ? (Size - 9) : map_size;
                memcpy(active_map.active_map, Data + 9, copy_len);
                if (copy_len < map_size) {
                    memset(active_map.active_map + copy_len, 0, map_size - copy_len);
                }
            } else {
                memset(active_map.active_map, 0, map_size);
            }
        }
    }

    // Call Target Functions
    // 1. AV1E_SET_ENABLE_TX_SIZE_SEARCH
    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_TX_SIZE_SEARCH, enable_tx_size_search);
    (void)ret;

    // 2. AV1E_SET_ENABLE_CFL_INTRA
    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_CFL_INTRA, enable_cfl_intra);
    (void)ret;

    // 3. AV1E_SET_TRANSFER_CHARACTERISTICS
    ret = aom_codec_control(&ctx, AV1E_SET_TRANSFER_CHARACTERISTICS, transfer_char);
    (void)ret;

    // 4. AOME_SET_ROI_MAP
    if (roi_map.roi_map) {
        ret = aom_codec_control(&ctx, AOME_SET_ROI_MAP, &roi_map);
        (void)ret;
    }

    // 5. AV1E_SET_ENABLE_INTRABC
    ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_INTRABC, enable_intrabc);
    (void)ret;

    // 6. AOME_SET_ACTIVEMAP
    if (active_map.active_map) {
        ret = aom_codec_control(&ctx, AOME_SET_ACTIVEMAP, &active_map);
        (void)ret;
    }

    // Cleanup allocated memory
    if (roi_map.roi_map) free(roi_map.roi_map);
    if (active_map.active_map) free(active_map.active_map);

    cleanup_codec(&ctx);
    return 0;
}