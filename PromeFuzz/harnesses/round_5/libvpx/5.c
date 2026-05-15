// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_img_wrap at vpx_image.c:168:14 in vpx_image.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vp8cx.h"
#include "vpx/vpx_image.h"
#include "vpx/vpx_codec.h"
#include "vpx/vpx_encoder.h"

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Initialize encoder configuration
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_iface_t *iface = vpx_codec_vp8_cx();
    
    // Call vpx_codec_enc_config_default
    vpx_codec_err_t err = vpx_codec_enc_config_default(iface, &cfg, 0);
    if (err != VPX_CODEC_OK) {
        return 0;
    }

    // Get VP8 encoder interface again
    vpx_codec_iface_t *iface2 = vpx_codec_vp8_cx();
    (void)iface2; // Use to avoid unused variable warning

    // Create image using vpx_img_wrap
    // Use fuzzer data to determine parameters
    unsigned int width = 64;
    unsigned int height = 64;
    unsigned int stride_align = 1;
    
    if (Size > 8) {
        width = (Data[0] << 8) | Data[1];
        height = (Data[2] << 8) | Data[3];
        stride_align = Data[4] % 32 + 1;
        
        // Limit dimensions to reasonable values
        if (width == 0) width = 64;
        if (height == 0) height = 64;
        if (width > 4096) width = 4096;
        if (height > 4096) height = 4096;
    }

    // Calculate buffer size for I420 format
    size_t y_plane = width * height;
    size_t uv_plane = (width / 2) * (height / 2);
    size_t total_size = y_plane + 2 * uv_plane;
    
    unsigned char *img_data = (unsigned char *)malloc(total_size);
    if (!img_data) return 0;
    
    // Fill image data with fuzzer input (wrapping if needed)
    for (size_t i = 0; i < total_size; i++) {
        img_data[i] = Data[i % Size];
    }

    vpx_image_t img;
    vpx_image_t *img_ptr = vpx_img_wrap(&img, VPX_IMG_FMT_I420, width, height, 
                                       stride_align, img_data);
    (void)img_ptr; // Use to avoid unused variable warning

    // Initialize codec context
    vpx_codec_ctx_t codec;
    cfg.g_w = width;
    cfg.g_h = height;
    
    // Use fuzzer data to modify some config parameters
    if (Size > 12) {
        cfg.rc_target_bitrate = (Data[5] << 8) | Data[6];
        cfg.rc_min_quantizer = Data[7] % 64;
        cfg.rc_max_quantizer = Data[8] % 64;
        cfg.g_error_resilient = Data[9] & 1;
    }

    err = vpx_codec_enc_init(&codec, iface, &cfg, 0);
    if (err == VPX_CODEC_OK) {
        // Try to encode a frame
        vpx_codec_encode(&codec, &img, 0, 1, 0, VPX_DL_REALTIME);
        
        // Get encoded data
        vpx_codec_iter_t iter = NULL;
        const vpx_codec_cx_pkt_t *pkt;
        while ((pkt = vpx_codec_get_cx_data(&codec, &iter)) != NULL) {
            // Process packet if needed
        }
    }

    // Clean up
    free(img_data);
    
    // Call vpx_codec_destroy
    if (err == VPX_CODEC_OK) {
        vpx_codec_destroy(&codec);
    } else {
        // Also try destroying uninitialized context
        vpx_codec_ctx_t dummy_ctx;
        memset(&dummy_ctx, 0, sizeof(dummy_ctx));
        vpx_codec_destroy(&dummy_ctx);
    }

    return 0;
}