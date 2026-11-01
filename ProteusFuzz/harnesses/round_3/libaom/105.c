// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_enc_config_set at aom_encoder.c:298:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom_image.h>
#include <aom/aom_codec.h>
#include <aom/aom_integer.h>
#include <aom/aom_frame_buffer.h>

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static int fuzz_encode_frame(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx || !data || size == 0) return 0;

    aom_image_t img;
    aom_image_t *img_ptr = &img;
    
    // Initialize image structure with safe defaults
    memset(img_ptr, 0, sizeof(aom_image_t));
    img_ptr->fmt = AOM_IMG_FMT_I420;
    img_ptr->w = 16;
    img_ptr->h = 16;
    img_ptr->bit_depth = 8;
    img_ptr->d_w = 16;
    img_ptr->d_h = 16;
    img_ptr->r_w = 16;
    img_ptr->r_h = 16;
    img_ptr->x_chroma_shift = 1;
    img_ptr->y_chroma_shift = 1;
    img_ptr->stride[0] = 16;
    img_ptr->stride[1] = 8;
    img_ptr->stride[2] = 8;
    
    // Allocate minimal planes for fuzzing
    size_t y_size = 16 * 16;
    size_t uv_size = 8 * 8;
    
    unsigned char *y_plane = malloc(y_size);
    unsigned char *u_plane = malloc(uv_size);
    unsigned char *v_plane = malloc(uv_size);
    
    if (!y_plane || !u_plane || !v_plane) {
        free(y_plane);
        free(u_plane);
        free(v_plane);
        return 0;
    }

    // Fill planes with fuzz data if available, otherwise random
    size_t total_needed = y_size + uv_size + uv_size;
    size_t available = size;
    
    if (available >= y_size) {
        memcpy(y_plane, data, y_size);
        available -= y_size;
    } else {
        memset(y_plane, 0, y_size);
    }
    
    if (available >= uv_size) {
        memcpy(u_plane, data + (size - available), uv_size);
        available -= uv_size;
    } else {
        memset(u_plane, 0, uv_size);
    }
    
    if (available >= uv_size) {
        memcpy(v_plane, data + (size - available), uv_size);
    } else {
        memset(v_plane, 0, uv_size);
    }

    img_ptr->planes[0] = y_plane;
    img_ptr->planes[1] = u_plane;
    img_ptr->planes[2] = v_plane;
    img_ptr->img_data = NULL;
    img_ptr->img_data_owner = 0;
    img_ptr->self_allocd = 1;

    aom_codec_err_t err;
    unsigned long duration = 1;
    aom_codec_pts_t pts = 0;
    aom_enc_frame_flags_t flags = 0;

    err = aom_codec_encode(ctx, img_ptr, pts, duration, flags);
    
    // Flush encoder
    while (err == AOM_CODEC_OK) {
        aom_codec_cx_pkt_t *pkt;
        if (aom_codec_get_cx_data(ctx, &pkt) == AOM_CODEC_OK) {
            // Do not attempt to access pkt->data.frame.buf.buf directly as the structure
            // might be opaque or the specific member access might be invalid in this context.
            // The codec manages the memory for the packet data.
            // We simply consume the packet to clear the queue.
        } else {
            break;
        }
    }

    free(y_plane);
    free(u_plane);
    free(v_plane);

    return (err == AOM_CODEC_OK) ? 1 : 0;
}

int LLVMFuzzerTestOneInput_105(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    
    // Initialize config with defaults
    if (aom_codec_enc_config_default(iface, &cfg, AOM_USAGE_GOOD_QUALITY) != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz configuration parameters based on input data
    cfg.g_w = (Data[0] % 256) + 16;
    cfg.g_h = (Data[1] % 256) + 16;
    cfg.g_bit_depth = (Data[2] % 3 == 0) ? AOM_BITS_8 : (Data[2] % 3 == 1 ? AOM_BITS_10 : AOM_BITS_12);
    cfg.g_input_bit_depth = cfg.g_bit_depth;
    cfg.g_threads = (Data[3] % 4) + 1;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = (Data[4] % 100) + 1;
    cfg.rc_target_bitrate = (Data[5] % 100) + 100;
    cfg.g_limit = (Data[6] % 10) + 1;
    cfg.g_profile = (Data[7] % 4);
    
    // Initialize encoder context
    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Try to set configuration again (reconfig)
    aom_codec_enc_config_set(&ctx, &cfg);

    // Encode a frame if we have enough data
    if (Size > 10) {
        fuzz_encode_frame(&ctx, Data + 10, Size - 10);
    }

    // Cleanup
    cleanup_encoder(&ctx);

    return 0;
}