// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_enc_config_set at vpx_encoder.c:348:17 in vpx_encoder.h
// vpx_codec_set_cx_data_buf at vpx_encoder.c:294:17 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <vpx/vpx_encoder.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>
#include <vpx/vpx_image.h>

static void free_image(vpx_image_t *img) {
    if (img && img->img_data) {
        vpx_img_free(img);
    }
}

static int create_test_image(vpx_image_t *img, unsigned int w, unsigned int h, const uint8_t *data, size_t size) {
    if (vpx_img_alloc(img, VPX_IMG_FMT_I420, w, h, 1) != 0) {
        return 0;
    }

    if (data && size > 0) {
        size_t plane_size = w * h;
        size_t total_needed = plane_size * 3; // Y, U, V planes for I420
        size_t copy_size = (size < total_needed) ? size : total_needed;
        
        if (img->planes[VPX_PLANE_Y]) {
            memcpy(img->planes[VPX_PLANE_Y], data, plane_size);
        }
        if (img->planes[VPX_PLANE_U] && copy_size > plane_size) {
            memcpy(img->planes[VPX_PLANE_U], data + plane_size, plane_size);
        }
        if (img->planes[VPX_PLANE_V] && copy_size > plane_size * 2) {
            memcpy(img->planes[VPX_PLANE_V], data + plane_size * 2, plane_size);
        }
    } else {
        // Fill with zeros if no data or insufficient data
        memset(img->planes[VPX_PLANE_Y], 0, w * h);
        memset(img->planes[VPX_PLANE_U], 0, w * h / 4);
        memset(img->planes[VPX_PLANE_V], 0, w * h / 4);
    }
    return 1;
}

int LLVMFuzzerTestOneInput_129(const uint8_t *Data, size_t Size) {
    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_iter_t iter = NULL;
    vpx_codec_cx_pkt_t *pkt;
    vpx_image_t img;
    vpx_image_t *img_ptr = NULL;
    int ret;
    int width, height;
    const vpx_codec_iface_t *iface;

    // Initialize image
    memset(&img, 0, sizeof(img));

    // Get encoder interface
    iface = vpx_codec_vp8_cx();
    if (!iface) {
        return 0;
    }

    // Initialize config
    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Fuzz width and height from input if possible, otherwise default
    if (Size >= 4) {
        width = Data[0] | (Data[1] << 8);
        height = Data[2] | (Data[3] << 8);
        if (width == 0) width = 16;
        if (height == 0) height = 16;
        // Ensure alignment
        width = (width + 15) & ~15;
        height = (height + 15) & ~15;
    } else {
        width = 16;
        height = 16;
    }

    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;
    cfg.rc_min_quantizer = 2;
    cfg.rc_max_quantizer = 63;
    cfg.kf_mode = VPX_KF_DISABLED;
    cfg.g_threads = 1;
    cfg.g_profile = 0;
    cfg.g_bit_depth = VPX_BITS_8;
    cfg.g_input_bit_depth = 8;
    cfg.rc_end_usage = VPX_VBR;

    // Initialize encoder
    if (vpx_codec_enc_init(&ctx, iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Create image
    if (!create_test_image(&img, width, height, Data, Size)) {
        vpx_codec_destroy(&ctx);
        return 0;
    }
    img_ptr = &img;

    // Fuzz encoding parameters
    vpx_codec_pts_t pts = 0;
    unsigned long duration = 1;
    vpx_enc_frame_flags_t flags = 0;
    vpx_enc_deadline_t deadline = 0;

    if (Size >= 8) {
        pts = (vpx_codec_pts_t)(Data[4] | (Data[5] << 8) | (Data[6] << 16) | (Data[7] << 24));
        duration = Data[8] % 100 + 1; // Avoid 0 duration
        flags = Data[9] % 10;
        deadline = (Size > 10) ? (Data[10] * 1000) : 0;
    }

    // Test vpx_codec_enc_config_set with fuzzed config
    if (Size > 12) {
        vpx_codec_enc_cfg_t fuzz_cfg = cfg;
        fuzz_cfg.g_w = (Data[12] % 1024) + 16;
        fuzz_cfg.g_h = (Data[13] % 1024) + 16;
        fuzz_cfg.rc_target_bitrate = (Data[14] % 1000) + 1;
        fuzz_cfg.rc_min_quantizer = Data[15] % 64;
        fuzz_cfg.rc_max_quantizer = (Data[16] % 64) + fuzz_cfg.rc_min_quantizer;
        
        vpx_codec_enc_config_set(&ctx, &fuzz_cfg);
    }

    // Test vpx_codec_set_cx_data_buf
    if (Size > 17) {
        uint8_t *custom_buf = malloc(65536);
        if (custom_buf) {
            vpx_fixed_buf_t buf;
            buf.buf = custom_buf;
            buf.sz = 65536;
            unsigned int pad_before = Data[17] % 100;
            unsigned int pad_after = Data[18] % 100;
            vpx_codec_set_cx_data_buf(&ctx, &buf, pad_before, pad_after);
            free(custom_buf);
        }
    }

    // Encode frame
    ret = vpx_codec_encode(&ctx, img_ptr, pts, duration, flags, deadline);
    
    // Retrieve packets
    while ((pkt = vpx_codec_get_cx_data(&ctx, &iter)) != NULL) {
        // Just iterate to consume data
        if (pkt->kind == VPX_CODEC_CX_FRAME_PKT) {
            // Do nothing with the frame data, just ensure no crash
            (void)pkt->data.frame.sz;
        }
    }

    // Flush encoder
    ret = vpx_codec_encode(&ctx, NULL, pts, duration, flags, deadline);
    
    // Retrieve remaining packets
    while ((pkt = vpx_codec_get_cx_data(&ctx, &iter)) != NULL) {
        // Consume
    }

    // Cleanup
    free_image(&img);
    vpx_codec_destroy(&ctx);

    return 0;
}