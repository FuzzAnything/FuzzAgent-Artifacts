// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// VP8E_SET_CPUUSED__value at vp8cx.h:984:1 in vp8cx.h
// VP8E_SET_NOISE_SENSITIVITY__value at vp8cx.h:988:1 in vp8cx.h
// VP8E_SET_FRAME_FLAGS__value at vp8cx.h:1012:1 in vp8cx.h
// VP8E_SET_TEMPORAL_LAYER_ID__value at vp8cx.h:1018:1 in vp8cx.h
// VP8E_SET_ACTIVEMAP__value at vp8cx.h:980:1 in vp8cx.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>
#include <vpx/vpx_image.h>

/* 
 * Helper to safely allocate and initialize a vpx_active_map_t from fuzz input.
 */
static int init_active_map_from_input(const uint8_t *data, size_t size, vpx_active_map_t *map) {
    if (size < 4) return -1;
    
    /* Parse rows and cols from the first 4 bytes */
    unsigned int rows = data[0];
    unsigned int cols = data[1];
    
    /* Sanity check to prevent huge allocations */
    if (rows > 256 || cols > 256) return -1;
    if (rows == 0 || cols == 0) return -1;
    
    size_t map_size = rows * cols;
    if (size < 4 + map_size) return -1;
    
    map->rows = rows;
    map->cols = cols;
    map->active_map = (unsigned char *)malloc(map_size);
    
    if (!map->active_map) return -1;
    
    memcpy(map->active_map, data + 4, map_size);
    
    return 0;
}

/* 
 * Helper to cleanup active map
 */
static void cleanup_active_map(vpx_active_map_t *map) {
    if (map && map->active_map) {
        free(map->active_map);
        map->active_map = NULL;
    }
}

int LLVMFuzzerTestOneInput_116(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    vpx_codec_ctx_t ctx;
    vpx_codec_iface_t *iface = vpx_codec_vp8_cx();
    vpx_codec_enc_cfg_t cfg;
    int res;
    
    if (!iface) return 0;

    res = vpx_codec_enc_config_default(iface, &cfg, 0);
    if (res != VPX_CODEC_OK) {
        return 0;
    }

    /* Configure basic settings */
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.g_lag_in_frames = 0;

    res = vpx_codec_enc_init(&ctx, iface, &cfg, 0);
    if (res != VPX_CODEC_OK) {
        return 0;
    }

    /* 
     * Prepare a dummy frame to encode to keep the encoder in a valid state 
     */
    vpx_image_t *img = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, 64, 64, 16);
    if (!img) {
        vpx_codec_destroy(&ctx);
        return 0;
    }
    
    /* Fill image with fuzz data */
    size_t img_size = img->d_w * img->d_h * 3 / 2;
    if (Size > 10 + img_size) {
        memcpy(img->planes[0], Data + 10, img_size);
    } else {
        memset(img->planes[0], 0, img_size);
    }

    /* 
     * Invoke the target control functions using vpx_codec_control.
     */
    
    /* VP8E_SET_CPUUSED */
    vpx_codec_control(&ctx, VP8E_SET_CPUUSED, (int)Data[1] % 9);
    
    /* VP8E_SET_NOISE_SENSITIVITY */
    vpx_codec_control(&ctx, VP8E_SET_NOISE_SENSITIVITY, (int)Data[2] % 256);
    
    /* VP8E_SET_FRAME_FLAGS */
    vpx_codec_control(&ctx, VP8E_SET_FRAME_FLAGS, (int)Data[3]);
    
    /* VP8E_SET_TEMPORAL_LAYER_ID */
    vpx_codec_control(&ctx, VP8E_SET_TEMPORAL_LAYER_ID, (int)Data[4] % 8);

    /* 
     * For Active Map controls, we need to construct the struct.
     */
    vpx_active_map_t amap;
    memset(&amap, 0, sizeof(amap));
    
    if (init_active_map_from_input(Data, Size, &amap) == 0) {
        /* VP8E_SET_ACTIVEMAP */
        vpx_codec_control(&ctx, VP8E_SET_ACTIVEMAP, &amap);
        /* Note: VP8E_GET_ACTIVEMAP is not a standard control in this version/context, skipping to avoid compile error */
        cleanup_active_map(&amap);
    }

    /* 
     * Attempt to encode a frame to ensure the state changes are applied 
     */
    vpx_codec_encode(&ctx, img, 0, 1, 0, 0);

    vpx_codec_destroy(&ctx);
    vpx_img_free(img);

    return 0;
}