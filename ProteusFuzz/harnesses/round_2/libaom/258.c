// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aom_image.h>
#include <aom/aom_integer.h>
#include <aom/aomcx.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static int init_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg,
                        int width, int height, int fps_num, int fps_den) {
    aom_codec_iface_t *iface;
    aom_codec_err_t ret;

    iface = aom_codec_av1_cx();
    if (!iface) return -1;

    ret = aom_codec_enc_config_default(iface, cfg, 0);
    if (ret != AOM_CODEC_OK) return -1;

    cfg->g_w = width;
    cfg->g_h = height;
    cfg->g_timebase.num = fps_num;
    cfg->g_timebase.den = fps_den;

    ret = aom_codec_enc_init(ctx, iface, cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return -1;
    }
    return 0;
}

static int32_t read_int32(const uint8_t *Data, size_t Size, size_t offset) {
    if (Size < offset + 4) return 0;
    int32_t val;
    memcpy(&val, Data + offset, 4);
    return val;
}

static uint16_t read_uint16(const uint8_t *Data, size_t Size, size_t offset) {
    if (Size < offset + 2) return 0;
    uint16_t val;
    memcpy(&val, Data + offset, 2);
    return val;
}

static uint8_t read_uint8(const uint8_t *Data, size_t Size, size_t offset) {
    if (Size < offset + 1) return 0;
    return Data[offset];
}

int LLVMFuzzerTestOneInput_258(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t ret;
    int width, height, fps_num, fps_den;
    
    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    if (Size < 8) return 0;

    width = read_uint16(Data, Size, 0);
    height = read_uint16(Data, Size, 2);
    fps_num = read_uint16(Data, Size, 4);
    fps_den = read_uint16(Data, Size, 6);

    if (width <= 0 || height <= 0 || fps_den <= 0) return 0;
    if (width > 8192 || height > 8192) return 0;
    if (fps_num > 10000 || fps_den > 10000) return 0;

    if (init_encoder(&ctx, &cfg, width, height, fps_num, fps_den) != 0) {
        return 0;
    }

    // Fuzz AOME_SET_ARNR_STRENGTH
    if (Size >= 12) {
        int arnr_strength = read_int32(Data, Size, 8);
        ret = aom_codec_control(&ctx, AOME_SET_ARNR_STRENGTH, arnr_strength);
        (void)ret;
    }

    // Fuzz AV1E_SET_COLOR_PRIMARIES
    if (Size >= 16) {
        int color_primaries = read_int32(Data, Size, 12);
        ret = aom_codec_control(&ctx, AV1E_SET_COLOR_PRIMARIES, color_primaries);
        (void)ret;
    }

    // Fuzz AOME_SET_ROI_MAP
    if (Size >= 22) {
        aom_roi_map_t roi_map;
        memset(&roi_map, 0, sizeof(roi_map));
        
        uint8_t enabled = read_uint8(Data, Size, 16);
        unsigned int rows = read_uint16(Data, Size, 17);
        unsigned int cols = read_uint16(Data, Size, 19);
        
        roi_map.enabled = enabled;
        roi_map.rows = rows;
        roi_map.cols = cols;
        
        if (rows > 0 && cols > 0 && rows * cols < 1024 * 1024) {
            roi_map.roi_map = (unsigned char *)malloc(rows * cols);
            if (roi_map.roi_map) {
                size_t map_size = rows * cols;
                size_t available = Size - 21;
                if (available > map_size) available = map_size;
                if (available > 0) {
                    memcpy(roi_map.roi_map, Data + 21, available);
                    if (available < map_size) {
                        memset(roi_map.roi_map + available, 0, map_size - available);
                    }
                }
                
                ret = aom_codec_control(&ctx, AOME_SET_ROI_MAP, &roi_map);
                (void)ret;
                free(roi_map.roi_map);
            }
        }
    }

    // Fuzz AV1E_SET_COLOR_RANGE
    if (Size >= 26) {
        int color_range = read_int32(Data, Size, 22);
        ret = aom_codec_control(&ctx, AV1E_SET_COLOR_RANGE, color_range);
        (void)ret;
    }

    // Fuzz AV1E_SET_TRANSFER_CHARACTERISTICS
    if (Size >= 30) {
        int transfer_characteristics = read_int32(Data, Size, 26);
        ret = aom_codec_control(&ctx, AV1E_SET_TRANSFER_CHARACTERISTICS, transfer_characteristics);
        (void)ret;
    }

    // Fuzz AOME_SET_ACTIVEMAP
    if (Size >= 34) {
        aom_active_map_t active_map;
        memset(&active_map, 0, sizeof(active_map));
        
        unsigned int rows = read_uint16(Data, Size, 30);
        unsigned int cols = read_uint16(Data, Size, 32);
        
        active_map.rows = rows;
        active_map.cols = cols;
        
        if (rows > 0 && cols > 0 && rows * cols < 1024 * 1024) {
            active_map.active_map = (unsigned char *)malloc(rows * cols);
            if (active_map.active_map) {
                size_t map_size = rows * cols;
                size_t available = Size - 34;
                if (available > map_size) available = map_size;
                if (available > 0) {
                    memcpy(active_map.active_map, Data + 34, available);
                    if (available < map_size) {
                        memset(active_map.active_map + available, 0, map_size - available);
                    }
                }
                
                ret = aom_codec_control(&ctx, AOME_SET_ACTIVEMAP, &active_map);
                (void)ret;
                free(active_map.active_map);
            }
        }
    }

    cleanup_codec(&ctx);

    return 0;
}