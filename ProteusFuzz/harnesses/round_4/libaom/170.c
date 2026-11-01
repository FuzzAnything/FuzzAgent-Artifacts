// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_error_detail at aom_codec.c:61:13 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_error_detail at aom_codec.c:61:13 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_error_detail at aom_codec.c:61:13 in aom_codec.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_error_detail at aom_codec.c:61:13 in aom_codec.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_error_detail at aom_codec.c:61:13 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include "aom/aomdx.h"
#include "aom/aom_encoder.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_decoder.h"
#include "aom/aom_image.h"
#include "aom/aom.h"
#include "aom/aom_integer.h"
#include "aom/aom_codec.h"
#include "aom/aomcx.h"
#include "aom/aom_external_partition.h"

static aom_image_t alloc_image(aom_img_fmt_t fmt, unsigned int w, unsigned int h, unsigned int bit_depth) {
    aom_image_t img;
    aom_img_fmt_t fmt_internal = fmt;
    if (bit_depth > 8) {
        if (fmt == AOM_IMG_FMT_I420) fmt_internal = AOM_IMG_FMT_I42016;
        else if (fmt == AOM_IMG_FMT_YV12) fmt_internal = AOM_IMG_FMT_YV1216;
        else if (fmt == AOM_IMG_FMT_I422) fmt_internal = AOM_IMG_FMT_I42216;
        else if (fmt == AOM_IMG_FMT_I444) fmt_internal = AOM_IMG_FMT_I44416;
    }

    if (aom_img_alloc(&img, fmt_internal, w, h, 32) != 0) {
        memset(&img, 0, sizeof(img));
        return img;
    }
    img.bit_depth = bit_depth;
    return img;
}

static void free_image(aom_image_t *img) {
    if (img && img->img_data) {
        aom_img_free(img);
    }
}

int LLVMFuzzerTestOneInput_170(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    // Parse input to determine test scenario
    uint32_t seed = *(uint32_t *)Data;
    int use_encoder = (seed & 1);
    int use_null_ctx = ((seed >> 1) & 1);
    int use_invalid_option = ((seed >> 2) & 1);
    int use_flush = ((seed >> 3) & 1);
    int bit_depth = (seed >> 4) & 3; // 0->8, 1->10, 2->12, 3->8 (fallback)
    int width = (seed >> 8) % 256 + 16;
    int height = (seed >> 16) % 256 + 16;

    if (width > 4096 || height > 4096) {
        width = 64;
        height = 64;
    }

    if (bit_depth == 3) bit_depth = 8;
    else if (bit_depth == 0) bit_depth = 8;
    else if (bit_depth == 1) bit_depth = 10;
    else if (bit_depth == 2) bit_depth = 12;

    aom_codec_ctx_t ctx = {0};
    aom_codec_iface_t *iface = NULL;
    aom_codec_err_t err;
    aom_image_t img = {0};
    const char *err_str = NULL;
    const char *err_detail = NULL;

    // 1. Prepare environment
    if (use_encoder) {
        iface = aom_codec_av1_cx();
        if (!iface) return 0;

        aom_codec_enc_cfg_t cfg;
        if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) return 0;

        cfg.g_w = width;
        cfg.g_h = height;
        cfg.g_bit_depth = bit_depth;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 30;
        cfg.rc_target_bitrate = 1000;

        err = aom_codec_enc_init(&ctx, iface, &cfg, 0);
        if (err != AOM_CODEC_OK) {
            // Fuzz error handling path
            err_str = aom_codec_error(&ctx);
            err_detail = aom_codec_error_detail(&ctx);
            (void)err_str;
            (void)err_detail;
            return 0;
        }

        // Set options
        if (use_invalid_option) {
            aom_codec_set_option(&ctx, "invalid_option_name_xyz", "invalid_value");
        } else {
            // Try to set a valid option if data allows
            if (Size > 10) {
                char key[32], val[32];
                int key_len = (Data[4] % 10) + 1;
                int val_len = (Data[5] % 10) + 1;
                if (key_len > 31) key_len = 31;
                if (val_len > 31) val_len = 31;
                
                memcpy(key, Data + 6, key_len);
                key[key_len] = '\0';
                memcpy(val, Data + 6 + key_len, val_len);
                val[val_len] = '\0';
                
                // Only set if it looks like a plausible key to avoid immediate crash on bad keys
                // But we want to fuzz, so we pass it.
                aom_codec_set_option(&ctx, key, val);
            }
        }

        // Allocate image
        aom_img_fmt_t fmt = (bit_depth > 8) ? AOM_IMG_FMT_I42016 : AOM_IMG_FMT_I420;
        img = alloc_image(fmt, width, height, bit_depth);
        if (img.planes[0] == NULL) {
            aom_codec_destroy(&ctx);
            return 0;
        }

        // Fill image with fuzz data
        for (int i = 0; i < 3; i++) {
            if (img.planes[i]) {
                size_t plane_size = (size_t)img.stride[i] * img.h;
                if (Size > 10) {
                    memcpy(img.planes[i], Data + 10, (Size - 10 < plane_size) ? (Size - 10) : plane_size);
                }
            }
        }

        // 2. Invoke target functions
        aom_codec_pts_t pts = 0;
        unsigned long duration = 1;
        aom_enc_frame_flags_t flags = 0;

        // Encode a frame
        if (!use_flush) {
            err = aom_codec_encode(&ctx, &img, pts, duration, flags);
        } else {
            // Flush
            err = aom_codec_encode(&ctx, NULL, 0, 0, flags);
        }

        // 3. Check errors and retrieve strings
        if (err != AOM_CODEC_OK) {
            err_str = aom_codec_error(&ctx);
            err_detail = aom_codec_error_detail(&ctx);
            // Fuzz error string conversion
            const char *err_str_conv = aom_codec_err_to_string(err);
            (void)err_str_conv;
        }

        // Test with null context for error functions
        if (use_null_ctx) {
            err_str = aom_codec_error(NULL);
            err_detail = aom_codec_error_detail(NULL);
            (void)err_str;
            (void)err_detail;
        }

        // Test error string conversion with various codes
        const char *dummy_err = aom_codec_err_to_string(AOM_CODEC_MEM_ERROR);
        const char *dummy_err2 = aom_codec_err_to_string(AOM_CODEC_INVALID_PARAM);
        const char *dummy_err3 = aom_codec_err_to_string((aom_codec_err_t)9999);
        (void)dummy_err;
        (void)dummy_err2;
        (void)dummy_err3;

        // 4. Cleanup
        free_image(&img);
        aom_codec_destroy(&ctx);

    } else {
        // Decoder path (simplified for fuzzing context)
        iface = aom_codec_av1_dx();
        if (!iface) return 0;

        aom_codec_dec_cfg_t cfg = {0};
        cfg.threads = 1;
        // Fixed: allow_highbitdepth does not exist, using allow_lowbitdepth or leaving default
        // cfg.allow_lowbitdepth = 1; // Not strictly necessary for basic init, but valid field

        err = aom_codec_dec_init(&ctx, iface, &cfg, 0);
        if (err != AOM_CODEC_OK) {
            err_str = aom_codec_error(&ctx);
            err_detail = aom_codec_error_detail(&ctx);
            (void)err_str;
            (void)err_detail;
            return 0;
        }

        // Test error functions on valid decoder context
        err_str = aom_codec_error(&ctx);
        err_detail = aom_codec_error_detail(&ctx);
        (void)err_str;
        (void)err_detail;

        // Test set_option on decoder
        if (use_invalid_option) {
            aom_codec_set_option(&ctx, "bad_decoder_opt", "bad_val");
        }

        aom_codec_destroy(&ctx);
    }

    return 0;
}