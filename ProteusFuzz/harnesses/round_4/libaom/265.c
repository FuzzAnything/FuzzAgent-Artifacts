// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <aom/aomdx.h>
#include <aom/aom_encoder.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aom_integer.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aom_external_partition.h>

/* 
 * Helper function to initialize the encoder context.
 * Returns 0 on success, -1 on failure.
 */
static int init_encoder_context(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg,
                                const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface;
    aom_codec_err_t res;
    int width, height;

    if (ctx == NULL || cfg == NULL) return -1;

    /* Determine dimensions from input data if possible, otherwise use defaults */
    if (size >= 4) {
        width = (data[0] % 64) + 16;
        height = (data[1] % 64) + 16;
    } else {
        width = 16;
        height = 16;
    }

    /* Ensure alignment */
    width = (width + 15) & ~15;
    height = (height + 15) & ~15;

    iface = aom_codec_av1_cx();
    if (iface == NULL) return -1;

    res = aom_codec_enc_config_default(iface, cfg, 0);
    if (res != AOM_CODEC_OK) return -1;

    cfg->g_w = width;
    cfg->g_h = height;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_profile = 0;
    cfg->rc_end_usage = AOM_CQ;
    cfg->rc_target_bitrate = 1000;

    res = aom_codec_enc_init(ctx, iface, cfg, 0);
    if (res != AOM_CODEC_OK) return -1;

    return 0;
}

/* Helper to create a dummy image */
static int create_dummy_image(aom_image_t *img, const uint8_t *data, size_t size,
                              unsigned int w, unsigned int h, unsigned int bit_depth) {
    aom_img_fmt_t fmt = AOM_IMG_FMT_I420;
    aom_image_t *ret_img;

    if (bit_depth > 8) {
        fmt = AOM_IMG_FMT_I42016;
    }

    ret_img = aom_img_alloc(img, fmt, w, h, 16);
    if (ret_img == NULL || img->planes[0] == NULL) return -1;

    /* Fill image with fuzz data safely using the allocated size */
    /* The allocated size is stored in img->sz for the total buffer, 
       but we need to be careful with plane strides.
       The safe way is to fill each plane up to its stride * height.
    */
    
    /* Y Plane */
    size_t y_plane_size = (size_t)img->stride[0] * h;
    size_t copy_len = (size < y_plane_size) ? size : y_plane_size;
    if (copy_len > 0) {
        memcpy(img->planes[0], data, copy_len);
        if (copy_len < y_plane_size) {
            memset(img->planes[0] + copy_len, 0, y_plane_size - copy_len);
        }
    }

    /* U Plane */
    size_t u_plane_size = (size_t)img->stride[1] * (h >> img->y_chroma_shift);
    size_t remaining_data = (size > y_plane_size) ? (size - y_plane_size) : 0;
    copy_len = (remaining_data < u_plane_size) ? remaining_data : u_plane_size;
    if (copy_len > 0) {
        memcpy(img->planes[1], data + y_plane_size, copy_len);
        if (copy_len < u_plane_size) {
            memset(img->planes[1] + copy_len, 0, u_plane_size - copy_len);
        }
    }

    /* V Plane */
    size_t v_plane_size = (size_t)img->stride[2] * (h >> img->y_chroma_shift);
    remaining_data = (size > (y_plane_size + u_plane_size)) ? (size - y_plane_size - u_plane_size) : 0;
    copy_len = (remaining_data < v_plane_size) ? remaining_data : v_plane_size;
    if (copy_len > 0) {
        memcpy(img->planes[2], data + y_plane_size + u_plane_size, copy_len);
        if (copy_len < v_plane_size) {
            memset(img->planes[2] + copy_len, 0, v_plane_size - copy_len);
        }
    }

    img->bit_depth = bit_depth;
    return 0;
}

int LLVMFuzzerTestOneInput_265(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_image_t img;
    aom_codec_err_t res;
    int width, height, bit_depth;
    const char *option_name;
    const char *option_value;
    aom_codec_cx_pkt_t *pkt;
    aom_codec_pts_t pts;
    unsigned long duration;
    aom_enc_frame_flags_t flags;

    if (Size < 4) return 0;

    /* Initialize context and config */
    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));
    memset(&img, 0, sizeof(img));

    if (init_encoder_context(&ctx, &cfg, Data, Size) != 0) {
        return 0;
    }

    /* Extract parameters from fuzz data */
    width = (Data[0] % 128) + 16;
    height = (Data[1] % 128) + 16;
    width = (width + 15) & ~15;
    height = (height + 15) & ~15;
    bit_depth = (Data[2] % 4) + 8; /* 8, 10, 12, or 14 */

    /* Create image */
    if (create_dummy_image(&img, Data + 3, Size - 3, width, height, bit_depth) != 0) {
        aom_codec_destroy(&ctx);
        return 0;
    }

    /* Fuzz aom_codec_set_option */
    /* We try a few known option names and random strings */
    const char *options[] = {
        "cpu-used", "crf", "end-usage", "threads", "arnr-maxframes",
        "noise-sensitivity", "denoise-block-size", "enable-qm", "deltaq-mode",
        "random-option-string"
    };
    const char *values[] = {
        "0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
        "vbr", "cbr", "cq", "cqp", "true", "false", "auto"
    };

    if (Size > 10) {
        int opt_idx = Data[3] % 11;
        int val_idx = Data[4] % 17;
        
        res = aom_codec_set_option(&ctx, options[opt_idx], values[val_idx]);
        (void)res; /* Ignore return for fuzzing exploration */
    }

    /* Fuzz aom_codec_control_typechecked_AV1E_SET_NOISE_SENSITIVITY */
    /* Signature: aom_codec_control(ctx, AV1E_SET_NOISE_SENSITIVITY, value) */
    if (Size > 5) {
        int noise_sens = (int)(Data[5] % 6); /* 0 to 5 */
        res = aom_codec_control(&ctx, AV1E_SET_NOISE_SENSITIVITY, noise_sens);
        (void)res;
    }

    /* Fuzz aom_codec_control_typechecked_AV1E_SET_DENOISE_BLOCK_SIZE */
    /* Signature: aom_codec_control(ctx, AV1E_SET_DENOISE_BLOCK_SIZE, value) */
    if (Size > 6) {
        int block_size = (int)(Data[6] % 4); /* 0 to 3 */
        res = aom_codec_control(&ctx, AV1E_SET_DENOISE_BLOCK_SIZE, block_size);
        (void)res;
    }

    /* Fuzz aom_codec_control_typechecked_AV1E_SET_ENABLE_QM */
    /* Signature: aom_codec_control(ctx, AV1E_SET_ENABLE_QM, value) */
    if (Size > 7) {
        int enable_qm = (Data[7] % 2); /* 0 or 1 */
        res = aom_codec_control(&ctx, AV1E_SET_ENABLE_QM, enable_qm);
        (void)res;
    }

    /* Fuzz aom_codec_control_typechecked_AV1E_SET_DELTAQ_MODE */
    /* Signature: aom_codec_control(ctx, AV1E_SET_DELTAQ_MODE, value) */
    if (Size > 8) {
        int deltaq_mode = (int)(Data[8] % 3); /* 0 to 2 */
        res = aom_codec_control(&ctx, AV1E_SET_DELTAQ_MODE, deltaq_mode);
        (void)res;
    }

    /* Fuzz aom_codec_encode */
    if (Size > 9) {
        pts = (aom_codec_pts_t)(Data[9] % 1000);
        duration = (unsigned long)(Data[10] % 1000);
        if (duration == 0) duration = 1; /* Must be non-zero if img is not NULL */
        flags = (aom_enc_frame_flags_t)(Data[11] % 4);

        res = aom_codec_encode(&ctx, &img, pts, duration, flags);
        (void)res;

        /* Flush the encoder */
        while (1) {
            res = aom_codec_encode(&ctx, NULL, 0, 0, 0);
            if (res != AOM_CODEC_OK) break;
            
            while ((pkt = aom_codec_get_cx_data(&ctx, NULL)) != NULL) {
                /* aom_codec_cx_pkt_data_free is not a standard public API function in all versions.
                 * The standard way is to let the codec handle cleanup or ignore the packet data 
                 * if we are just fuzzing. However, to avoid memory leaks in the fuzzer, 
                 * we rely on the fact that aom_codec_destroy cleans up internal buffers.
                 * If the packet contains user data that needs freeing, the API usually 
                 * requires the caller to free it, but aom_codec_cx_pkt_data_free 
                 * might be internal. We will skip explicit freeing of the packet data 
                 * to avoid compilation errors if the symbol is missing, relying on 
                 * aom_codec_destroy for cleanup.
                 */
            }
            if (pkt == NULL) break;
        }
    }

    /* Cleanup */
    aom_codec_destroy(&ctx);
    aom_img_free(&img);

    return 0;
}