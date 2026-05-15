// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_iface_name at vpx_codec.c:30:13 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_err_to_string at vpx_codec.c:34:13 in vpx_codec.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_encoder.h"
#include "vpx/vp8cx.h"
#include "vpx/vpx_codec.h"
#include "vpx/vpx_image.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void fill_image_with_data(vpx_image_t *img, const uint8_t *data, size_t size) {
    if (!img || !data) return;
    
    size_t offset = 0;
    for (int plane = 0; plane < 3; ++plane) {
        if (!img->planes[plane]) continue;
        
        int stride = img->stride[plane];
        int plane_height = (plane == 0) ? img->d_h : (img->d_h + 1) >> img->y_chroma_shift;
        
        for (int y = 0; y < plane_height; ++y) {
            size_t row_size = stride;
            if (offset + row_size > size) {
                row_size = size - offset;
                if (row_size <= 0) return;
            }
            
            memcpy(img->planes[plane] + y * stride, data + offset, row_size);
            offset += row_size;
            
            if (offset >= size) return;
        }
    }
}

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // 1. vpx_codec_iface_name
    vpx_codec_iface_t *iface = vpx_codec_vp8_cx();
    const char *iface_name = vpx_codec_iface_name(iface);
    (void)iface_name; // Use the result to avoid unused variable warning
    
    // 2. vpx_codec_vp8_cx
    vpx_codec_iface_t *iface2 = vpx_codec_vp8_cx();
    
    // 3. vpx_codec_enc_config_default
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_err_t err = vpx_codec_enc_config_default(iface2, &cfg, 0);
    if (err != VPX_CODEC_OK) {
        return 0;
    }
    
    // Use fuzzer data to modify some configuration parameters
    if (Size > 10) {
        cfg.g_w = (Data[0] % 128) + 16;  // Width between 16 and 143
        cfg.g_h = (Data[1] % 128) + 16;  // Height between 16 and 143
        cfg.rc_target_bitrate = (Data[2] % 1000) + 100;  // Bitrate between 100 and 1099
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = (Data[3] % 60) + 1;  // Framerate between 1 and 60
    }
    
    // 4. vpx_codec_vp8_cx (again)
    vpx_codec_iface_t *iface3 = vpx_codec_vp8_cx();
    
    // 5. vpx_codec_err_to_string
    const char *err_str = vpx_codec_err_to_string(err);
    (void)err_str;
    
    // 6. vpx_img_alloc
    vpx_img_fmt_t fmt = VPX_IMG_FMT_I420;
    unsigned int width = cfg.g_w;
    unsigned int height = cfg.g_h;
    unsigned int align = 16;
    
    // Ensure dimensions are valid for I420 format
    if (width % 2 != 0) width++;
    if (height % 2 != 0) height++;
    
    vpx_image_t *img = vpx_img_alloc(NULL, fmt, width, height, align);
    if (!img) {
        return 0;
    }
    
    // Fill image with fuzzer data
    fill_image_with_data(img, Data, Size);
    
    // 7. vpx_codec_vp8_cx (again)
    vpx_codec_iface_t *iface4 = vpx_codec_vp8_cx();
    
    // Initialize encoder
    vpx_codec_ctx_t codec;
    err = vpx_codec_enc_init(&codec, iface4, &cfg, 0);
    if (err != VPX_CODEC_OK) {
        vpx_img_free(img);
        return 0;
    }
    
    // 8. vpx_codec_encode
    vpx_codec_pts_t pts = 0;
    unsigned long duration = 1;
    vpx_enc_frame_flags_t flags = 0;
    vpx_enc_deadline_t deadline = VPX_DL_REALTIME;
    
    // Encode a frame
    err = vpx_codec_encode(&codec, img, pts, duration, flags, deadline);
    if (err != VPX_CODEC_OK) {
        vpx_codec_destroy(&codec);
        vpx_img_free(img);
        return 0;
    }
    
    // Flush the encoder
    err = vpx_codec_encode(&codec, NULL, pts, 0, flags, deadline);
    
    // 9. vpx_codec_get_cx_data
    vpx_codec_iter_t iter = NULL;
    const vpx_codec_cx_pkt_t *pkt;
    while ((pkt = vpx_codec_get_cx_data(&codec, &iter)) != NULL) {
        // Process the packet (just consume it for fuzzing)
        (void)pkt;
    }
    
    // 10. vpx_codec_destroy
    err = vpx_codec_destroy(&codec);
    (void)err;
    
    // 11. vpx_img_free
    vpx_img_free(img);
    
    return 0;
}