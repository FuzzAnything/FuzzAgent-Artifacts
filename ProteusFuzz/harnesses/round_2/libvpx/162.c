// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_img_flip at vpx_image.c:229:6 in vpx_image.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_img_wrap at vpx_image.c:168:14 in vpx_image.h
// vpx_img_flip at vpx_image.c:229:6 in vpx_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <vpx/vpx_image.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>
#include <vpx/vpx_encoder.h>
#include <vpx/vp8.h>

static int extract_uint32(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + 4 > size) return 0;
    uint32_t val = data[*offset] | (data[*offset + 1] << 8) |
                   (data[*offset + 2] << 16) | (data[*offset + 3] << 24);
    *offset += 4;
    return val;
}

static int extract_uint16(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + 2 > size) return 0;
    uint16_t val = data[*offset] | (data[*offset + 1] << 8);
    *offset += 2;
    return val;
}

int LLVMFuzzerTestOneInput_162(const uint8_t *Data, size_t Size) {
    if (Size < 16) return 0;

    size_t offset = 0;
    uint32_t width = extract_uint32(Data, Size, &offset);
    uint32_t height = extract_uint32(Data, Size, &offset);
    uint32_t align = extract_uint32(Data, Size, &offset);
    uint32_t fmt_val = extract_uint32(Data, Size, &offset);

    // Sanity checks to prevent crashes from huge allocations or invalid params
    if (width == 0 || height == 0 || width > 0x08000000 || height > 0x08000000) {
        return 0;
    }
    if (align == 0 || align > 65536) {
        align = 16; // Default safe alignment
    }

    vpx_img_fmt_t fmt = (vpx_img_fmt_t)fmt_val;
    // Only test valid known formats to avoid undefined behavior
    if (fmt == VPX_IMG_FMT_NONE || fmt > VPX_IMG_FMT_I44016) {
        fmt = VPX_IMG_FMT_I420;
    }

    // Test vpx_img_alloc
    vpx_image_t *img_alloc = vpx_img_alloc(NULL, fmt, width, height, align);
    if (img_alloc) {
        // Fill with fuzz data if possible
        if (img_alloc->planes[0]) {
            size_t plane_size = (size_t)img_alloc->h * (size_t)img_alloc->stride[0];
            if (Size > offset && plane_size > 0) {
                size_t copy_len = (Size - offset < plane_size) ? (Size - offset) : plane_size;
                memcpy(img_alloc->planes[0], Data + offset, copy_len);
            }
        }
        
        // Test vpx_img_flip
        vpx_img_flip(img_alloc);
        
        // Cleanup
        vpx_img_free(img_alloc);
    }

    // Test vpx_img_wrap with external buffer
    // Calculate required size roughly (simplified for fuzzing)
    uint64_t approx_size = (uint64_t)width * height * 2; // Conservative estimate
    unsigned char *external_buf = NULL;
    if (approx_size < (uint64_t)SIZE_MAX / 2) {
        external_buf = (unsigned char *)malloc(approx_size);
    }
    
    if (external_buf) {
        vpx_image_t img_wrap;
        vpx_image_t *img_res = vpx_img_wrap(&img_wrap, fmt, width, height, align, external_buf);
        
        if (img_res) {
            // Fill buffer
            if (img_res->planes[0]) {
                size_t plane_size = (size_t)img_res->h * (size_t)img_res->stride[0];
                if (Size > offset && plane_size > 0) {
                    size_t copy_len = (Size - offset < plane_size) ? (Size - offset) : plane_size;
                    memcpy(img_res->planes[0], Data + offset, copy_len);
                }
            }
            // Test flip on wrapped image
            vpx_img_flip(img_res);
        }
        free(external_buf);
    }

    // Test vpx_codec_encode (requires encoder context)
    // We attempt to initialize an encoder context to test the encode function
    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_err_t res;

    res = vpx_codec_enc_config_default(NULL, &cfg, 0);
    if (res != VPX_CODEC_OK) {
        return 0;
    }

    // Apply fuzzed parameters to config if reasonable
    cfg.g_w = width;
    cfg.g_h = height;
    cfg.rc_target_bitrate = extract_uint32(Data, Size, &offset);
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30; // Default 30fps

    // Initialize encoder
    res = vpx_codec_enc_init(&ctx, NULL, &cfg, 0);
    if (res != VPX_CODEC_OK) {
        return 0;
    }

    // Allocate a frame for encoding
    vpx_image_t *encode_img = vpx_img_alloc(NULL, fmt, width, height, align);
    if (encode_img) {
        // Fill with data
        if (encode_img->planes[0]) {
            size_t plane_size = (size_t)encode_img->h * (size_t)encode_img->stride[0];
            if (Size > offset && plane_size > 0) {
                size_t copy_len = (Size - offset < plane_size) ? (Size - offset) : plane_size;
                memcpy(encode_img->planes[0], Data + offset, copy_len);
            }
        }

        // Extract PTS and duration
        int64_t pts = 0;
        unsigned long duration = 1;
        if (offset + 8 <= Size) {
            pts = *(int64_t*)(Data + offset);
            offset += 8;
        }
        if (offset + 4 <= Size) {
            duration = extract_uint32(Data, Size, &offset);
        }
        
        // Ensure strictly increasing PTS logic is not violated in a single run context
        // but for fuzzing we just pass the value.
        
        vpx_enc_frame_flags_t flags = 0;
        if (offset + 4 <= Size) {
            flags = (vpx_enc_frame_flags_t)extract_uint32(Data, Size, &offset);
        }
        
        vpx_enc_deadline_t deadline = 0;
        if (offset + 4 <= Size) {
            deadline = (vpx_enc_deadline_t)extract_uint32(Data, Size, &offset);
        }

        // Call encode
        vpx_codec_encode(&ctx, encode_img, pts, duration, flags, deadline);

        // Flush
        vpx_codec_encode(&ctx, NULL, pts, duration, flags, deadline);

        vpx_img_free(encode_img);
    }

    // Cleanup encoder
    vpx_codec_destroy(&ctx);

    return 0;
}