// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_iface_name at vpx_codec.c:30:13 in vpx_codec.h
// vpx_codec_get_caps at vpx_codec.c:85:18 in vpx_codec.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_enc_init_multi_ver at vpx_encoder.c:69:17 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>
#include <vpx/vpx_encoder.h>

static void cleanup_codec_contexts(vpx_codec_ctx_t *ctxs, int num_enc) {
    for (int i = 0; i < num_enc; i++) {
        if (ctxs[i].priv != NULL) {
            vpx_codec_destroy(&ctxs[i]);
        }
    }
}

static void cleanup_images(vpx_image_t **imgs, int num_enc) {
    for (int i = 0; i < num_enc; i++) {
        if (imgs[i]) {
            vpx_img_free(imgs[i]);
        }
    }
}

static vpx_image_t *create_test_image(const uint8_t *data, size_t size, vpx_img_fmt_t fmt, unsigned int w, unsigned int h) {
    // Safety checks for dimensions
    if (w == 0 || h == 0 || w > 2048 || h > 2048) return NULL;
    
    // Ensure alignment for VP8/VP9 (typically 16 or 32)
    if (w % 16 != 0) w += (16 - (w % 16));
    if (h % 16 != 0) h += (16 - (h % 16));

    vpx_image_t *img = vpx_img_alloc(NULL, fmt, w, h, 16);
    if (!img) return NULL;

    // Fill planes with fuzzing data if available, strictly respecting strides
    if (data && size > 0) {
        size_t bytes_written = 0;
        
        if (fmt == VPX_IMG_FMT_I420) {
            // Y plane
            size_t y_plane_size = (size_t)img->stride[VPX_PLANE_Y] * img->h;
            if (size >= y_plane_size) {
                memcpy(img->planes[VPX_PLANE_Y], data, y_plane_size);
                data += y_plane_size;
                size -= y_plane_size;
                bytes_written += y_plane_size;
            } else {
                // Fill with zeros if data is insufficient
                memset(img->planes[VPX_PLANE_Y], 0, y_plane_size);
                bytes_written += y_plane_size;
            }

            // U and V planes
            int chroma_h = (img->h + 1) / 2;
            size_t uv_plane_size = (size_t)img->stride[VPX_PLANE_U] * chroma_h;
            
            if (size >= uv_plane_size) {
                memcpy(img->planes[VPX_PLANE_U], data, uv_plane_size);
                data += uv_plane_size;
                size -= uv_plane_size;
                bytes_written += uv_plane_size;
            } else {
                memset(img->planes[VPX_PLANE_U], 0, uv_plane_size);
                bytes_written += uv_plane_size;
            }

            if (size >= uv_plane_size) {
                memcpy(img->planes[VPX_PLANE_V], data, uv_plane_size);
                data += uv_plane_size;
                size -= uv_plane_size;
                bytes_written += uv_plane_size;
            } else {
                memset(img->planes[VPX_PLANE_V], 0, uv_plane_size);
                bytes_written += uv_plane_size;
            }
        } else if (fmt == VPX_IMG_FMT_I42016) {
             // For 16-bit, we just fill the Y plane for simplicity in fuzzing
             size_t y_plane_size = (size_t)img->stride[VPX_PLANE_Y] * img->h;
             if (size >= y_plane_size) {
                 memcpy(img->planes[VPX_PLANE_Y], data, y_plane_size);
             } else {
                 memset(img->planes[VPX_PLANE_Y], 0, y_plane_size);
             }
        }
    }
    return img;
}

int LLVMFuzzerTestOneInput_71(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    // Extract parameters from fuzzing data
    unsigned int num_enc = (Data[0] % 16) + 1; // 1 to 16 encoders
    if (num_enc > 16) num_enc = 16;
    
    // Generate safe dimensions
    unsigned int width = (Data[1] % 320) + 16; 
    unsigned int height = ((Data[1] >> 4) % 240) + 16; 

    vpx_codec_iface_t *iface = vpx_codec_vp8_cx();
    if (!iface) {
        // Fallback to VP9 if VP8 is not available
        iface = vpx_codec_vp9_cx();
    }

    if (!iface) return 0;

    // Test vpx_codec_iface_name and vpx_codec_get_caps
    const char *name = vpx_codec_iface_name(iface);
    (void)name; 
    vpx_codec_caps_t caps = vpx_codec_get_caps(iface);
    (void)caps;

    // Allocate contexts
    vpx_codec_ctx_t *ctxs = (vpx_codec_ctx_t *)calloc(num_enc, sizeof(vpx_codec_ctx_t));
    if (!ctxs) return 0;

    // Allocate configs
    vpx_codec_enc_cfg_t *cfgs = (vpx_codec_enc_cfg_t *)calloc(num_enc, sizeof(vpx_codec_enc_cfg_t));
    if (!cfgs) {
        free(ctxs);
        return 0;
    }

    // Allocate down-sampling factors
    vpx_rational_t *dsf = (vpx_rational_t *)calloc(num_enc, sizeof(vpx_rational_t));
    if (!dsf) {
        free(cfgs);
        free(ctxs);
        return 0;
    }

    int init_success = 0;

    // Prepare configs for multi-init
    for (int i = 0; i < num_enc; i++) {
        if (vpx_codec_enc_config_default(iface, &cfgs[i], 0) != VPX_CODEC_OK) continue;
        
        cfgs[i].g_w = width;
        cfgs[i].g_h = height;
        cfgs[i].g_timebase.num = 1;
        cfgs[i].g_timebase.den = 30;
        cfgs[i].g_threads = 1;
        cfgs[i].rc_end_usage = VPX_VBR;
        cfgs[i].rc_target_bitrate = 500;
        cfgs[i].g_bit_depth = VPX_BITS_8;
        cfgs[i].g_input_bit_depth = 8;
        
        // Set down-sampling factor
        dsf[i].num = 1;
        dsf[i].den = 1;
    }

    // Call the target API: vpx_codec_enc_init_multi_ver
    vpx_codec_err_t multi_err = vpx_codec_enc_init_multi_ver(
        ctxs, iface, cfgs, num_enc, 0, dsf, VPX_ENCODER_ABI_VERSION
    );

    if (multi_err == VPX_CODEC_OK) {
        init_success = 1;
    }

    // If initialization succeeded, perform encoding
    if (init_success) {
        // Allocate an array of images to match the number of encoders
        // This is required for multi-resolution encoding (num_enc > 1)
        vpx_image_t **imgs = (vpx_image_t **)calloc(num_enc, sizeof(vpx_image_t *));
        if (!imgs) {
            cleanup_codec_contexts(ctxs, num_enc);
            free(ctxs);
            free(cfgs);
            free(dsf);
            return 0;
        }

        // Create images for each encoder context
        for (int i = 0; i < num_enc; i++) {
            imgs[i] = create_test_image(Data, Size, VPX_IMG_FMT_I420, width, height);
            if (!imgs[i]) {
                // If allocation fails, clean up previously allocated images and contexts
                cleanup_images(imgs, i);
                free(imgs);
                cleanup_codec_contexts(ctxs, num_enc);
                free(ctxs);
                free(cfgs);
                free(dsf);
                return 0;
            }
        }

        for (int i = 0; i < num_enc; i++) {
            if (ctxs[i].priv != NULL) {
                vpx_codec_pts_t pts = 0;
                unsigned long duration = 1;
                vpx_enc_frame_flags_t flags = 0;
                vpx_enc_deadline_t deadline = 0; // Infinite

                // Encode a frame using the corresponding image from the array
                vpx_codec_encode(&ctxs[i], imgs[i], pts, duration, flags, deadline);
                
                // Flush
                vpx_codec_encode(&ctxs[i], NULL, pts, duration, flags, deadline);
            }
        }
        
        cleanup_images(imgs, num_enc);
        free(imgs);
    }

    // Cleanup
    cleanup_codec_contexts(ctxs, num_enc);
    free(ctxs);
    free(cfgs);
    free(dsf);

    return 0;
}