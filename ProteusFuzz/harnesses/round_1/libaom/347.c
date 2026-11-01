// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_get_preview_frame at aom_encoder.c:264:20 in aom_encoder.h
// aom_codec_get_global_headers at aom_encoder.c:281:18 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_img_wrap at aom_image.c:216:14 in aom_image.h
// aom_img_wrap at aom_image.c:216:14 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <aom/aom_codec.h>
#include <aom/aom_image.h>
#include <aom/aom_encoder.h>
#include <aom/aom_decoder.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom.h>
#include <aom/aom_integer.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_frame_buffer.h>

static int extract_uint32(const uint8_t *data, size_t *offset, size_t size) {
    if (*offset + 4 > size) return 0;
    uint32_t val = (uint32_t)data[*offset] |
                   ((uint32_t)data[*offset + 1] << 8) |
                   ((uint32_t)data[*offset + 2] << 16) |
                   ((uint32_t)data[*offset + 3] << 24);
    *offset += 4;
    return val;
}

int LLVMFuzzerTestOneInput_347(const uint8_t *Data, size_t Size) {
    if (Size < 16) return 0;

    size_t offset = 0;
    uint32_t width = extract_uint32(Data, &offset, Size);
    uint32_t height = extract_uint32(Data, &offset, Size);
    uint32_t align = extract_uint32(Data, &offset, Size);
    uint32_t fmt_val = extract_uint32(Data, &offset, Size);
    uint32_t flags_val = extract_uint32(Data, &offset, Size);

    // Bounds checking for dimensions and alignment
    if (width == 0 || height == 0 || width > 0x08000000 || height > 0x08000000) return 0;
    if (align == 0 || align > 65536) align = 16;
    
    // Validate format
    aom_img_fmt_t fmt = (aom_img_fmt_t)fmt_val;
    if (fmt == AOM_IMG_FMT_NONE || fmt > AOM_IMG_FMT_I44416) fmt = AOM_IMG_FMT_I420;

    // Initialize Encoder Context
    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) return 0;

    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) return 0;

    aom_image_t *img = NULL;
    unsigned char *user_buffer = NULL;
    size_t buffer_size = 0;

    // Try aom_img_alloc
    img = aom_img_alloc(NULL, fmt, width, height, align);
    if (img) {
        // Fill image with fuzz data if possible
        if (Size > offset + img->sz) {
            memcpy(img->planes[0], Data + offset, img->sz);
        }
    }

    // Try aom_img_wrap (requires calculating size first)
    // We use a dummy wrap to get size requirements, then allocate
    aom_image_t wrap_img;
    aom_img_wrap(&wrap_img, fmt, width, height, align, NULL);
    
    // Calculate required size roughly based on stride
    if (wrap_img.stride[0] > 0 && wrap_img.h > 0) {
        size_t y_size = (size_t)wrap_img.stride[0] * wrap_img.h;
        size_t u_size = (size_t)wrap_img.stride[1] * wrap_img.h / 2; // Approx for 4:2:0
        size_t v_size = (size_t)wrap_img.stride[2] * wrap_img.h / 2;
        buffer_size = y_size + u_size + v_size;
        
        // Cap buffer size to prevent OOM
        if (buffer_size > 1024 * 1024 * 10) buffer_size = 1024 * 1024 * 10;
        
        if (buffer_size > 0) {
            user_buffer = (unsigned char *)malloc(buffer_size);
            if (user_buffer) {
                memset(user_buffer, 0, buffer_size);
                if (Size > offset) {
                    size_t copy_size = (Size - offset < buffer_size) ? (Size - offset) : buffer_size;
                    memcpy(user_buffer, Data + offset, copy_size);
                }
                aom_img_wrap(&wrap_img, fmt, width, height, align, user_buffer);
            }
        }
    }

    // Encoding loop
    aom_codec_pts_t pts = 0;
    unsigned long duration = (Size > offset + 4) ? (unsigned long)(Data[offset] | (Data[offset+1] << 8)) : 1;
    if (duration == 0) duration = 1;
    if (duration > UINT32_MAX) duration = UINT32_MAX;

    aom_enc_frame_flags_t flags = (aom_enc_frame_flags_t)flags_val;

    // Encode frames
    int frames_to_encode = 3;
    for (int i = 0; i < frames_to_encode; i++) {
        const aom_image_t *current_img = NULL;
        if (i == 0 && img) current_img = img;
        else if (i == 1 && user_buffer) current_img = &wrap_img;
        else current_img = NULL; // Flush

        aom_codec_encode(&ctx, current_img, pts, duration, flags);
        pts += duration;
    }

    // Flush
    while (1) {
        aom_codec_err_t res = aom_codec_encode(&ctx, NULL, 0, 0, 0);
        if (res != AOM_CODEC_OK) break;
        
        aom_codec_cx_pkt_t *pkt;
        while ((pkt = aom_codec_get_cx_data(&ctx, NULL)) != NULL) {
            if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                // Consume frame data
            }
            // aom_codec_destroy_pkt is not a standard function in this version, 
            // packets are typically managed by the context or just ignored in fuzzing if not used.
            // We do not call a non-existent function.
        }
        
        // Check if flush is complete
        if (aom_codec_get_cx_data(&ctx, NULL) == NULL) break;
    }

    // Test aom_codec_get_preview_frame
    const aom_image_t *preview = aom_codec_get_preview_frame(&ctx);
    if (preview) {
        // Do not modify, just check existence
        (void)preview;
    }

    // Test aom_codec_get_global_headers
    aom_fixed_buf_t *headers = aom_codec_get_global_headers(&ctx);
    if (headers) {
        if (headers->buf) {
            // Data exists, do not free here as per spec (caller owns), 
            // but in fuzzing we must clean up to avoid leaks.
            // The spec says caller owns memory.
            free(headers->buf);
        }
        free(headers);
    }

    // Cleanup
    aom_codec_destroy(&ctx);
    if (img) aom_img_free(img);
    if (user_buffer) free(user_buffer);

    return 0;
}