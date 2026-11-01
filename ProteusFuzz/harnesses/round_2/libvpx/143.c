// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_set_cx_data_buf at vpx_encoder.c:294:17 in vpx_encoder.h
// vpx_codec_set_cx_data_buf at vpx_encoder.c:294:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_enc_config_set at vpx_encoder.c:348:17 in vpx_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include "vp8cx.h"
#include "vpx_encoder.h"

// Helper to safely consume input data
static uint32_t read_u32(const uint8_t *data, size_t *offset, size_t size) {
    if (*offset + 4 > size) return 0;
    uint32_t val = data[*offset] | (data[*offset + 1] << 8) |
                   (data[*offset + 2] << 16) | (data[*offset + 3] << 24);
    *offset += 4;
    return val;
}

static uint16_t read_u16(const uint8_t *data, size_t *offset, size_t size) {
    if (*offset + 2 > size) return 0;
    uint16_t val = data[*offset] | (data[*offset + 1] << 8);
    *offset += 2;
    return val;
}

static uint8_t read_u8(const uint8_t *data, size_t *offset, size_t size) {
    if (*offset >= size) return 0;
    uint8_t val = data[*offset];
    *offset += 1;
    return val;
}

// Fuzz driver entry point
int LLVMFuzzerTestOneInput_143(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Initialize fuzzing state
    size_t offset = 0;
    vpx_codec_ctx_t ctx = {0};
    vpx_codec_enc_cfg_t cfg = {0};
    vpx_image_t img = {0};
    vpx_fixed_buf_t custom_buf = {0};
    uint8_t *custom_buf_data = NULL;

    // 1. Prepare Environment: Initialize Encoder Context
    // Use the global interface pointer provided by vp8cx.h
    // Corrected identifier: vpx_codec_vp8_cx_algo
    const vpx_codec_iface_t *iface = &vpx_codec_vp8_cx_algo;
    
    if (iface == NULL) {
        return 0;
    }

    // Initialize configuration with defaults
    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Fuzz configuration parameters
    if (offset < Size) cfg.g_w = read_u16(Data, &offset, Size);
    if (offset < Size) cfg.g_h = read_u16(Data, &offset, Size);
    if (offset < Size) cfg.g_threads = read_u8(Data, &offset, Size);
    if (offset < Size) cfg.g_profile = read_u8(Data, &offset, Size);
    if (offset < Size) cfg.g_bit_depth = (vpx_bit_depth_t)read_u8(Data, &offset, Size);
    if (offset < Size) cfg.g_input_bit_depth = read_u8(Data, &offset, Size);
    
    // Ensure dimensions are valid for allocation
    if (cfg.g_w == 0 || cfg.g_h == 0) {
        cfg.g_w = 64;
        cfg.g_h = 64;
    }
    if (cfg.g_w > 4096) cfg.g_w = 4096;
    if (cfg.g_h > 4096) cfg.g_h = 4096;

    // Timebase
    cfg.g_timebase.num = 1;
    if (offset < Size) cfg.g_timebase.den = read_u32(Data, &offset, Size);
    if (cfg.g_timebase.den == 0) cfg.g_timebase.den = 30;

    // Rate control
    if (offset < Size) cfg.rc_end_usage = (enum vpx_rc_mode)read_u8(Data, &offset, Size);
    if (offset < Size) cfg.rc_target_bitrate = read_u32(Data, &offset, Size);
    if (offset < Size) cfg.rc_min_quantizer = read_u8(Data, &offset, Size);
    if (offset < Size) cfg.rc_max_quantizer = read_u8(Data, &offset, Size);

    // Initialize the encoder context
    if (vpx_codec_enc_init(&ctx, iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // 2. Fuzz vpx_codec_enc_config_set
    // Reconfigure with potentially modified settings
    if (offset < Size) cfg.g_w = read_u16(Data, &offset, Size);
    if (offset < Size) cfg.g_h = read_u16(Data, &offset, Size);
    
    // Call the target function
    vpx_codec_err_t err1 = vpx_codec_enc_config_set(&ctx, &cfg);
    (void)err1; // Suppress unused warning

    // 3. Prepare Image Data for vpx_codec_encode
    // Allocate image planes
    int stride = cfg.g_w;
    int size_y = stride * cfg.g_h;
    int size_uv = (stride / 2) * (cfg.g_h / 2);
    
    img.planes[0] = malloc(size_y);
    img.planes[1] = malloc(size_uv);
    img.planes[2] = malloc(size_uv);
    
    if (!img.planes[0] || !img.planes[1] || !img.planes[2]) {
        vpx_codec_destroy(&ctx);
        free(img.planes[0]);
        free(img.planes[1]);
        free(img.planes[2]);
        return 0;
    }

    // Fill image with fuzz data
    memset(img.planes[0], 0, size_y);
    memset(img.planes[1], 0, size_uv);
    memset(img.planes[2], 0, size_uv);
    
    if (Size > offset) {
        size_t copy_len = Size - offset;
        if (copy_len > size_y) copy_len = size_y;
        memcpy(img.planes[0], Data + offset, copy_len);
    }

    img.fmt = VPX_IMG_FMT_I420;
    img.w = cfg.g_w;
    img.h = cfg.g_h;
    img.d_w = cfg.g_w;
    img.d_h = cfg.g_h;
    img.stride[0] = stride;
    img.stride[1] = stride / 2;
    img.stride[2] = stride / 2;
    img.bit_depth = cfg.g_bit_depth;
    img.x_chroma_shift = 1;
    img.y_chroma_shift = 1;
    img.cs = VPX_CS_BT_601;
    img.range = VPX_CR_STUDIO_RANGE;

    // 4. Fuzz vpx_codec_encode
    vpx_codec_pts_t pts = 0;
    unsigned long duration = 1;
    vpx_enc_frame_flags_t flags = 0;
    vpx_enc_deadline_t deadline = 0;

    if (offset < Size) pts = (vpx_codec_pts_t)read_u32(Data, &offset, Size);
    if (offset < Size) duration = read_u32(Data, &offset, Size);
    if (offset < Size) flags = (vpx_enc_frame_flags_t)read_u32(Data, &offset, Size);
    if (offset < Size) deadline = (vpx_enc_deadline_t)read_u32(Data, &offset, Size);

    // Encode a frame
    vpx_codec_err_t err2 = vpx_codec_encode(&ctx, &img, pts, duration, flags, deadline);
    (void)err2;

    // Flush the encoder (send NULL image)
    vpx_codec_err_t err3 = vpx_codec_encode(&ctx, NULL, pts + 1, duration, flags, deadline);
    (void)err3;

    // 5. Fuzz vpx_codec_set_cx_data_buf
    // Allocate a custom buffer
    size_t buf_size = 1024;
    if (offset < Size) {
        uint32_t s = read_u32(Data, &offset, Size);
        if (s > 0 && s < 1024 * 1024) buf_size = s;
    }
    
    custom_buf_data = malloc(buf_size);
    if (custom_buf_data) {
        custom_buf.buf = custom_buf_data;
        custom_buf.sz = buf_size;
        
        unsigned int pad_before = 0;
        unsigned int pad_after = 0;
        if (offset < Size) pad_before = read_u16(Data, &offset, Size);
        if (offset < Size) pad_after = read_u16(Data, &offset, Size);
        
        vpx_codec_err_t err4 = vpx_codec_set_cx_data_buf(&ctx, &custom_buf, pad_before, pad_after);
        (void)err4;
        
        // Try setting to NULL to reset
        vpx_codec_err_t err5 = vpx_codec_set_cx_data_buf(&ctx, NULL, 0, 0);
        (void)err5;
    }

    // 6. Handle Empty/Placeholder Functions
    // VP9D_GET_BIT_DEPTH__value, VP8E_SET_CQ_LEVEL__value, VP8E_SET_ARNR_TYPE__value
    // Since their signatures are empty or unknown, we cannot call them safely.
    // We skip them to avoid compilation errors or undefined behavior.

    // 7. Cleanup
    vpx_codec_destroy(&ctx);
    free(img.planes[0]);
    free(img.planes[1]);
    free(img.planes[2]);
    free(custom_buf_data);

    return 0;
}