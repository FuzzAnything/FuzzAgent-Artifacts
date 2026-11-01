// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_err_to_string at vpx_codec.c:34:13 in vpx_codec.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_err_to_string at vpx_codec.c:34:13 in vpx_codec.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_get_global_headers at vpx_encoder.c:331:18 in vpx_encoder.h
// vpx_codec_enc_config_set at vpx_encoder.c:348:17 in vpx_encoder.h
// vpx_codec_err_to_string at vpx_codec.c:34:13 in vpx_codec.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>

static vpx_codec_ctx_t ctx;
static vpx_codec_enc_cfg_t cfg;
static vpx_image_t img;
static uint8_t *img_data = NULL;
static size_t img_data_size = 0;

static void cleanup(void) {
    if (img_data != NULL) {
        free(img_data);
        img_data = NULL;
        img_data_size = 0;
    }
    if (ctx.name != NULL) {
        vpx_codec_destroy(&ctx);
        ctx.name = NULL;
    }
}

int LLVMFuzzerTestOneInput_76(const uint8_t *Data, size_t Size) {
    cleanup();

    if (Size < 2) {
        return 0;
    }

    // Initialize configuration
    vpx_codec_enc_config_default(vpx_codec_vp8_cx(), &cfg, 0);

    // Use fuzzing data to set configuration parameters
    cfg.g_w = Data[0] ? Data[0] : 64;
    cfg.g_h = Data[1] ? Data[1] : 64;
    cfg.g_threads = (Size > 2) ? Data[2] % 8 + 1 : 1;
    cfg.g_profile = (Size > 3) ? Data[3] % 4 : 0;
    cfg.g_bit_depth = VPX_BITS_8;
    cfg.g_input_bit_depth = VPX_BITS_8;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = (Size > 4) ? Data[4] + 1 : 30;
    cfg.rc_end_usage = VPX_VBR;
    cfg.rc_target_bitrate = (Size > 5) ? (Data[5] * 100 + 100) : 100;
    cfg.kf_mode = VPX_KF_AUTO;
    cfg.kf_min_dist = (Size > 6) ? Data[6] * 10 : 120;
    cfg.kf_max_dist = (Size > 7) ? Data[7] * 10 + 120 : 240;

    // Initialize encoder context
    vpx_codec_iface_t *iface = vpx_codec_vp8_cx();
    if (iface == NULL) {
        return 0;
    }

    vpx_codec_err_t err = vpx_codec_enc_init_ver(&ctx, iface, &cfg, 0, VPX_ENCODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        return 0;
    }

    // Allocate image data
    img_data_size = (cfg.g_w * cfg.g_h * 3) / 2;
    img_data = (uint8_t *)malloc(img_data_size);
    if (img_data == NULL) {
        vpx_codec_destroy(&ctx);
        ctx.name = NULL;
        return 0;
    }

    // Initialize image
    vpx_image_t *img_ptr = vpx_img_alloc(NULL, VPX_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
    if (img_ptr == NULL) {
        free(img_data);
        img_data = NULL;
        vpx_codec_destroy(&ctx);
        ctx.name = NULL;
        return 0;
    }
    
    // Copy the allocated image structure to our static variable
    img = *img_ptr;
    // Note: vpx_img_alloc allocates memory for planes. We must not free img_ptr 
    // here because we copied the data, but the underlying memory ownership 
    // is tricky. For fuzzing, we will manually manage the planes if needed,
    // but standard practice is to free the image struct. 
    // However, since we copied the struct, the planes point to memory owned by img_ptr.
    // To be safe and avoid double free or use-after-free, we will use img_ptr directly
    // in the loop and free it at the end, instead of copying to static 'img'.
    
    // Re-assigning strategy: Use img_ptr directly in the loop, free it later.
    // We will not use the static 'img' variable for the encoding loop to avoid ownership issues.
    // Instead, we will just use 'img_ptr'.
    
    // Copy fuzzing data to image planes
    size_t data_to_copy = (Size < img_data_size) ? Size : img_data_size;
    if (img_ptr->planes[0] != NULL) {
        memcpy(img_ptr->planes[0], Data, data_to_copy);
        if (img_data_size > data_to_copy) {
            memset(img_ptr->planes[0] + data_to_copy, 0, img_data_size - data_to_copy);
        }
    }

    // Encode frames
    vpx_codec_pts_t pts = 0;
    unsigned long duration = 1;
    vpx_enc_frame_flags_t flags = 0;
    vpx_enc_deadline_t deadline = 0;

    for (size_t i = 0; i < Size / 10; ++i) {
        err = vpx_codec_encode(&ctx, img_ptr, pts, duration, flags, deadline);
        if (err != VPX_CODEC_OK) {
            const char *err_str = vpx_codec_err_to_string(err);
            (void)err_str; // Suppress unused warning
            break;
        }

        // Retrieve encoded data
        vpx_codec_iter_t iter = NULL;
        const vpx_codec_cx_pkt_t *pkt;
        while ((pkt = vpx_codec_get_cx_data(&ctx, &iter)) != NULL) {
            if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
                (void)pkt->data.frame.buf;
                (void)pkt->data.frame.sz;
            }
        }

        pts += duration;
    }

    // Flush the encoder
    err = vpx_codec_encode(&ctx, NULL, pts, duration, flags, deadline);
    if (err != VPX_CODEC_OK) {
        const char *err_str = vpx_codec_err_to_string(err);
        (void)err_str; // Suppress unused warning
    }

    // Retrieve remaining encoded data
    vpx_codec_iter_t iter = NULL;
    const vpx_codec_cx_pkt_t *pkt;
    while ((pkt = vpx_codec_get_cx_data(&ctx, &iter)) != NULL) {
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            (void)pkt->data.frame.buf;
            (void)pkt->data.frame.sz;
        }
    }

    // Try to get global headers (VP8 returns NULL, but we call it to test the API)
    vpx_fixed_buf_t *global_headers = vpx_codec_get_global_headers(&ctx);
    if (global_headers != NULL) {
        (void)global_headers->buf;
        (void)global_headers->sz;
    }

    // Reconfigure encoder
    cfg.g_threads = (Size > 8) ? Data[8] % 8 + 1 : 1;
    err = vpx_codec_enc_config_set(&ctx, &cfg);
    if (err != VPX_CODEC_OK) {
        const char *err_str = vpx_codec_err_to_string(err);
        (void)err_str; // Suppress unused warning
    }

    // Cleanup image
    vpx_img_free(img_ptr);
    
    cleanup();
    return 0;
}