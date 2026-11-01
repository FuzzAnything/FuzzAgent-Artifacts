// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_iface_name at vpx_codec.c:30:13 in vpx_codec.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_err_to_string at vpx_codec.c:34:13 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_err_to_string at vpx_codec.c:34:13 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>
#include <vpx/vpx_encoder.h>
#include <vpx/vp8dx.h>
#include <vpx/vpx_image.h>

static void cleanup_image(vpx_image_t *img) {
    if (img) {
        vpx_img_free(img);
    }
}

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    // Extract parameters from fuzzing input
    uint32_t width = Data[0] | (Data[1] << 8);
    uint32_t height = Data[2] | (Data[3] << 8);
    
    // Ensure dimensions are within valid limits (max 2^27)
    if (width > 0x08000000 || height > 0x08000000) {
        width = 64;
        height = 64;
    }
    
    // Ensure minimum size
    if (width == 0) width = 16;
    if (height == 0) height = 16;

    // Step 1: Get VP8 encoder interface
    vpx_codec_iface_t *iface = vpx_codec_vp8_cx();
    if (!iface) {
        return 0;
    }

    // Step 2: Get interface name (vpx_codec_iface_name)
    const char *iface_name = vpx_codec_iface_name(iface);
    (void)iface_name; // Use for potential logging/debugging

    // Step 3: Initialize encoder config
    vpx_codec_enc_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    vpx_codec_err_t err = vpx_codec_enc_config_default(iface, &cfg, 0);
    
    if (err != VPX_CODEC_OK) {
        // Log error string
        const char *err_str = vpx_codec_err_to_string(err);
        (void)err_str;
        return 0;
    }

    // Set dimensions in config
    cfg.g_w = width;
    cfg.g_h = height;

    // Step 4: Call vpx_codec_vp8_cx again (as per sequence requirement)
    // This is redundant but follows the specified order
    vpx_codec_iface_t *iface2 = vpx_codec_vp8_cx();
    if (!iface2) {
        return 0;
    }

    // Step 5: Convert error code to string (using a known error code)
    const char *err_str = vpx_codec_err_to_string(VPX_CODEC_MEM_ERROR);
    (void)err_str;

    // Step 6: Allocate image
    vpx_image_t img;
    memset(&img, 0, sizeof(img));
    
    // Determine format from input data if available, otherwise default
    vpx_img_fmt_t fmt = VPX_IMG_FMT_I420;
    if (Size > 4) {
        int fmt_idx = Data[4] % 10; // Limit to first 10 formats roughly
        switch (fmt_idx) {
            case 0: fmt = VPX_IMG_FMT_I420; break;
            case 1: fmt = VPX_IMG_FMT_I422; break;
            case 2: fmt = VPX_IMG_FMT_I444; break;
            case 3: fmt = VPX_IMG_FMT_NV12; break;
            case 4: fmt = VPX_IMG_FMT_YV12; break;
            case 5: fmt = VPX_IMG_FMT_I42016; break;
            case 6: fmt = VPX_IMG_FMT_I42216; break;
            case 7: fmt = VPX_IMG_FMT_I44416; break;
            case 8: fmt = VPX_IMG_FMT_I440; break;
            case 9: fmt = VPX_IMG_FMT_I44016; break;
            default: fmt = VPX_IMG_FMT_I420; break;
        }
    }

    // Alignment from input, clamped to max 65536
    uint32_t align = 16;
    if (Size > 5) {
        align = Data[5];
        if (align > 65536) align = 65536;
        if (align == 0) align = 16;
    }

    vpx_image_t *img_ptr = vpx_img_alloc(&img, fmt, width, height, align);
    if (img_ptr) {
        // Optionally use the image data here for further fuzzing
        // For now, just ensure we clean up
        cleanup_image(img_ptr);
    }

    return 0;
}