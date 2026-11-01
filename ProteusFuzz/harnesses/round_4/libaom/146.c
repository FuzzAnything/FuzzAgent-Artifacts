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
#include <aom/aomdx.h>
#include <aom/aom_encoder.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aom_integer.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aom_external_partition.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx || !data || size == 0) return 0;

    // Test AV1E_SET_ENABLE_INTERINTRA_COMP
    int val = data[0];
    aom_codec_control(ctx, AV1E_SET_ENABLE_INTERINTRA_COMP, val);

    // Test AV1E_SET_REDUCED_REFERENCE_SET
    if (size > 1) {
        val = data[1] & 0xFF;
        aom_codec_control(ctx, AV1E_SET_REDUCED_REFERENCE_SET, val);
    }

    // Test AV1E_SET_ENABLE_SMOOTH_INTERINTRA
    if (size > 2) {
        val = data[2];
        aom_codec_control(ctx, AV1E_SET_ENABLE_SMOOTH_INTERINTRA, val);
    }

    // Test AV1E_SET_ENABLE_SUPERRES
    if (size > 3) {
        val = data[3];
        aom_codec_control(ctx, AV1E_SET_ENABLE_SUPERRES, val);
    }

    // Test AOME_SET_ACTIVEMAP
    if (size > 10) {
        unsigned int rows = (data[4] & 0x0F) + 1;
        unsigned int cols = (data[5] & 0x0F) + 1;
        // Limit to reasonable size to avoid OOM in fuzzing
        if (rows > 100 || cols > 100) {
            rows = 10; cols = 10;
        }
        size_t map_size = rows * cols;
        unsigned char *map_data = malloc(map_size);
        if (map_data) {
            size_t available = size - 6;
            size_t copy_len = (map_size < available) ? map_size : available;
            memcpy(map_data, &data[6], copy_len);
            if (copy_len < map_size) {
                memset(map_data + copy_len, 0, map_size - copy_len);
            }
            aom_active_map_t am;
            am.active_map = map_data;
            am.rows = rows;
            am.cols = cols;
            aom_codec_control(ctx, AOME_SET_ACTIVEMAP, &am);
            free(map_data);
        }
    }

    // Test AOME_SET_ROI_MAP
    if (size > 26) {
        unsigned int r_rows = (data[10] & 0x0F) + 1;
        unsigned int r_cols = (data[11] & 0x0F) + 1;
        if (r_rows > 100 || r_cols > 100) {
            r_rows = 10; r_cols = 10;
        }
        size_t roi_size = r_rows * r_cols;
        unsigned char *roi_data = malloc(roi_size);
        if (roi_data) {
            size_t available_roi = size - 12;
            size_t copy_len_roi = (roi_size < available_roi) ? roi_size : available_roi;
            memcpy(roi_data, &data[12], copy_len_roi);
            if (copy_len_roi < roi_size) {
                memset(roi_data + copy_len_roi, 0, roi_size - copy_len_roi);
            }
            aom_roi_map_t rm;
            memset(&rm, 0, sizeof(rm));
            rm.enabled = data[20] & 1;
            rm.roi_map = roi_data;
            rm.rows = r_rows;
            rm.cols = r_cols;
            rm.delta_qp_enabled = (data[21] & 1);
            rm.reference_enabled = (data[22] & 1);
            // Fill some deltas safely
            for (int i = 0; i < AOM_MAX_SEGMENTS && i < 4; i++) {
                rm.delta_q[i] = (int)data[23 + i];
                rm.delta_lf[i] = (int)data[24 + i];
                rm.skip[i] = (data[25 + i] & 1);
                rm.ref_frame[i] = data[26 + i] & 7;
            }
            rm.rdmult_delta_qp = 1;
            aom_codec_control(ctx, AOME_SET_ROI_MAP, &rm);
            free(roi_data);
        }
    }

    return 1;
}

int LLVMFuzzerTestOneInput_146(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use input data to vary configuration slightly, ensuring bounds checks
    cfg.g_w = (Data[0] % 256) + 16;
    if (Size > 1) {
        cfg.g_h = (Data[1] % 256) + 16;
    } else {
        cfg.g_h = 16;
    }
    
    if (Size > 2) {
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = (Data[2] % 256) + 1;
    } else {
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 1;
    }

    if (Size > 3) {
        cfg.rc_target_bitrate = (Data[3] % 100) + 10;
    } else {
        cfg.rc_target_bitrate = 10;
    }

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Run fuzz logic
    fuzz_encoder_control(&ctx, Data, Size);

    // Cleanup
    cleanup_codec(&ctx);

    return 0;
}