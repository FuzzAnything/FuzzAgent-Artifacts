// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_iface_name at vpx_codec.c:30:13 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_err_to_string at vpx_codec.c:34:13 in vpx_codec.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_codec.h"
#include "vpx/vp8cx.h"
#include "vpx/vpx_encoder.h"
#include "vpx/vpx_image.h"

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    // 1. vpx_codec_iface_name
    const char *iface_name = vpx_codec_iface_name(NULL);
    (void)iface_name; // Use result to avoid unused variable warning
    
    // 2. vpx_codec_vp8_cx (first call)
    vpx_codec_iface_t *iface1 = vpx_codec_vp8_cx();
    
    // 3. vpx_codec_enc_config_default
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_err_t err = vpx_codec_enc_config_default(iface1, &cfg, 0);
    
    // 4. vpx_codec_vp8_cx (second call)
    vpx_codec_iface_t *iface2 = vpx_codec_vp8_cx();
    
    // 5. vpx_codec_err_to_string
    const char *err_str = vpx_codec_err_to_string(err);
    (void)err_str;
    
    // 6. vpx_img_alloc
    // Use fuzzer data to determine image parameters
    unsigned int width = 16;
    unsigned int height = 16;
    unsigned int align = 1;
    
    if (Size >= 12) {
        width = (Data[0] << 24) | (Data[1] << 16) | (Data[2] << 8) | Data[3];
        height = (Data[4] << 24) | (Data[5] << 16) | (Data[6] << 8) | Data[7];
        align = (Data[8] % 32) + 1; // Alignment between 1 and 32
        
        // Limit dimensions to reasonable values to avoid excessive memory allocation
        width = width % 4096 + 1;
        height = height % 4096 + 1;
    }
    
    // Choose image format based on fuzzer input
    vpx_img_fmt_t fmt = VPX_IMG_FMT_I420;
    if (Size >= 13) {
        int fmt_idx = Data[9] % 4;
        switch (fmt_idx) {
            case 0: fmt = VPX_IMG_FMT_I420; break;
            case 1: fmt = VPX_IMG_FMT_I422; break;
            case 2: fmt = VPX_IMG_FMT_I444; break;
            case 3: fmt = VPX_IMG_FMT_NV12; break;
        }
    }
    
    vpx_image_t *img = vpx_img_alloc(NULL, fmt, width, height, align);
    
    // 7. vpx_codec_vp8_cx (third call)
    vpx_codec_iface_t *iface3 = vpx_codec_vp8_cx();
    
    // Initialize encoder context
    vpx_codec_ctx_t codec;
    if (vpx_codec_enc_init(&codec, iface3, &cfg, 0) == VPX_CODEC_OK) {
        // 8. vpx_codec_encode
        // Fill image data with fuzzer input if available
        if (img) {
            size_t data_offset = 10;
            for (int plane = 0; plane < 3; plane++) {
                if (img->planes[plane]) {
                    int plane_height = (plane == 0) ? height : (height >> img->y_chroma_shift);
                    int plane_width = (plane == 0) ? width : (width >> img->x_chroma_shift);
                    size_t plane_size = plane_height * img->stride[plane];
                    
                    for (size_t i = 0; i < plane_size && data_offset + i < Size; i++) {
                        img->planes[plane][i] = Data[data_offset + i];
                    }
                    data_offset += plane_size;
                }
            }
            
            // Determine encoding parameters from fuzzer input
            vpx_codec_pts_t pts = 0;
            unsigned long duration = 1;
            vpx_enc_frame_flags_t flags = 0;
            vpx_enc_deadline_t deadline = 0;
            
            if (Size >= data_offset + 8) {
                pts = ((int64_t)Data[data_offset] << 56) |
                      ((int64_t)Data[data_offset + 1] << 48) |
                      ((int64_t)Data[data_offset + 2] << 40) |
                      ((int64_t)Data[data_offset + 3] << 32) |
                      ((int64_t)Data[data_offset + 4] << 24) |
                      ((int64_t)Data[data_offset + 5] << 16) |
                      ((int64_t)Data[data_offset + 6] << 8) |
                      Data[data_offset + 7];
                
                duration = (Data[data_offset + 8] % 10) + 1;
                flags = Data[data_offset + 9] % 4;
                deadline = (Data[data_offset + 10] % 1000) * 1000; // 0-999ms
            }
            
            err = vpx_codec_encode(&codec, img, pts, duration, flags, deadline);
            
            // 9. vpx_codec_get_cx_data
            vpx_codec_iter_t iter = NULL;
            const vpx_codec_cx_pkt_t *pkt;
            while ((pkt = vpx_codec_get_cx_data(&codec, &iter)) != NULL) {
                // Process packet if needed
                (void)pkt;
            }
        }
        
        // Also try encoding with NULL image to flush
        if (Size >= 20) {
            vpx_codec_pts_t flush_pts = 1000;
            err = vpx_codec_encode(&codec, NULL, flush_pts, 0, 0, 0);
            
            // Get any remaining data
            vpx_codec_iter_t iter2 = NULL;
            const vpx_codec_cx_pkt_t *pkt2;
            while ((pkt2 = vpx_codec_get_cx_data(&codec, &iter2)) != NULL) {
                (void)pkt2;
            }
        }
        
        // 10. vpx_codec_destroy
        vpx_codec_destroy(&codec);
    }
    
    // 11. vpx_img_free
    vpx_img_free(img);
    
    return 0;
}