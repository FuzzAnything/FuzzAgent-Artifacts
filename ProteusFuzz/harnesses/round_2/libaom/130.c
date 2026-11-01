// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aom_image.h>
#include <aom/aom_integer.h>
#include <aom/aomcx.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static int init_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg) {
    if (!ctx || !cfg) return 0;

    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    return 1;
}

int LLVMFuzzerTestOneInput_130(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    if (!init_encoder(&ctx, &cfg)) {
        return 0;
    }

    int ret = 0;
    uint8_t val = Data[0] % 2;
    uint8_t val2 = Data[1] % 2;
    uint8_t val3 = Data[2] % 2;
    uint8_t val4 = Data[3] % 2;

    // Fuzz AV1E_SET_REDUCED_REFERENCE_SET
    if (aom_codec_control(&ctx, AV1E_SET_REDUCED_REFERENCE_SET, val) != AOM_CODEC_OK) {
        ret = 1;
    }

    // Fuzz AOME_SET_ROI_MAP
    if (Size > 10) {
        aom_roi_map_t roi_map;
        memset(&roi_map, 0, sizeof(roi_map));
        roi_map.enabled = Data[4] % 2;
        roi_map.rows = (Data[5] % 100) + 1;
        roi_map.cols = (Data[6] % 100) + 1;
        size_t map_size = roi_map.rows * roi_map.cols;
        if (map_size > 0 && map_size < 10000) {
            roi_map.roi_map = malloc(map_size);
            if (roi_map.roi_map) {
                memcpy(roi_map.roi_map, &Data[7], map_size < (Size - 7) ? map_size : Size - 7);
                if (aom_codec_control(&ctx, AOME_SET_ROI_MAP, &roi_map) != AOM_CODEC_OK) {
                    ret = 1;
                }
                free(roi_map.roi_map);
            }
        }
    }

    // Fuzz AOME_SET_ACTIVEMAP
    if (Size > 15) {
        aom_active_map_t active_map;
        memset(&active_map, 0, sizeof(active_map));
        active_map.rows = (Data[10] % 100) + 1;
        active_map.cols = (Data[11] % 100) + 1;
        size_t map_size = active_map.rows * active_map.cols;
        if (map_size > 0 && map_size < 10000) {
            active_map.active_map = malloc(map_size);
            if (active_map.active_map) {
                memcpy(active_map.active_map, &Data[12], map_size < (Size - 12) ? map_size : Size - 12);
                if (aom_codec_control(&ctx, AOME_SET_ACTIVEMAP, &active_map) != AOM_CODEC_OK) {
                    ret = 1;
                }
                free(active_map.active_map);
            }
        }
    }

    // Fuzz AV1E_SET_ENABLE_WARPED_MOTION
    if (aom_codec_control(&ctx, AV1E_SET_ENABLE_WARPED_MOTION, val2) != AOM_CODEC_OK) {
        ret = 1;
    }

    // Fuzz AV1E_GET_ACTIVEMAP
    aom_active_map_t get_active_map;
    memset(&get_active_map, 0, sizeof(get_active_map));
    if (aom_codec_control(&ctx, AV1E_GET_ACTIVEMAP, &get_active_map) != AOM_CODEC_OK) {
        ret = 1;
    }
    if (get_active_map.active_map) {
        free(get_active_map.active_map);
    }

    // Fuzz AV1E_SET_ENABLE_SUPERRES
    if (aom_codec_control(&ctx, AV1E_SET_ENABLE_SUPERRES, val3) != AOM_CODEC_OK) {
        ret = 1;
    }

    cleanup_codec(&ctx);
    return ret;
}