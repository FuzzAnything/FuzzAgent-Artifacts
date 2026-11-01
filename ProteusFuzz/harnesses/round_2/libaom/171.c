// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_get_global_headers at aom_encoder.c:281:18 in aom_encoder.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>

// Helper to safely free global headers
static void free_global_headers(aom_fixed_buf_t *hdr) {
    if (hdr) {
        if (hdr->buf) {
            free(hdr->buf);
        }
        free(hdr);
    }
}

// Helper to safely read a byte from fuzz data with bounds checking
static uint8_t get_byte(const uint8_t *data, size_t size, size_t index, uint8_t default_val) {
    if (index < size) {
        return data[index];
    }
    return default_val;
}

// Helper to create a dummy image frame from fuzz input
static aom_image_t *create_dummy_image(const uint8_t *data, size_t size) {
    if (size < 4) return NULL;
    
    // Extract width and height from first 4 bytes (2 bytes each)
    uint16_t width = get_byte(data, size, 0, 16) | (get_byte(data, size, 1, 16) << 8);
    uint16_t height = get_byte(data, size, 2, 16) | (get_byte(data, size, 3, 16) << 8);
    
    // Clamp to reasonable values to avoid allocation issues
    if (width == 0) width = 16;
    if (height == 0) height = 16;
    if (width > 4096) width = 4096;
    if (height > 4096) height = 4096;
    
    aom_image_t *img = malloc(sizeof(aom_image_t));
    if (!img) return NULL;
    
    if (aom_img_alloc(img, AOM_IMG_FMT_I420, width, height, 1) != 0) {
        free(img);
        return NULL;
    }
    
    // Fill with fuzz data if available
    size_t plane_size = img->d_w * img->d_h;
    if (size > 4 && size - 4 >= plane_size) {
        memcpy(img->planes[0], data + 4, plane_size);
    }
    
    return img;
}

int LLVMFuzzerTestOneInput_171(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Initialize encoder context
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    
    if (!iface) return 0;

    // Initialize config
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use fuzz data to configure encoder with bounds checking
    cfg.g_w = (get_byte(Data, Size, 0, 16) % 256) + 16;
    cfg.g_h = (get_byte(Data, Size, 1, 16) % 256) + 16;
    cfg.g_timebase.num = (get_byte(Data, Size, 2, 1) % 100) + 1;
    cfg.g_timebase.den = (get_byte(Data, Size, 3, 1) % 100) + 1;
    cfg.g_threads = (get_byte(Data, Size, 4, 1) % 8) + 1;
    cfg.g_error_resilient = (get_byte(Data, Size, 5, 0) % 2);
    cfg.rc_target_bitrate = ((get_byte(Data, Size, 6, 0) * 256 + get_byte(Data, Size, 7, 0)) % 10000) + 1000;
    cfg.rc_min_quantizer = get_byte(Data, Size, 8, 0) % 64;
    cfg.rc_max_quantizer = (get_byte(Data, Size, 9, 64) % 64) + cfg.rc_min_quantizer;
    cfg.g_lag_in_frames = get_byte(Data, Size, 10, 0) % 20;

    // Initialize encoder
    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz control functions with type-checked variants
    // AV1E_SET_ERROR_RESILIENT_MODE
    int error_resilient_mode = get_byte(Data, Size, 11, 0) % 3;
    aom_codec_control(&ctx, AV1E_SET_ERROR_RESILIENT_MODE, error_resilient_mode);

    // AV1E_SET_DELTAQ_STRENGTH
    int deltaq_strength = get_byte(Data, Size, 12, 0) % 64;
    aom_codec_control(&ctx, AV1E_SET_DELTAQ_STRENGTH, deltaq_strength);

    // AV1E_SET_NUM_TG
    int num_tg = get_byte(Data, Size, 13, 0) % 16;
    aom_codec_control(&ctx, AV1E_SET_NUM_TG, num_tg);

    // AV1E_SET_PARTITION_INFO_PATH - requires file path
    // Create dummy file for partition info
    FILE *f = fopen("./dummy_file", "w");
    if (f) {
        if (Size > 14) {
            fwrite(Data + 14, 1, Size - 14, f);
        }
        fclose(f);
        aom_codec_control(&ctx, AV1E_SET_PARTITION_INFO_PATH, "./dummy_file");
    }

    // Encode a frame
    if (Size > 15) {
        aom_image_t *img = create_dummy_image(Data + 15, Size - 15);
        if (img) {
            // Fixed: aom_codec_encode takes 5 arguments: ctx, img, pts, duration, flags
            aom_codec_encode(&ctx, img, 0, 1, 0);
            aom_img_free(img);
            free(img);
        }
    }

    // Get global headers
    aom_fixed_buf_t *global_hdr = aom_codec_get_global_headers(&ctx);
    if (global_hdr) {
        // Use the data to trigger more states
        if (global_hdr->sz > 0 && global_hdr->buf) {
            // Just access to ensure memory is valid
            volatile uint8_t *ptr = (uint8_t *)global_hdr->buf;
            volatile uint8_t val = ptr[0];
            (void)val;
        }
        free_global_headers(global_hdr);
    }

    // Get error string
    const char *err_str = aom_codec_error(&ctx);
    if (err_str) {
        // Use error string to trigger more states
        size_t len = strlen(err_str);
        volatile size_t vlen = len;
        (void)vlen;
    }

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}