// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
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
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vp8cx.h"
#include "vpx/vpx_image.h"
#include "vpx/vpx_codec.h"
#include "vpx/vpx_encoder.h"

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    // Call functions in the specified order
    
    // 1. vpx_codec_iface_name
    vpx_codec_iface_t *iface = vpx_codec_vp8_cx();
    const char *name = vpx_codec_iface_name(iface);
    (void)name; // Use to avoid unused variable warning
    
    // 2. vpx_codec_vp8_cx (already called above, but call again as per order)
    iface = vpx_codec_vp8_cx();
    
    // 3. vpx_codec_enc_config_default
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_err_t err = vpx_codec_enc_config_default(iface, &cfg, 0);
    if (err != VPX_CODEC_OK) {
        // Handle error
        return 0;
    }
    
    // 4. vpx_codec_vp8_cx (again)
    iface = vpx_codec_vp8_cx();
    
    // 5. vpx_codec_err_to_string
    const char *err_str = vpx_codec_err_to_string(err);
    (void)err_str; // Use to avoid unused variable warning
    
    // 6. vpx_img_alloc
    // Use fuzzer data to determine image parameters within safe bounds
    unsigned int width = 16;
    unsigned int height = 16;
    unsigned int align = 1;
    vpx_img_fmt_t fmt = VPX_IMG_FMT_I420;
    
    if (Size >= 3) {
        width = (Data[0] % 64) + 1;  // Limit to reasonable size
        height = (Data[1] % 64) + 1;
        align = (Data[2] % 16) + 1;
    }
    
    vpx_image_t *img = vpx_img_alloc(NULL, fmt, width, height, align);
    if (!img) {
        return 0;
    }
    
    // Fill image planes with fuzzer data if available
    if (Size > 3) {
        size_t offset = 3;
        for (int plane = 0; plane < 3; plane++) {
            if (img->planes[plane]) {
                int plane_size = img->stride[plane] * height;
                if (plane == 1 || plane == 2) { // UV planes are half height for 4:2:0
                    plane_size = img->stride[plane] * ((height + 1) / 2);
                }
                size_t copy_size = (Size - offset < plane_size) ? (Size - offset) : plane_size;
                if (copy_size > 0) {
                    memcpy(img->planes[plane], Data + offset, copy_size);
                    offset += copy_size;
                }
            }
        }
    }
    
    // 7. vpx_codec_vp8_cx (again)
    iface = vpx_codec_vp8_cx();
    
    // Update config with image dimensions
    cfg.g_w = width;
    cfg.g_h = height;
    cfg.rc_target_bitrate = 1000; // Set a reasonable bitrate
    
    // Initialize encoder
    vpx_codec_ctx_t codec;
    err = vpx_codec_enc_init(&codec, iface, &cfg, 0);
    if (err != VPX_CODEC_OK) {
        vpx_img_free(img);
        return 0;
    }
    
    // 8. vpx_codec_encode
    vpx_codec_pts_t pts = 0;
    unsigned long duration = 1;
    vpx_enc_frame_flags_t flags = 0;
    vpx_enc_deadline_t deadline = 0; // Infinite deadline
    
    err = vpx_codec_encode(&codec, img, pts, duration, flags, deadline);
    (void)err; // Check error if needed
    
    // Flush encoder
    err = vpx_codec_encode(&codec, NULL, pts, 0, flags, deadline);
    
    // 9. vpx_codec_get_cx_data
    vpx_codec_iter_t iter = NULL;
    const vpx_codec_cx_pkt_t *pkt;
    while ((pkt = vpx_codec_get_cx_data(&codec, &iter)) != NULL) {
        // Process packet if needed
        (void)pkt;
    }
    
    // 10. vpx_codec_destroy
    err = vpx_codec_destroy(&codec);
    (void)err; // Check error if needed
    
    // 11. vpx_img_free
    vpx_img_free(img);
    
    return 0;
}