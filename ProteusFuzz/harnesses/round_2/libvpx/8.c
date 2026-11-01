// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_iface_name at vpx_codec.c:30:13 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_iface_name at vpx_codec.c:30:13 in vpx_codec.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_err_to_string at vpx_codec.c:34:13 in vpx_codec.h
// vpx_codec_err_to_string at vpx_codec.c:34:13 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "vpx_encoder.h"
#include "vp8cx.h"
#include "vpx_codec.h"
#include "vp8dx.h"
#include "vpx_image.h"

static int parse_uint32(const uint8_t **data, size_t *size, uint32_t *val) {
    if (*size < 4) return 0;
    memcpy(val, *data, 4);
    *data += 4;
    *size -= 4;
    return 1;
}

static int parse_uint16(const uint8_t **data, size_t *size, uint16_t *val) {
    if (*size < 2) return 0;
    memcpy(val, *data, 2);
    *data += 2;
    *size -= 2;
    return 1;
}

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    const uint8_t *ptr = Data;
    size_t remaining = Size;

    // 1. vpx_codec_iface_name
    // We will call this with NULL first to test the error path, then with a valid interface later.
    const char *name_null = vpx_codec_iface_name(NULL);
    (void)name_null; // Suppress unused warning

    // 2. vpx_codec_vp8_cx
    vpx_codec_iface_t *iface = vpx_codec_vp8_cx();
    if (iface == NULL) {
        return 0; // Cannot proceed without interface
    }

    // Call vpx_codec_iface_name with valid interface
    const char *name_valid = vpx_codec_iface_name(iface);
    (void)name_valid;

    // 3. vpx_codec_enc_config_default
    vpx_codec_enc_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    
    // Parse usage from input (should be 0, but fuzzing allows variation to test error handling)
    uint32_t usage = 0;
    if (remaining >= 4) {
        parse_uint32(&ptr, &remaining, &usage);
    }

    vpx_codec_err_t err = vpx_codec_enc_config_default(iface, &cfg, usage);
    if (err != VPX_CODEC_OK) {
        // If default config fails (e.g., invalid usage), we might still want to test other paths
        // but for this sequence, we proceed to error string conversion.
    }

    // 4. vpx_codec_vp8_cx (Called again as per instruction order)
    vpx_codec_iface_t *iface2 = vpx_codec_vp8_cx();
    if (iface2 == NULL) {
        return 0;
    }

    // 5. vpx_codec_err_to_string
    // Convert the error from the config step, and also test with a hardcoded error
    const char *err_str = vpx_codec_err_to_string(err);
    (void)err_str;

    const char *err_str_mem = vpx_codec_err_to_string(VPX_CODEC_MEM_ERROR);
    (void)err_str_mem;

    // 6. vpx_img_alloc
    // Parse parameters for image allocation from fuzz input
    uint32_t width = 0;
    uint32_t height = 0;
    uint32_t align = 16; // Default alignment
    uint8_t fmt_byte = 0;

    if (remaining >= 12) {
        if (!parse_uint32(&ptr, &remaining, &width)) return 0;
        if (!parse_uint32(&ptr, &remaining, &height)) return 0;
        if (!parse_uint32(&ptr, &remaining, &align)) return 0;
    } else if (remaining >= 8) {
        if (!parse_uint32(&ptr, &remaining, &width)) return 0;
        if (!parse_uint32(&ptr, &remaining, &height)) return 0;
    } else if (remaining >= 4) {
        if (!parse_uint32(&ptr, &remaining, &width)) return 0;
        height = 16;
    } else {
        width = 16;
        height = 16;
    }

    // Sanitize dimensions to avoid huge allocations or invalid values
    // Max allowed by API is 2^27 (134217728), but we cap for fuzzing stability
    if (width > 4096) width = 4096;
    if (height > 4096) height = 4096;
    if (width == 0) width = 16;
    if (height == 0) height = 16;
    if (align > 65536) align = 65536;
    if (align == 0) align = 16;

    // Map fmt_byte to a valid enum value
    vpx_img_fmt_t fmt = VPX_IMG_FMT_I420;
    switch (fmt_byte % 4) {
        case 0: fmt = VPX_IMG_FMT_I420; break;
        case 1: fmt = VPX_IMG_FMT_YV12; break;
        case 2: fmt = VPX_IMG_FMT_NV12; break;
        case 3: fmt = VPX_IMG_FMT_I42016; break;
        default: fmt = VPX_IMG_FMT_I420; break;
    }

    // Allocate image
    vpx_image_t *img = vpx_img_alloc(NULL, fmt, width, height, align);
    
    if (img != NULL) {
        // Optional: Do some minimal interaction to ensure state is exercised
        // e.g., access planes if allocated
        if (img->planes[0] != NULL) {
            // Just a read to ensure memory is accessible
            volatile uint8_t _ = img->planes[0][0];
        }
        
        // Cleanup
        vpx_img_free(img);
    }

    return 0;
}