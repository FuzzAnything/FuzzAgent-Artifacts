// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_enc_config_set at aom_encoder.c:298:17 in aom_encoder.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_error_detail at aom_codec.c:61:13 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
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
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_error_detail at aom_codec.c:61:13 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aom_integer.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aom_external_partition.h>

static void init_config_from_fuzz(aom_codec_enc_cfg_t *cfg, const uint8_t *Data, size_t Size) {
    memset(cfg, 0, sizeof(aom_codec_enc_cfg_t));
    
    // Use fuzz data to populate config, ensuring bounds
    if (Size > 0) cfg->g_w = Data[0] ? Data[0] : 16;
    if (Size > 1) cfg->g_h = Data[1] ? Data[1] : 16;
    if (Size > 2) cfg->g_bit_depth = (Data[2] % 3 == 0) ? AOM_BITS_8 : (Data[2] % 3 == 1 ? AOM_BITS_10 : AOM_BITS_12);
    if (Size > 3) cfg->g_input_bit_depth = cfg->g_bit_depth;
    
    cfg->g_timebase.num = (Size > 4) ? Data[4] : 1;
    cfg->g_timebase.den = (Size > 5) ? (Data[5] ? Data[5] : 1) : 1;
    
    cfg->g_threads = (Size > 6) ? (Data[6] % 16) : 1;
    cfg->rc_target_bitrate = (Size > 7) ? (Data[7] * 100) : 1000;
    cfg->rc_end_usage = (Size > 8) ? (Data[8] % 4) : AOM_VBR;
    
    cfg->kf_min_dist = (Size > 9) ? (Data[9] * 2) : 12;
    cfg->kf_max_dist = (Size > 10) ? (Data[10] * 2 + cfg->kf_min_dist) : 12;
    
    // Ensure valid dimensions
    if (cfg->g_w < 16) cfg->g_w = 16;
    if (cfg->g_h < 16) cfg->g_h = 16;
}

static aom_image_t create_image_from_fuzz(const uint8_t *Data, size_t Size, unsigned int w, unsigned int h, unsigned int bit_depth) {
    aom_image_t img;
    aom_img_fmt_t fmt = AOM_IMG_FMT_I420;
    if (bit_depth > 8) fmt = AOM_IMG_FMT_I42016;

    aom_img_alloc(&img, fmt, w, h, 16);
    
    // Fill image data with fuzz data
    size_t plane_idx = 0;
    for (int i = 0; i < 3; i++) {
        if (img.planes[i]) {
            size_t stride = img.stride[i];
            size_t height = (i == 0) ? h : (h + 1) / 2;
            size_t bytes_per_pixel = (bit_depth == 8) ? 1 : 2;
            
            for (size_t y = 0; y < height; y++) {
                size_t row_size = stride * bytes_per_pixel;
                size_t data_left = Size - plane_idx;
                size_t to_copy = (row_size < data_left) ? row_size : data_left;
                
                if (to_copy > 0) {
                    memcpy(img.planes[i] + y * stride, Data + plane_idx, to_copy);
                    plane_idx += to_copy;
                }
                // Fill rest with 0 if data runs out
                if (to_copy < row_size) {
                    memset(img.planes[i] + y * stride + to_copy, 0, row_size - to_copy);
                }
            }
        }
    }
    img.bit_depth = bit_depth;
    return img;
}

int LLVMFuzzerTestOneInput_67(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t ret;
    const char *err_str;
    const char *err_detail;

    // 1. Prepare environment
    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    // Initialize config with fuzz data
    init_config_from_fuzz(&cfg, Data, Size);

    // Initialize encoder
    ret = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        // Test error handling on init failure
        err_str = aom_codec_error(&ctx);
        (void)err_str; // Suppress unused warning
        err_detail = aom_codec_error_detail(&ctx);
        (void)err_detail;
        aom_codec_err_to_string(ret);
        return 0;
    }

    // 2. Fuzz aom_codec_enc_config_set
    // Reconfigure with potentially modified config
    if (Size > 12) {
        cfg.g_threads = Data[12] % 32;
        ret = aom_codec_enc_config_set(&ctx, &cfg);
        if (ret != AOM_CODEC_OK) {
            err_str = aom_codec_error(&ctx);
            err_detail = aom_codec_error_detail(&ctx);
            aom_codec_err_to_string(ret);
        }
    }

    // 3. Fuzz aom_codec_set_option
    // Try setting various options using fuzz data as keys/values
    char key[64], value[64];
    if (Size > 13) {
        size_t key_len = Data[13] % 32;
        size_t val_len = (Size > 14) ? (Data[14] % 32) : 0;
        
        if (key_len > 0 && key_len < sizeof(key)) {
            memcpy(key, Data + 15, key_len);
            key[key_len] = '\0';
            
            size_t val_start = 15 + key_len;
            if (val_len > 0 && val_len < sizeof(value) && val_start + val_len <= Size) {
                memcpy(value, Data + val_start, val_len);
                value[val_len] = '\0';
                
                ret = aom_codec_set_option(&ctx, key, value);
                if (ret != AOM_CODEC_OK) {
                    err_str = aom_codec_error(&ctx);
                    err_detail = aom_codec_error_detail(&ctx);
                    aom_codec_err_to_string(ret);
                }
            }
        }
    }

    // 4. Fuzz aom_codec_encode
    // Create a dummy image
    aom_image_t img = create_image_from_fuzz(Data, Size, cfg.g_w, cfg.g_h, cfg.g_bit_depth);
    
    // Encode a few frames
    aom_codec_pts_t pts = 0;
    unsigned long duration = (Size > 20) ? (Data[20] ? Data[20] : 1) : 1;
    aom_enc_frame_flags_t flags = (Size > 21) ? (Data[21] & 0xFF) : 0;

    // Encode one frame
    ret = aom_codec_encode(&ctx, &img, pts, duration, flags);
    if (ret != AOM_CODEC_OK) {
        err_str = aom_codec_error(&ctx);
        err_detail = aom_codec_error_detail(&ctx);
        aom_codec_err_to_string(ret);
    }
    
    // Flush encoder
    while (1) {
        aom_codec_cx_pkt_t *pkt;
        ret = aom_codec_encode(&ctx, NULL, 0, 0, 0);
        if (ret != AOM_CODEC_OK) {
            err_str = aom_codec_error(&ctx);
            err_detail = aom_codec_error_detail(&ctx);
            aom_codec_err_to_string(ret);
            break;
        }
        
        pkt = aom_codec_get_cx_data(&ctx, NULL);
        if (!pkt) break;
        
        // No need to free pkt explicitly as it is managed by the context
        // The aom_codec_cx_pkt_free function does not exist in the public API
        // We just continue to the next iteration to consume all packets
    }

    // 5. Cleanup
    aom_img_free(&img);
    aom_codec_destroy(&ctx);

    return 0;
}