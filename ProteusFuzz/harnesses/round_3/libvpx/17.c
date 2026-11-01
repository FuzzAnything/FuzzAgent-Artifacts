// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_iface_name at vpx_codec.c:30:13 in vpx_codec.h
// vpx_codec_iface_name at vpx_codec.c:30:13 in vpx_codec.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_err_to_string at vpx_codec.c:34:13 in vpx_codec.h
// vpx_codec_err_to_string at vpx_codec.c:34:13 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>
#include <vpx/vpx_image.h>

static void safe_img_free(vpx_image_t *img) {
    if (img) {
        vpx_img_free(img);
    }
}

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // 1. Prepare environment and invoke vpx_codec_iface_name
    // First, get the interface pointer using vpx_codec_vp8_cx
    vpx_codec_iface_t *iface = vpx_codec_vp8_cx();
    
    // Invoke vpx_codec_iface_name with the valid interface
    const char *name = vpx_codec_iface_name(iface);
    (void)name; // Use the result to avoid unused variable warnings

    // Also test with NULL as per description
    const char *null_name = vpx_codec_iface_name(NULL);
    (void)null_name;

    if (!iface) {
        return 0;
    }

    // 2. Invoke vpx_codec_enc_config_default
    vpx_codec_enc_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    
    vpx_codec_err_t err = vpx_codec_enc_config_default(iface, &cfg, 0);
    
    // 3. Invoke vpx_codec_err_to_string
    const char *err_str = vpx_codec_err_to_string(err);
    (void)err_str;

    // Also test with a specific error code to explore more states
    const char *mem_err_str = vpx_codec_err_to_string(VPX_CODEC_MEM_ERROR);
    (void)mem_err_str;

    // 4. Invoke vpx_codec_vp8_cx again (as per requested order)
    vpx_codec_iface_t *iface2 = vpx_codec_vp8_cx();
    (void)iface2;

    // 5. Invoke vpx_img_alloc
    // We need to extract parameters from fuzz data safely
    unsigned int width = 0;
    unsigned int height = 0;
    unsigned int align = 1;
    vpx_img_fmt_t fmt = VPX_IMG_FMT_I420;

    if (Size >= 8) {
        // Read width and height from input, ensuring they are within limits
        // Limit: 2^27 = 134217728
        uint32_t w_raw = *(const uint32_t *)(Data);
        uint32_t h_raw = *(const uint32_t *)(Data + 4);
        
        width = (w_raw % 134217728) + 1; // Ensure non-zero and within limit
        height = (h_raw % 134217728) + 1;

        if (Size >= 12) {
            uint32_t a_raw = *(const uint32_t *)(Data + 8);
            // Limit: 65536
            align = (a_raw % 65536) + 1;
        }
        
        if (Size >= 16) {
            uint32_t f_raw = *(const uint32_t *)(Data + 12);
            // Map to valid enum values roughly
            fmt = (vpx_img_fmt_t)(f_raw % 10);
            // Ensure it's a valid format from the enum if possible, 
            // though passing invalid might be part of fuzzing.
            // Let's clamp to known valid ones for stability if desired, 
            // but for fuzzing, raw casting is often okay if the lib handles it.
            // Here we just ensure it's within the enum range roughly.
        }
    } else {
        // Default values if input is too small
        width = 64;
        height = 64;
        align = 1;
        fmt = VPX_IMG_FMT_I420;
    }

    // Allocate image
    vpx_image_t *img = vpx_img_alloc(NULL, fmt, width, height, align);

    // Cleanup
    safe_img_free(img);

    return 0;
}