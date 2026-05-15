// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_img_wrap at vpx_image.c:168:14 in vpx_image.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_encoder.h"
#include "vpx/vpx_image.h"
#include "vpx/vpx_codec.h"
#include "vpx/vp8cx.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static vpx_codec_ctx_t codec_ctx;
static vpx_codec_enc_cfg_t cfg;
static vpx_image_t img;
static vpx_codec_iface_t *iface;

int LLVMFuzzerTestOneInput_2(const uint8_t *Data, size_t Size) {
    // Initialize variables
    vpx_codec_err_t res;
    
    // Call vpx_codec_vp8_cx first to get interface pointer
    iface = vpx_codec_vp8_cx();
    
    // Call vpx_codec_enc_config_default
    res = vpx_codec_enc_config_default(iface, &cfg, 0);
    if (res != VPX_CODEC_OK) {
        // Clean up and return
        return 0;
    }
    
    // Call vpx_codec_vp8_cx again (as per order)
    iface = vpx_codec_vp8_cx();
    
    // Initialize encoder with configuration
    res = vpx_codec_enc_init(&codec_ctx, iface, &cfg, 0);
    if (res != VPX_CODEC_OK) {
        // Clean up and return
        vpx_codec_destroy(&codec_ctx);
        return 0;
    }
    
    // Prepare image parameters from fuzzer input
    // Use first few bytes to determine image parameters
    if (Size < 4) {
        vpx_codec_destroy(&codec_ctx);
        return 0;
    }
    
    // Extract parameters from fuzzer data
    unsigned int width = (Data[0] << 8) | Data[1];
    unsigned int height = (Data[2] << 8) | Data[3];
    
    // Limit dimensions to reasonable values to avoid excessive memory
    width = width % 1024 + 1;
    height = height % 1024 + 1;
    
    // Choose image format based on fuzzer input
    vpx_img_fmt_t fmt = VPX_IMG_FMT_I420;
    if (Size > 4) {
        switch (Data[4] % 4) {
            case 0: fmt = VPX_IMG_FMT_I420; break;
            case 1: fmt = VPX_IMG_FMT_YV12; break;
            case 2: fmt = VPX_IMG_FMT_I422; break;
            case 3: fmt = VPX_IMG_FMT_I444; break;
        }
    }
    
    // Calculate stride alignment (power of 2, up to 32)
    unsigned int stride_align = 1 << (Data[5] % 6);  // 1, 2, 4, 8, 16, 32
    
    // Calculate required buffer size for image
    // Simplified calculation based on format
    size_t img_size = 0;
    switch (fmt) {
        case VPX_IMG_FMT_I420:
        case VPX_IMG_FMT_YV12:
            img_size = width * height * 3 / 2;
            break;
        case VPX_IMG_FMT_I422:
            img_size = width * height * 2;
            break;
        case VPX_IMG_FMT_I444:
            img_size = width * height * 3;
            break;
        default:
            img_size = width * height * 3 / 2;
            break;
    }
    
    // Allocate image buffer
    unsigned char *img_data = (unsigned char *)malloc(img_size);
    if (!img_data) {
        vpx_codec_destroy(&codec_ctx);
        return 0;
    }
    
    // Fill image buffer with fuzzer data (or zeros if not enough)
    size_t copy_size = Size - 6 > img_size ? img_size : (Size > 6 ? Size - 6 : 0);
    if (copy_size > 0) {
        memcpy(img_data, Data + 6, copy_size);
    } else {
        memset(img_data, 0, img_size);
    }
    
    // Call vpx_img_wrap
    vpx_image_t *wrapped_img = vpx_img_wrap(&img, fmt, width, height, stride_align, img_data);
    
    if (wrapped_img) {
        // Set color space and range
        wrapped_img->cs = VPX_CS_BT_601;
        wrapped_img->range = VPX_CR_STUDIO_RANGE;
        
        // Try to encode the image
        // Note: We don't check the return value to explore more states
        vpx_codec_encode(&codec_ctx, wrapped_img, 0, 1, 0, VPX_DL_REALTIME);
    }
    
    // Clean up
    free(img_data);
    
    // Call vpx_codec_destroy
    vpx_codec_destroy(&codec_ctx);
    
    return 0;
}