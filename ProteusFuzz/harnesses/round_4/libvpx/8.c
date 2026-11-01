// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_iface_name at vpx_codec.c:30:13 in vpx_codec.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_err_to_string at vpx_codec.c:34:13 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_err_to_string at vpx_codec.c:34:13 in vpx_codec.h
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
#include <vpx/vpx_encoder.h>
#include <vpx/vpx_image.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

static void safe_free_image(vpx_image_t *img) {
    if (img) {
        vpx_img_free(img);
    }
}

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // 1. Prepare the environment
    // Get the VP8 encoder interface
    vpx_codec_iface_t *iface = vpx_codec_vp8_cx();
    if (!iface) {
        return 0;
    }

    // Invoke vpx_codec_iface_name to get the interface name
    const char *iface_name = vpx_codec_iface_name(iface);
    (void)iface_name; // Use the result to avoid unused variable warning

    // Initialize encoder configuration
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_err_t err = vpx_codec_enc_config_default(iface, &cfg, 0);
    if (err != VPX_CODEC_OK) {
        // Invoke error string function on failure
        const char *err_str = vpx_codec_err_to_string(err);
        (void)err_str;
        return 0;
    }

    // Re-invoke vpx_codec_vp8_cx as per the required order
    vpx_codec_iface_t *iface2 = vpx_codec_vp8_cx();
    if (!iface2) {
        return 0;
    }

    // Invoke vpx_codec_err_to_string with a known error code to explore state
    const char *known_err_str = vpx_codec_err_to_string(VPX_CODEC_MEM_ERROR);
    (void)known_err_str;

    // Invoke vpx_codec_err_to_string with an invalid/error code if possible
    // Using a large value to trigger "Unrecognized error code"
    const char *unknown_err_str = vpx_codec_err_to_string((vpx_codec_err_t)-1);
    (void)unknown_err_str;

    // Prepare image allocation parameters from fuzz data
    // Ensure dimensions are within limits (0x08000000) and alignment <= 65536
    unsigned int width = 0;
    unsigned int height = 0;
    unsigned int align = 16; // Default alignment
    vpx_img_fmt_t fmt = VPX_IMG_FMT_I420;

    if (Size >= 8) {
        width = *(const uint32_t *)(Data);
        height = *(const uint32_t *)(Data + 4);
    } else if (Size >= 4) {
        width = *(const uint32_t *)(Data);
        height = 64; // Default height
    } else {
        width = 64;
        height = 64;
    }

    // Clamp dimensions to valid range
    if (width > 0x08000000) width = 0x08000000;
    if (height > 0x08000000) height = 0x08000000;
    if (width == 0 || height == 0) {
        width = 64;
        height = 64;
    }

    if (Size >= 12) {
        align = *(const uint32_t *)(Data + 8);
        if (align > 65536) align = 65536;
        if (align == 0) align = 16;
    }

    // Determine format from data if possible, otherwise default
    if (Size >= 16) {
        int fmt_idx = *(const uint32_t *)(Data + 12) % 10;
        switch (fmt_idx) {
            case 0: fmt = VPX_IMG_FMT_YV12; break;
            case 1: fmt = VPX_IMG_FMT_I420; break;
            case 2: fmt = VPX_IMG_FMT_I422; break;
            case 3: fmt = VPX_IMG_FMT_I444; break;
            case 4: fmt = VPX_IMG_FMT_NV12; break;
            case 5: fmt = VPX_IMG_FMT_I42016; break;
            case 6: fmt = VPX_IMG_FMT_I42216; break;
            case 7: fmt = VPX_IMG_FMT_I44416; break;
            case 8: fmt = VPX_IMG_FMT_I440; break;
            case 9: fmt = VPX_IMG_FMT_I44016; break;
            default: fmt = VPX_IMG_FMT_I420; break;
        }
    }

    // Allocate image
    vpx_image_t *img = vpx_img_alloc(NULL, fmt, width, height, align);
    
    // Cleanup
    safe_free_image(img);

    return 0;
}