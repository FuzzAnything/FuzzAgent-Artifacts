// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_plane_height at aom_image.c:320:5 in aom_image.h
// aom_img_plane_height at aom_image.c:320:5 in aom_image.h
// aom_img_plane_height at aom_image.c:320:5 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
// aom_img_plane_height at aom_image.c:320:5 in aom_image.h
// aom_img_plane_height at aom_image.c:320:5 in aom_image.h
// aom_img_plane_height at aom_image.c:320:5 in aom_image.h
// aom_img_plane_height at aom_image.c:320:5 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_img_plane_height at aom_image.c:320:5 in aom_image.h
// aom_img_plane_height at aom_image.c:320:5 in aom_image.h
// aom_img_plane_height at aom_image.c:320:5 in aom_image.h
// aom_img_plane_height at aom_image.c:320:5 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_wrap at aom_image.c:216:14 in aom_image.h
// aom_img_plane_height at aom_image.c:320:5 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aomdx.h"
#include "aom/aomcx.h"
#include "aom/aom_codec.h"
#include "aom/aom_image.h"
#include "aom/aom_integer.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_frame_buffer.h"

/* Helper to clamp values to valid ranges for fuzzing */
static unsigned int clamp_width(unsigned int w) {
    if (w == 0 || w > 0x08000000) return 16;
    return w;
}

static unsigned int clamp_height(unsigned int h) {
    if (h == 0 || h > 0x08000000) return 16;
    return h;
}

static unsigned int clamp_align(unsigned int align) {
    if (align == 0 || align > 65536) return 16;
    return align;
}

/* Helper to safely interpret fuzz data as integers */
static unsigned int get_uint32(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + 4 > size) {
        *offset = 0;
        return 0;
    }
    uint32_t val = 0;
    memcpy(&val, data + *offset, 4);
    *offset += 4;
    return val;
}

int LLVMFuzzerTestOneInput_229(const uint8_t *Data, size_t Size) {
    if (Size < 16) return 0;

    size_t offset = 0;
    unsigned int w = clamp_width(get_uint32(Data, Size, &offset));
    unsigned int h = clamp_height(get_uint32(Data, Size, &offset));
    unsigned int align = clamp_align(get_uint32(Data, Size, &offset));
    
    /* Fuzz image format */
    aom_img_fmt_t fmt = (aom_img_fmt_t)(Data[offset] % 16);
    offset++;
    
    /* Fuzz plane index for aom_img_plane_height */
    int plane = (Data[offset] % 3);
    offset++;

    /* 1. Test aom_img_alloc */
    aom_image_t img_alloc = {0};
    aom_image_t *img_alloc_ptr = aom_img_alloc(&img_alloc, fmt, w, h, align);
    
    if (img_alloc_ptr != NULL) {
        /* Test aom_img_plane_height with allocated image */
        int h_luma = aom_img_plane_height(img_alloc_ptr, 0);
        int h_u = aom_img_plane_height(img_alloc_ptr, 1);
        int h_v = aom_img_plane_height(img_alloc_ptr, 2);
        
        /* Trigger logic with specific plane from fuzz input */
        int h_fuzz = aom_img_plane_height(img_alloc_ptr, plane);
        
        /* Cleanup allocated image */
        aom_img_free(img_alloc_ptr);
    }

    /* 2. Test aom_img_wrap with user-allocated buffer */
    /* Calculate required buffer size roughly to avoid immediate crash, 
       though aom_img_wrap validates this. We allocate a safe large buffer. */
    size_t max_size = (size_t)w * h * 4 + 4096;
    if (max_size > 1024 * 1024 * 10) max_size = 1024 * 1024 * 10; // Limit to 10MB
    
    unsigned char *user_buf = NULL;
    if (max_size > 0) {
        user_buf = (unsigned char *)malloc(max_size);
        if (user_buf) {
            memset(user_buf, 0, max_size);
        }
    }

    aom_image_t img_wrap = {0};
    if (user_buf) {
        aom_image_t *img_wrap_ptr = aom_img_wrap(&img_wrap, fmt, w, h, align, user_buf);
        
        if (img_wrap_ptr != NULL) {
            /* Test aom_img_plane_height with wrapped image */
            int h_luma_wrap = aom_img_plane_height(img_wrap_ptr, 0);
            int h_u_wrap = aom_img_plane_height(img_wrap_ptr, 1);
            int h_v_wrap = aom_img_plane_height(img_wrap_ptr, 2);
            int h_fuzz_wrap = aom_img_plane_height(img_wrap_ptr, plane);
            
            /* Cleanup wrapped image (does not free user_buf) */
            aom_img_free(img_wrap_ptr);
        }
    }
    
    if (user_buf) {
        free(user_buf);
    }

    /* 3. Test aom_codec_get_frame with a decoder context */
    /* We need to initialize a decoder to get a valid context for aom_codec_get_frame */
    const aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    /* Fixed: removed allow_highbitdepth which does not exist in this struct version */
    // cfg.allow_highbitdepth = 1; 

    if (aom_codec_dec_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    /* Feed some dummy data to the decoder to potentially generate frames */
    /* Fixed: Use aom_codec_decode directly with buffer instead of constructing a packet struct */
    if (Size > offset + 4) {
        size_t pkt_size = get_uint32(Data, Size, &offset);
        if (pkt_size > 100000) pkt_size = 100000; // Limit packet size
        
        if (pkt_size > 0 && offset + pkt_size <= Size) {
            const uint8_t *pkt_data = Data + offset;
            aom_codec_decode(&ctx, pkt_data, pkt_size, NULL);
        }
    }

    /* Iterate frames */
    aom_codec_iter_t iter = NULL;
    aom_image_t *frame;
    while ((frame = aom_codec_get_frame(&ctx, &iter)) != NULL) {
        /* Test plane height on decoded frame */
        int h_dec_luma = aom_img_plane_height(frame, 0);
        int h_dec_u = aom_img_plane_height(frame, 1);
        int h_dec_v = aom_img_plane_height(frame, 2);
        int h_dec_fuzz = aom_img_plane_height(frame, plane);
        
        /* Do not free frame, it is managed by the codec */
    }

    aom_codec_destroy(&ctx);

    /* Note: aom_codec_control_typechecked_AV1_GET_NEW_FRAME_IMAGE and 
       aom_codec_control_typechecked_AV1_COPY_NEW_FRAME_IMAGE signatures 
       were empty in the prompt, so they are not invoked. */

    return 0;
}