// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_get_global_headers at vpx_encoder.c:331:18 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_enc_config_set at vpx_encoder.c:348:17 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_encoder.h"
#include "vpx/vpx_codec.h"
#include "vpx/vp8cx.h"
#include "vpx/vp8.h"

static void fuzz_init_image(vpx_image_t *img, const uint8_t *data, size_t size) {
    if (size < 64) return;
    
    img->fmt = VPX_IMG_FMT_I420;
    img->cs = VPX_CS_BT_601;
    img->range = VPX_CR_STUDIO_RANGE;
    img->w = 64;
    img->h = 64;
    img->bit_depth = 8;
    img->d_w = 64;
    img->d_h = 64;
    img->r_w = 64;
    img->r_h = 64;
    img->x_chroma_shift = 1;
    img->y_chroma_shift = 1;
    img->bps = 12;
    
    size_t y_plane = img->w * img->h;
    size_t uv_plane = (img->w >> 1) * (img->h >> 1);
    size_t total_size = y_plane + 2 * uv_plane;
    
    img->img_data = malloc(total_size);
    if (!img->img_data) return;
    
    img->img_data_owner = 1;
    img->self_allocd = 1;
    
    size_t copy_size = size < total_size ? size : total_size;
    memcpy(img->img_data, data, copy_size);
    
    if (copy_size < total_size) {
        memset(img->img_data + copy_size, 128, total_size - copy_size);
    }
    
    img->planes[VPX_PLANE_Y] = img->img_data;
    img->planes[VPX_PLANE_U] = img->img_data + y_plane;
    img->planes[VPX_PLANE_V] = img->img_data + y_plane + uv_plane;
    
    img->stride[VPX_PLANE_Y] = img->w;
    img->stride[VPX_PLANE_U] = img->w >> 1;
    img->stride[VPX_PLANE_V] = img->w >> 1;
}

static void fuzz_init_config(vpx_codec_enc_cfg_t *cfg, const uint8_t *data, size_t size) {
    if (size < sizeof(vpx_codec_enc_cfg_t)) return;
    
    memcpy(cfg, data, sizeof(vpx_codec_enc_cfg_t));
    
    cfg->g_w = 64;
    cfg->g_h = 64;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->rc_target_bitrate = 256;
    cfg->rc_min_quantizer = 0;
    cfg->rc_max_quantizer = 63;
    cfg->g_threads = 1;
    cfg->g_profile = 0;
    cfg->g_pass = VPX_RC_ONE_PASS;
    cfg->g_lag_in_frames = 0;
    cfg->rc_end_usage = VPX_VBR;
}

int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    vpx_codec_ctx_t codec;
    vpx_codec_enc_cfg_t cfg;
    vpx_image_t img;
    
    const vpx_codec_iface_t *iface = vpx_codec_vp8_cx();
    if (!iface) return 0;
    
    fuzz_init_config(&cfg, Data, Size);
    
    vpx_codec_err_t err = vpx_codec_enc_init_ver(&codec, iface, &cfg, 0, VPX_ENCODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        return 0;
    }
    
    size_t data_offset = sizeof(vpx_codec_enc_cfg_t) < Size ? sizeof(vpx_codec_enc_cfg_t) : Size;
    fuzz_init_image(&img, Data + data_offset, Size - data_offset);
    
    uint32_t control_id = *(uint32_t *)Data;
    int int_arg = (int)(control_id >> 16);
    void *ptr_arg = NULL;
    
    vpx_codec_control_(&codec, control_id & 0xFF, int_arg);
    vpx_codec_control_(&codec, control_id & 0xFF, ptr_arg);
    
    vpx_codec_encode(&codec, &img, 0, 1, 0, VPX_DL_REALTIME);
    
    vpx_codec_iter_t iter = NULL;
    const vpx_codec_cx_pkt_t *pkt;
    while ((pkt = vpx_codec_get_cx_data(&codec, &iter)) != NULL) {
    }
    
    vpx_fixed_buf_t *headers = vpx_codec_get_global_headers(&codec);
    if (headers) {
    }
    
    vpx_codec_encode(&codec, NULL, 1, 1, 0, VPX_DL_REALTIME);
    
    iter = NULL;
    while ((pkt = vpx_codec_get_cx_data(&codec, &iter)) != NULL) {
    }
    
    if (Size > 100) {
        fuzz_init_config(&cfg, Data + 50, Size - 50);
        vpx_codec_enc_config_set(&codec, &cfg);
        
        vpx_codec_encode(&codec, &img, 2, 1, 0, VPX_DL_GOOD_QUALITY);
        
        iter = NULL;
        while ((pkt = vpx_codec_get_cx_data(&codec, &iter)) != NULL) {
        }
    }
    
    if (img.img_data_owner && img.img_data) {
        free(img.img_data);
    }
    
    vpx_codec_destroy(&codec);
    
    return 0;
}