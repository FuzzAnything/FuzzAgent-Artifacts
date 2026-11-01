// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_set_cx_data_buf at aom_encoder.c:244:17 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_get_global_headers at aom_encoder.c:281:18 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_enc_config_set at aom_encoder.c:298:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
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

// Helper to create a simple test image
static aom_image_t *create_test_image(aom_codec_ctx_t *ctx, unsigned int w, unsigned int h, unsigned int bit_depth) {
    if (w == 0 || h == 0) return NULL;
    if (bit_depth == 0) bit_depth = 8;
    
    aom_image_t *img = aom_img_alloc(NULL, AOM_IMG_FMT_I420, w, h, 1);
    if (!img) return NULL;
    
    img->bit_depth = bit_depth;
    img->d_w = w;
    img->d_h = h;
    img->r_w = w;
    img->r_h = h;
    
    // Fill with random data from the fuzz input if possible, or just zeros
    // For safety in fuzzing, we just zero it or use a pattern
    for (int i = 0; i < 3; i++) {
        if (img->planes[i]) {
            memset(img->planes[i], 0, img->stride[i] * h);
        }
    }
    return img;
}

int LLVMFuzzerTestOneInput_186(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Initialize context
    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface = aom_codec_av1_dx(); // Use decoder interface as a fallback or encoder
    // Actually, for encoding functions we need encoder interface
    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    if (aom_codec_enc_init(&ctx, iface, NULL, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Prepare configuration
    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        aom_codec_destroy(&ctx);
        return 0;
    }

    // Fuzz configuration parameters based on input data
    if (Size > 0) cfg.g_w = Data[0] == 0 ? 64 : Data[0];
    if (Size > 1) cfg.g_h = Data[1] == 0 ? 64 : Data[1];
    if (Size > 2) cfg.g_bit_depth = (Data[2] % 5 == 0) ? AOM_BITS_8 : (Data[2] % 5 == 1 ? AOM_BITS_10 : AOM_BITS_12);
    if (Size > 3) cfg.g_threads = Data[3] % 4 + 1;
    if (Size > 4) cfg.g_profile = Data[4] % 4;
    if (Size > 5) cfg.rc_target_bitrate = (Data[5] * 100) + 100;
    if (Size > 6) cfg.g_usage = Data[6] % 3;

    // Apply configuration
    aom_codec_err_t err = aom_codec_enc_config_set(&ctx, &cfg);
    if (err != AOM_CODEC_OK) {
        aom_codec_destroy(&ctx);
        return 0;
    }

    // Re-initialize with config
    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        aom_codec_destroy(&ctx);
        return 0;
    }

    // Fuzz aom_codec_set_option
    if (Size > 10) {
        char key[32], val[32];
        size_t key_len = Data[10] % 30;
        size_t val_len = Data[11] % 30;
        if (key_len > 0 && val_len > 0) {
            memcpy(key, Data + 12, key_len);
            key[key_len] = '\0';
            memcpy(val, Data + 12 + key_len, val_len);
            val[val_len] = '\0';
            aom_codec_set_option(&ctx, key, val);
        }
    }

    // Fuzz aom_codec_set_cx_data_buf
    if (Size > 15) {
        unsigned int pad_before = Data[15] % 100;
        unsigned int pad_after = Data[16] % 100;
        size_t buf_size = (Size > 17) ? (Data[17] * 1000) : 10000;
        if (buf_size > 0) {
            uint8_t *buf = malloc(buf_size);
            if (buf) {
                aom_fixed_buf_t fixed_buf;
                fixed_buf.buf = buf;
                fixed_buf.sz = buf_size;
                aom_codec_set_cx_data_buf(&ctx, &fixed_buf, pad_before, pad_after);
                free(buf);
            }
        }
    }

    // Fuzz aom_codec_encode
    if (Size > 20) {
        unsigned int w = (Data[20] % 10) + 16;
        unsigned int h = (Data[21] % 10) + 16;
        unsigned int bit_depth = (Data[22] % 3 == 0) ? 8 : (Data[22] % 3 == 1 ? 10 : 12);
        
        aom_image_t *img = create_test_image(&ctx, w, h, bit_depth);
        if (img) {
            aom_codec_pts_t pts = (Size > 23) ? (int64_t)Data[23] : 0;
            unsigned long duration = (Size > 24) ? (unsigned long)Data[24] : 1;
            if (duration == 0) duration = 1;
            aom_enc_frame_flags_t flags = (Size > 25) ? (aom_enc_frame_flags_t)Data[25] : 0;

            aom_codec_encode(&ctx, img, pts, duration, flags);
            
            // Flush
            aom_codec_encode(&ctx, NULL, 0, 0, 0);
            
            aom_img_free(img);
        }
    }

    // Fuzz aom_codec_get_global_headers
    aom_fixed_buf_t *headers = aom_codec_get_global_headers(&ctx);
    if (headers) {
        // Do something with headers to avoid unused variable warning, then free
        if (headers->buf && headers->sz > 0) {
            // Just read first byte to ensure it's valid
            volatile uint8_t dummy = *(uint8_t*)headers->buf;
            (void)dummy;
        }
        free(headers->buf);
        free(headers);
    }

    // Cleanup
    aom_codec_destroy(&ctx);
    return 0;
}