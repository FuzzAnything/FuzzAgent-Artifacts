// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
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
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_codec.h"
#include "vpx/vp8cx.h"
#include "vpx/vpx_image.h"
#include "vpx/vpx_encoder.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    // Step 1: vpx_codec_iface_name
    vpx_codec_iface_t *iface1 = vpx_codec_vp8_cx();
    const char *name1 = vpx_codec_iface_name(iface1);
    (void)name1; // Use result to avoid unused variable warning
    
    // Step 2: vpx_codec_vp8_cx
    vpx_codec_iface_t *iface2 = vpx_codec_vp8_cx();
    
    // Step 3: vpx_codec_enc_config_default
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_err_t err = vpx_codec_enc_config_default(iface2, &cfg, 0);
    
    // Step 4: vpx_codec_vp8_cx
    vpx_codec_iface_t *iface3 = vpx_codec_vp8_cx();
    
    // Step 5: vpx_codec_err_to_string
    const char *err_str = vpx_codec_err_to_string(err);
    (void)err_str; // Use result
    
    // Step 6: vpx_img_alloc
    // Use fuzz data to determine parameters safely
    unsigned int width = 64;
    unsigned int height = 64;
    unsigned int align = 16;
    
    if (Size >= 12) {
        // Use first 12 bytes for width, height, align (4 bytes each)
        width = *(const unsigned int *)(Data) % 4096 + 1; // Limit to reasonable size
        height = *(const unsigned int *)(Data + 4) % 4096 + 1;
        align = *(const unsigned int *)(Data + 8) % 256 + 1;
        Data += 12;
        Size -= 12;
    }
    
    vpx_image_t *img = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, width, height, align);
    
    if (img) {
        // Fill image with fuzz data if available
        size_t bytes_per_pixel = 1; // I420 is 8-bit
        size_t y_plane_size = width * height;
        size_t uv_plane_size = (width / 2) * (height / 2);
        size_t total_size = y_plane_size + 2 * uv_plane_size;
        
        if (Size >= total_size) {
            // Fill Y plane
            memcpy(img->planes[VPX_PLANE_Y], Data, y_plane_size);
            Data += y_plane_size;
            Size -= y_plane_size;
            
            // Fill U plane
            memcpy(img->planes[VPX_PLANE_U], Data, uv_plane_size);
            Data += uv_plane_size;
            Size -= uv_plane_size;
            
            // Fill V plane
            memcpy(img->planes[VPX_PLANE_V], Data, uv_plane_size);
            Data += uv_plane_size;
            Size -= uv_plane_size;
        } else if (Size > 0) {
            // Partial fill
            size_t remaining = Size;
            if (remaining > 0) {
                size_t to_copy = remaining > y_plane_size ? y_plane_size : remaining;
                memcpy(img->planes[VPX_PLANE_Y], Data, to_copy);
                remaining -= to_copy;
                Data += to_copy;
            }
            if (remaining > 0) {
                size_t to_copy = remaining > uv_plane_size ? uv_plane_size : remaining;
                memcpy(img->planes[VPX_PLANE_U], Data, to_copy);
                remaining -= to_copy;
                Data += to_copy;
            }
            if (remaining > 0) {
                size_t to_copy = remaining > uv_plane_size ? uv_plane_size : remaining;
                memcpy(img->planes[VPX_PLANE_V], Data, to_copy);
            }
        }
        
        // Set color space and range
        img->cs = VPX_CS_BT_601;
        img->range = VPX_CR_STUDIO_RANGE;
    }
    
    // Step 7: vpx_codec_vp8_cx
    vpx_codec_iface_t *iface4 = vpx_codec_vp8_cx();
    
    // Initialize codec context
    vpx_codec_ctx_t codec;
    vpx_codec_err_t init_err = vpx_codec_enc_init(&codec, iface4, &cfg, 0);
    
    if (init_err == VPX_CODEC_OK && img) {
        // Step 8: vpx_codec_encode
        // Use remaining fuzz data for PTS and duration if available
        vpx_codec_pts_t pts = 0;
        unsigned long duration = 1;
        vpx_enc_frame_flags_t flags = 0;
        vpx_enc_deadline_t deadline = 0;
        
        if (Size >= 8) {
            pts = *(const vpx_codec_pts_t *)Data;
            Data += sizeof(vpx_codec_pts_t);
            Size -= sizeof(vpx_codec_pts_t);
            
            if (Size >= sizeof(unsigned long)) {
                duration = *(const unsigned long *)Data;
                Data += sizeof(unsigned long);
                Size -= sizeof(unsigned long);
            }
        }
        
        vpx_codec_err_t encode_err = vpx_codec_encode(&codec, img, pts, duration, flags, deadline);
        (void)encode_err;
        
        // Also encode with NULL img to flush
        if (Size > 0) {
            vpx_codec_pts_t flush_pts = pts + 1;
            vpx_codec_encode(&codec, NULL, flush_pts, duration, flags, deadline);
        }
        
        // Step 9: vpx_codec_get_cx_data
        vpx_codec_iter_t iter = NULL;
        const vpx_codec_cx_pkt_t *pkt;
        while ((pkt = vpx_codec_get_cx_data(&codec, &iter)) != NULL) {
            // Process packet - just access fields to explore state
            (void)pkt->kind;
            if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                (void)pkt->data.frame.buf;
                (void)pkt->data.frame.sz;
                (void)pkt->data.frame.pts;
                (void)pkt->data.frame.duration;
                (void)pkt->data.frame.flags;
            }
        }
    }
    
    // Step 10: vpx_codec_destroy
    vpx_codec_err_t destroy_err = vpx_codec_destroy(&codec);
    (void)destroy_err;
    
    // Step 11: vpx_img_free
    vpx_img_free(img);
    
    return 0;
}