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
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom_integer.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>

/*
 * Fuzz driver for libaom codec control functions.
 * Targets:
 * - AOME_SET_ACTIVEMAP
 * - AV1E_SET_ENABLE_WARPED_MOTION
 * - AV1E_SET_ENABLE_RESTORATION
 * - AV1E_SET_ALLOW_WARPED_MOTION
 * - AV1E_GET_ACTIVEMAP
 * - AV1E_SET_ENABLE_SUPERRES
 */

static aom_codec_ctx_t *init_encoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *codec = NULL;
    const aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t ret;
    int width, height;

    if (!data || size < 4) return NULL;

    width = data[0] & 0xFF;
    height = data[1] & 0xFF;

    /* Ensure reasonable dimensions */
    if (width == 0 || height == 0 || width > 4096 || height > 4096) {
        width = 64;
        height = 64;
    }

    /* Use the correct function name: aom_codec_av1_cx */
    iface = aom_codec_av1_cx();
    if (!iface) return NULL;

    ret = aom_codec_enc_config_default(iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) return NULL;

    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.g_threads = 1;
    cfg.g_error_resilient = 0;

    codec = malloc(sizeof(aom_codec_ctx_t));
    if (!codec) return NULL;
    memset(codec, 0, sizeof(aom_codec_ctx_t));

    ret = aom_codec_enc_init(codec, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        free(codec);
        return NULL;
    }

    return codec;
}

static void cleanup_encoder(aom_codec_ctx_t *codec) {
    if (codec) {
        aom_codec_destroy(codec);
        free(codec);
    }
}

int LLVMFuzzerTestOneInput_154(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t *codec;
    aom_codec_err_t ret;
    int val;
    aom_active_map_t active_map;

    if (Size < 1) return 0;

    codec = init_encoder(Data, Size);
    if (!codec) return 0;

    /*
     * Test AOME_SET_ACTIVEMAP
     * Requires an active_map structure.
     */
    if (Size > 2) {
        active_map.rows = Data[0] & 0xFF;
        active_map.cols = Data[1] & 0xFF;
        if (active_map.rows == 0 || active_map.cols == 0) {
            active_map.rows = 1;
            active_map.cols = 1;
        }
        active_map.rows = active_map.rows % 16 + 1;
        active_map.cols = active_map.cols % 16 + 1;

        size_t map_size = active_map.rows * active_map.cols;
        active_map.active_map = malloc(map_size);
        if (active_map.active_map) {
            if (Size > 2) {
                size_t copy_len = (Size - 2) < map_size ? (Size - 2) : map_size;
                memcpy(active_map.active_map, Data + 2, copy_len);
                if (copy_len < map_size) {
                    memset(active_map.active_map + copy_len, 0, map_size - copy_len);
                }
            } else {
                memset(active_map.active_map, 0, map_size);
            }

            ret = aom_codec_control(codec, AOME_SET_ACTIVEMAP, &active_map);
            (void)ret;

            free(active_map.active_map);
        }
    }

    /*
     * Test AV1E_SET_ENABLE_WARPED_MOTION
     * Input: int (0 or 1)
     */
    val = (Size > 3) ? (Data[3] & 0xFF) : 0;
    val = val % 2;
    ret = aom_codec_control(codec, AV1E_SET_ENABLE_WARPED_MOTION, val);
    (void)ret;

    /*
     * Test AV1E_SET_ENABLE_RESTORATION
     * Input: int (0 or 1)
     */
    val = (Size > 4) ? (Data[4] & 0xFF) : 0;
    val = val % 2;
    ret = aom_codec_control(codec, AV1E_SET_ENABLE_RESTORATION, val);
    (void)ret;

    /*
     * Test AV1E_SET_ALLOW_WARPED_MOTION
     * Input: int (0 or 1)
     */
    val = (Size > 5) ? (Data[5] & 0xFF) : 0;
    val = val % 2;
    ret = aom_codec_control(codec, AV1E_SET_ALLOW_WARPED_MOTION, val);
    (void)ret;

    /*
     * Test AV1E_GET_ACTIVEMAP
     * Output: aom_active_map_t
     */
    memset(&active_map, 0, sizeof(active_map));
    active_map.rows = 1;
    active_map.cols = 1;
    active_map.active_map = malloc(1);
    if (active_map.active_map) {
        ret = aom_codec_control(codec, AV1E_GET_ACTIVEMAP, &active_map);
        (void)ret;
        free(active_map.active_map);
    }

    /*
     * Test AV1E_SET_ENABLE_SUPERRES
     * Input: int (0 or 1)
     */
    val = (Size > 6) ? (Data[6] & 0xFF) : 0;
    val = val % 2;
    ret = aom_codec_control(codec, AV1E_SET_ENABLE_SUPERRES, val);
    (void)ret;

    cleanup_encoder(codec);
    return 0;
}