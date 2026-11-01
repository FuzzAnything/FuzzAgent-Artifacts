// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>

static int create_test_image(vpx_image_t *img, unsigned int w, unsigned int h, const uint8_t *data, size_t size) {
    if (!img || !data || size == 0) return 0;
    
    vpx_img_fmt_t fmt = VPX_IMG_FMT_I420;
    unsigned int bit_depth = 8;
    
    // Fuzz the format and bit depth based on input data
    if (size > 0) fmt = (vpx_img_fmt_t)(data[0] % 10);
    if (size > 1) bit_depth = (data[1] % 5 == 0) ? 8 : (data[1] % 3 == 0) ? 10 : 12;
    
    // Clamp dimensions
    if (w == 0) w = 16;
    if (h == 0) h = 16;
    if (w > 4096) w = 4096;
    if (h > 4096) h = 4096;
    
    vpx_img_alloc(img, fmt, w, h, bit_depth);
    if (!img->planes[0]) return 0;
    
    // Fill image data with fuzzed input
    size_t plane_size = (size_t)img->stride[0] * img->h;
    size_t total_size = plane_size * 3; // Y, U, V for I420
    
    if (size < total_size) {
        // Repeat data if input is too small
        for (size_t i = 0; i < total_size; i++) {
            img->planes[0][i] = data[i % size];
        }
    } else {
        memcpy(img->planes[0], data, total_size);
    }
    
    return 1;
}

int LLVMFuzzerTestOneInput_63(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_iface_t *codec_iface;
    vpx_codec_err_t res;
    
    // Initialize configuration
    memset(&cfg, 0, sizeof(cfg));
    
    // Fuzz configuration parameters with bounds checking
    cfg.g_w = (Size > 0) ? ((Data[0] % 100) + 16) : 32;
    cfg.g_h = (Size > 1) ? ((Data[1] % 100) + 16) : 32;
    cfg.g_threads = (Size > 2) ? ((Data[2] % 4) + 1) : 1;
    cfg.g_profile = (Size > 3) ? (Data[3] % 4) : 0;
    cfg.g_bit_depth = (Size > 4) ? ((Data[4] % 3 == 0) ? VPX_BITS_8 : (Data[4] % 3 == 1) ? VPX_BITS_10 : VPX_BITS_12) : VPX_BITS_8;
    cfg.g_input_bit_depth = cfg.g_bit_depth;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = (Size > 5) ? ((Data[5] % 100) + 1) : 1;
    cfg.rc_end_usage = (Size > 6) ? (Data[6] % 4) : 0;
    cfg.rc_target_bitrate = (Size > 7) ? ((Data[7] % 1000) + 100) : 100;
    cfg.kf_mode = (Size > 8) ? (Data[8] % 3) : 0;
    cfg.kf_min_dist = (Size > 9) ? ((Data[9] % 100) + 1) : 1;
    cfg.kf_max_dist = (Size > 10) ? ((Data[10] % 100) + 1) : 1;
    
    // Get encoder interface (VP9)
    codec_iface = vpx_codec_vp9_cx();
    if (!codec_iface) return 0;
    
    // Initialize encoder context
    res = vpx_codec_enc_init(&ctx, codec_iface, &cfg, 0);
    if (res != VPX_CODEC_OK) {
        return 0;
    }
    
    vpx_image_t img;
    memset(&img, 0, sizeof(img));
    
    // Fuzz image creation
    unsigned int w = cfg.g_w;
    unsigned int h = cfg.g_h;
    
    if (!create_test_image(&img, w, h, Data, Size)) {
        vpx_codec_destroy(&ctx);
        return 0;
    }
    
    // Fuzz encoding parameters
    vpx_codec_pts_t pts = (Size > 11) ? (int64_t)Data[11] : 0;
    unsigned long duration = (Size > 12) ? (unsigned long)Data[12] : 1;
    vpx_enc_frame_flags_t flags = (Size > 13) ? (vpx_enc_frame_flags_t)Data[13] : 0;
    vpx_enc_deadline_t deadline = (Size > 14) ? (vpx_enc_deadline_t)Data[14] : 0;
    
    // Encode frame
    res = vpx_codec_encode(&ctx, &img, pts, duration, flags, deadline);
    
    // Get encoded data
    const vpx_codec_cx_pkt_t *pkt;
    while ((pkt = vpx_codec_get_cx_data(&ctx, NULL)) != NULL) {
        // Process packet if needed
        (void)pkt; 
    }
    
    // Flush encoder
    res = vpx_codec_encode(&ctx, NULL, pts + duration, duration, flags, deadline);
    
    // Get remaining encoded data
    while ((pkt = vpx_codec_get_cx_data(&ctx, NULL)) != NULL) {
        (void)pkt;
    }
    
    // Test vpx_codec_control_ with various control IDs
    // Using the raw function vpx_codec_control_ to avoid macro expansion issues with hardcoded IDs
    if (Size > 15) {
        int ctrl_id = Data[15] % 100;
        res = vpx_codec_control_(&ctx, ctrl_id);
    }
    
    // Test VP9E_SET_TILE_COLUMNS (ID 1001)
    if (Size > 16) {
        int tile_cols = Data[16] % 16;
        vpx_codec_control_(&ctx, 1001, tile_cols);
    }
    
    // Test VP9E_SET_SVC (ID 1002)
    if (Size > 17) {
        int svc_value = Data[17] % 2;
        vpx_codec_control_(&ctx, 1002, svc_value);
    }
    
    // Test VP9E_SET_KEY_FRAME_FILTERING (ID 1003)
    if (Size > 18) {
        int kf_filter = Data[18] % 2;
        vpx_codec_control_(&ctx, 1003, kf_filter);
    }
    
    // Cleanup
    vpx_img_free(&img);
    vpx_codec_destroy(&ctx);
    
    return 0;
}