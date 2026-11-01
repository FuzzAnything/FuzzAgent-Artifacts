// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_error_detail at aom_codec.c:61:13 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_enc_config_set at aom_encoder.c:298:17 in aom_encoder.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_error_detail at aom_codec.c:61:13 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_error_detail at aom_codec.c:61:13 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_error_detail at aom_codec.c:61:13 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_error_detail at aom_codec.c:61:13 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_error_detail at aom_codec.c:61:13 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_image.h>
#include <aom/aomcx.h>

static void setup_config_from_fuzz_data(aom_codec_enc_cfg_t *cfg, const uint8_t *Data, size_t Size) {
    if (!cfg || !Data) return;

    memset(cfg, 0, sizeof(aom_codec_enc_cfg_t));

    if (Size > 0) cfg->g_w = Data[0] + 16;
    if (Size > 1) cfg->g_h = Data[1] + 16;
    if (Size > 2) cfg->g_threads = Data[2] % 8 + 1;
    if (Size > 3) cfg->g_profile = Data[3] % 4;
    if (Size > 4) cfg->g_bit_depth = (Data[4] % 3 == 0) ? AOM_BITS_8 : ((Data[4] % 3 == 1) ? AOM_BITS_10 : AOM_BITS_12);
    if (Size > 5) cfg->g_input_bit_depth = cfg->g_bit_depth;
    if (Size > 6) cfg->rc_target_bitrate = (Data[6] * 100) + 100;
    if (Size > 7) cfg->rc_end_usage = (enum aom_rc_mode)(Data[7] % 4);
    if (Size > 8) cfg->g_usage = Data[8];
    if (Size > 9) cfg->g_pass = (enum aom_enc_pass)(Data[9] % 4);
    if (Size > 10) cfg->kf_mode = (enum aom_kf_mode)(Data[10] % 3);
    if (Size > 11) cfg->kf_min_dist = Data[11] + 1;
    if (Size > 12) cfg->kf_max_dist = (Data[12] * 10) + 10;
    
    // Timebase
    cfg->g_timebase.num = 1;
    if (Size > 13) cfg->g_timebase.den = (Data[13] * 100) + 1000;

    // Ensure valid ranges for critical fields
    if (cfg->g_w < 16) cfg->g_w = 16;
    if (cfg->g_h < 16) cfg->g_h = 16;
    if (cfg->g_w % 2 != 0) cfg->g_w++;
    if (cfg->g_h % 2 != 0) cfg->g_h++;
}

static int create_test_image(aom_image_t *img, unsigned int w, unsigned int h, unsigned int bit_depth) {
    if (!img || w == 0 || h == 0) return -1;

    aom_img_fmt_t fmt = AOM_IMG_FMT_I420;
    if (bit_depth > 8) fmt = AOM_IMG_FMT_I42016;

    if (aom_img_alloc(img, fmt, w, h, 1) != 0) {
        return -1;
    }

    // Fill with some data to avoid all-zero frames if possible, though fuzzing likes zeros
    for (int i = 0; i < 3; i++) {
        if (img->planes[i] && img->stride[i] > 0) {
            size_t plane_size = (size_t)img->stride[i] * h;
            // Simple pattern fill
            for (size_t j = 0; j < plane_size; j++) {
                img->planes[i][j] = (uint8_t)(j % 256);
            }
        }
    }
    img->bit_depth = bit_depth;
    return 0;
}

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_image_t img;
    aom_codec_iface_t *iface = NULL;

    // 1. Prepare environment
    memset(&ctx, 0, sizeof(ctx));
    memset(&img, 0, sizeof(img));

    // Get encoder interface
    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    // Setup config based on fuzz data
    setup_config_from_fuzz_data(&cfg, Data, Size);

    // Initialize encoder
    aom_codec_err_t ret = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        // Test error handling functions on a failed init
        const char *err_str = aom_codec_error(&ctx);
        (void)err_str; // consume
        const char *err_detail = aom_codec_error_detail(&ctx);
        (void)err_detail; // consume
        const char *err_code_str = aom_codec_err_to_string(ret);
        (void)err_code_str; // consume
        return 0;
    }

    // 2. Explore program states with target functions

    // Test aom_codec_set_option with fuzzed strings
    // We create safe strings from the input data to avoid nulls in the middle
    char key[64], val[64];
    size_t key_len = (Size > 10) ? (Data[10] % 10) + 1 : 1;
    size_t val_len = (Size > 11) ? (Data[11] % 10) + 1 : 1;
    if (key_len > 63) key_len = 63;
    if (val_len > 63) val_len = 63;

    memcpy(key, Data + 12, key_len);
    key[key_len] = '\0';
    memcpy(val, Data + 12 + key_len, val_len);
    val[val_len] = '\0';

    // Try setting an option
    ret = aom_codec_set_option(&ctx, key, val);
    // Check error
    if (ret != AOM_CODEC_OK) {
        (void)aom_codec_error(&ctx);
        (void)aom_codec_error_detail(&ctx);
        (void)aom_codec_err_to_string(ret);
    }

    // Test aom_codec_enc_config_set
    // Reconfigure with potentially modified config
    if (Size > 20) {
        cfg.g_w = (Data[20] % 1024) + 16;
        cfg.g_h = (Data[21] % 1024) + 16;
        ret = aom_codec_enc_config_set(&ctx, &cfg);
        if (ret != AOM_CODEC_OK) {
            (void)aom_codec_error(&ctx);
            (void)aom_codec_error_detail(&ctx);
            (void)aom_codec_err_to_string(ret);
        }
    }

    // Create an image for encoding
    unsigned int w = cfg.g_w;
    unsigned int h = cfg.g_h;
    unsigned int bit_depth = cfg.g_bit_depth;

    // Ensure alignment
    if (w % 2 != 0) w++;
    if (h % 2 != 0) h++;

    if (create_test_image(&img, w, h, bit_depth) == 0) {
        aom_codec_pts_t pts = 0;
        unsigned long duration = (Size > 22) ? (Data[22] + 1) : 1;
        aom_enc_frame_flags_t flags = 0;

        // Encode one frame
        ret = aom_codec_encode(&ctx, &img, pts, duration, flags);
        if (ret != AOM_CODEC_OK) {
            (void)aom_codec_error(&ctx);
            (void)aom_codec_error_detail(&ctx);
            (void)aom_codec_err_to_string(ret);
        }

        // Flush the encoder
        while (1) {
            ret = aom_codec_encode(&ctx, NULL, 0, 0, 0);
            if (ret != AOM_CODEC_OK) {
                (void)aom_codec_error(&ctx);
                (void)aom_codec_error_detail(&ctx);
                (void)aom_codec_err_to_string(ret);
                break;
            }
            // Check if there is output data to stop flushing
            aom_codec_cx_pkt_t *pkt = NULL;
            while ((pkt = aom_codec_get_cx_data(&ctx, pkt)) != NULL) {
                if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                    // Consume packet
                }
            }
            if (pkt == NULL) break; 
        }
    }

    // Test error functions with null context (safe by spec)
    const char *null_err = aom_codec_error(NULL);
    (void)null_err;
    const char *null_detail = aom_codec_error_detail(NULL);
    (void)null_detail;

    // Test error string conversion with various codes
    (void)aom_codec_err_to_string(AOM_CODEC_OK);
    (void)aom_codec_err_to_string(AOM_CODEC_INVALID_PARAM);
    (void)aom_codec_err_to_string(AOM_CODEC_INCAPABLE);
    (void)aom_codec_err_to_string(AOM_CODEC_MEM_ERROR);
    (void)aom_codec_err_to_string((aom_codec_err_t)999); // Unrecognized

    // 3. Cleanup
    aom_codec_destroy(&ctx);
    aom_img_free(&img);

    return 0;
}