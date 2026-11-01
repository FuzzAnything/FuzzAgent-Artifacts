// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_error_detail at aom_codec.c:61:13 in aom_codec.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_error_detail at aom_codec.c:61:13 in aom_codec.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_decoder.h>
#include <aom/aom_external_partition.h>
#include <aom/aomcx.h>
#include <aom/aom_image.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom.h>

static void cleanup_ctx(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static aom_image_t* create_test_image(const uint8_t *data, size_t size, int width, int height, int bit_depth) {
    if (width <= 0 || height <= 0 || bit_depth <= 0 || bit_depth > 12) return NULL;
    if (size < (size_t)width * height * 3) return NULL;

    aom_image_t *img = (aom_image_t *)malloc(sizeof(aom_image_t));
    if (!img) return NULL;

    memset(img, 0, sizeof(aom_image_t));
    img->fmt = (bit_depth > 8) ? AOM_IMG_FMT_I42016 : AOM_IMG_FMT_I420;
    img->w = width;
    img->h = height;
    img->d_w = width;
    img->d_h = height;
    img->bit_depth = bit_depth;
    img->x_chroma_shift = 1;
    img->y_chroma_shift = 1;
    img->stride[0] = width;
    img->stride[1] = width / 2;
    img->stride[2] = width / 2;

    // Allocate planes
    size_t y_size = width * height;
    size_t uv_size = (width / 2) * (height / 2);
    
    img->planes[0] = (unsigned char *)malloc(y_size * sizeof(uint16_t));
    img->planes[1] = (unsigned char *)malloc(uv_size * sizeof(uint16_t));
    img->planes[2] = (unsigned char *)malloc(uv_size * sizeof(uint16_t));

    if (!img->planes[0] || !img->planes[1] || !img->planes[2]) {
        free(img->planes[0]);
        free(img->planes[1]);
        free(img->planes[2]);
        free(img);
        return NULL;
    }

    // Fill with data
    size_t total_needed = y_size + 2 * uv_size;
    size_t copy_len = (size < total_needed) ? size : total_needed;
    memcpy(img->planes[0], data, copy_len);
    
    return img;
}

static void free_image(aom_image_t *img) {
    if (img) {
        free(img->planes[0]);
        free(img->planes[1]);
        free(img->planes[2]);
        free(img);
    }
}

int LLVMFuzzerTestOneInput_125(const uint8_t *Data, size_t Size) {
    if (Size < 16) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);

    // Use fuzzing data to configure encoder
    cfg.g_w = (Data[0] % 256) + 16;
    cfg.g_h = (Data[1] % 256) + 16;
    cfg.g_timebase.num = (Data[2] % 255) + 1;
    cfg.g_timebase.den = (Data[3] % 255) + 1;
    cfg.rc_target_bitrate = (Data[4] * 256 + Data[5]) + 100;
    cfg.g_pass = AOM_RC_ONE_PASS;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        cleanup_ctx(&ctx);
        return 0;
    }

    // Fuzz aom_codec_set_option
    char key[64], val[64];
    
    if (Size > 10) {
        int len_k = Data[8] % 20;
        int len_v = Data[9] % 20;
        if (len_k > 0 && len_v > 0) {
            const char *safe_keys[] = {"crf", "cpu-used", "arnr-maxframes", "arnr-strength", "lag-in-frames", "static-thresh"};
            int key_idx = Data[10] % 6;
            strncpy(key, safe_keys[key_idx], sizeof(key) - 1);
            key[sizeof(key) - 1] = '\0';

            int val_int = (Data[11] << 8) | Data[12];
            snprintf(val, sizeof(val), "%d", val_int);
            
            aom_codec_set_option(&ctx, key, val);
        }
    }

    // Fuzz VMAF model path via option
    if (Size > 20) {
        char path[128];
        int path_len = Data[20] % 100;
        if (path_len > 0 && path_len < 120) {
            memcpy(path, Data + 21, path_len);
            path[path_len] = '\0';
            aom_codec_set_option(&ctx, "vmaf-model-path", path);
        }
    }

    // Fuzz aom_codec_encode
    aom_image_t *img = NULL;
    aom_codec_pts_t pts = 0;
    unsigned long duration = 1;
    aom_enc_frame_flags_t flags = 0;

    if (Size > 32) {
        int w = (Data[24] % 128) + 16;
        int h = (Data[25] % 128) + 16;
        int bd = (Data[26] % 4) + 8; // 8, 10, 12
        
        img = create_test_image(Data + 32, Size - 32, w, h, bd);
        if (img) {
            pts = ((int64_t)Data[27] << 24) | (Data[28] << 16) | (Data[29] << 8) | Data[30];
            duration = (unsigned long)((Data[31] << 8) | Data[32]);
            if (duration == 0) duration = 1;
            
            flags = Data[33];
            
            aom_codec_err_t ret = aom_codec_encode(&ctx, img, pts, duration, flags);
            
            if (ret != AOM_CODEC_OK) {
                const char *err_str = aom_codec_error(&ctx);
                const char *err_detail = aom_codec_error_detail(&ctx);
                (void)err_str;
                (void)err_detail;
            }
            
            // Consume output data
            const aom_codec_cx_pkt_t *pkt;
            while ((pkt = aom_codec_get_cx_data(&ctx, NULL)) != NULL) {
                aom_codec_destroy(&ctx); // Destroying context implicitly frees data, but we need to be careful
                // Actually, aom_codec_get_cx_data returns a packet, we should free it properly if possible
                // The standard way is aom_codec_cx_pkt_free if available, but often the data is owned by the context
                // Let's just break or continue. The correct API is aom_codec_cx_pkt_free(pkt) if it exists, 
                // but in many versions it's handled by the context. 
                // To be safe and avoid crashes, we just ignore the packet content for fuzzing.
                // However, the loop condition checks for NULL, so we just need to ensure we don't leak.
                // In libaom, the packet data is usually freed when the context is destroyed or when the next packet is retrieved.
                // But to be safe, let's just stop after one packet if we can't free it safely.
                // Actually, the standard pattern is:
                // while ((pkt = aom_codec_get_cx_data(&ctx, NULL)) != NULL) {
                //    // process pkt
                //    aom_codec_cx_pkt_free(pkt); // This function might not exist in all versions
                // }
                // Since aom_codec_cx_pkt_free is not in the provided headers, we assume the context manages it or we just break.
                // Let's try to call aom_codec_cx_pkt_free if it compiles, otherwise skip.
                // Given the error, we must not call undefined functions.
                // We will just break to avoid infinite loops if the API is tricky.
                break; 
            }
            
            free_image(img);
            img = NULL;
        }
    }

    // Flush the encoder
    while (1) {
        aom_codec_err_t ret = aom_codec_encode(&ctx, NULL, 0, 0, 0);
        if (ret != AOM_CODEC_OK) {
            const char *err = aom_codec_error(&ctx);
            const char *detail = aom_codec_error_detail(&ctx);
            (void)err;
            (void)detail;
            break;
        }
        const aom_codec_cx_pkt_t *pkt;
        int has_data = 0;
        while ((pkt = aom_codec_get_cx_data(&ctx, NULL)) != NULL) {
            has_data = 1;
            // Avoid calling undefined free function
            break; 
        }
        if (!has_data) break;
    }

    // Test error string conversion
    aom_codec_err_t dummy_err = AOM_CODEC_INVALID_PARAM;
    if (Size > 40) dummy_err = (aom_codec_err_t)(Data[40] % 100);
    const char *err_str = aom_codec_err_to_string(dummy_err);
    (void)err_str;

    cleanup_ctx(&ctx);
    return 0;
}