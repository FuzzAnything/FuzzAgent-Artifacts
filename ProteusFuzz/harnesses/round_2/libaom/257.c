// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_set_cx_data_buf at aom_encoder.c:244:17 in aom_encoder.h
// aom_codec_enc_config_set at aom_encoder.c:298:17 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_error_detail at aom_codec.c:61:13 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_error_detail at aom_codec.c:61:13 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aom_image.h>
#include <aom/aom_integer.h>

// Forward declarations for AV1 specific interfaces if not in headers
extern const struct aom_codec_iface *aom_codec_av1_cx(void);

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static void cleanup_image(aom_image_t *img) {
    if (img) {
        aom_img_free(img);
    }
}

int LLVMFuzzerTestOneInput_257(const uint8_t *Data, size_t Size) {
    if (Size < 16) return 0;

    aom_codec_ctx_t ctx = {0};
    aom_codec_enc_cfg_t cfg = {0};
    
    // Get encoder interface
    const aom_codec_iface_t *enc_iface = aom_codec_av1_cx();
    if (!enc_iface) return 0;

    // Use fuzz data to derive configuration parameters
    unsigned int width = (Data[0] % 64) + 16;
    unsigned int height = (Data[1] % 64) + 16;
    unsigned int threads = (Data[2] % 8) + 1;
    unsigned int bitrate = (Data[3] % 100) + 10;
    unsigned int usage = Data[4] % 4; 
    unsigned int profile = Data[5] % 4;
    unsigned int bit_depth = (Data[6] % 3 == 0) ? 8 : ((Data[6] % 2 == 0) ? 10 : 12);
    
    // Initialize encoder config
    if (aom_codec_enc_config_default(enc_iface, &cfg, usage) != AOM_CODEC_OK) {
        return 0;
    }

    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_threads = threads;
    cfg.g_profile = profile;
    cfg.g_bit_depth = (aom_bit_depth_t)bit_depth;
    cfg.g_input_bit_depth = bit_depth;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = (Data[7] % 60) + 30; 
    cfg.rc_target_bitrate = bitrate;
    cfg.rc_end_usage = AOM_VBR;
    cfg.g_limit = 0; 
    cfg.kf_mode = AOM_KF_AUTO;
    cfg.kf_min_dist = (Data[8] % 10) + 1;
    cfg.kf_max_dist = (Data[9] % 100) + 10;

    // Initialize encoder context
    if (aom_codec_enc_init(&ctx, enc_iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Create an image to encode
    aom_image_t img = {0};
    aom_img_fmt_t fmt = AOM_IMG_FMT_I420;
    if (bit_depth > 8) fmt = AOM_IMG_FMT_I42016;

    if (aom_img_alloc(&img, fmt, width, height, 1) == NULL) {
        cleanup_codec(&ctx);
        return 0;
    }

    // Fill image planes with fuzz data safely
    // We must respect the stride and plane dimensions
    // Plane 0 (Y)
    for (int i = 0; i < img.h; i++) {
        for (int j = 0; j < img.w; j++) {
            size_t idx = (i * img.w + j) % Size;
            img.planes[0][i * img.stride[0] + j] = Data[idx];
        }
    }

    // Plane 1 (U) and Plane 2 (V)
    // Calculate dimensions based on format (I420 has 2x subsampling)
    int uv_w = (width + 1) / 2;
    int uv_h = (height + 1) / 2;

    for (int i = 0; i < uv_h; i++) {
        for (int j = 0; j < uv_w; j++) {
            size_t idx_u = (i * uv_w + j) % Size;
            size_t idx_v = (i * uv_w + j + 1) % Size;
            
            if (bit_depth <= 8) {
                img.planes[1][i * img.stride[1] + j] = (uint8_t)(Data[idx_u] & 0x3F) + 128;
                img.planes[2][i * img.stride[2] + j] = (uint8_t)(Data[idx_v] & 0x3F) + 128;
            } else {
                // 10/12 bit depth handling (assuming 16-bit storage)
                uint16_t *p1 = (uint16_t *)img.planes[1];
                uint16_t *p2 = (uint16_t *)img.planes[2];
                
                // Write 16-bit values. Note stride is in bytes, so we divide by 2 for 16-bit indexing
                // However, aom_img_alloc sets stride in bytes. 
                // For 10/12 bit, stride is usually multiple of 2.
                // We access as uint16_t pointer, so index is (row * stride/2 + col)
                // But we must ensure we don't exceed allocated memory. 
                // The safe way is to use the byte stride and cast the target address.
                
                uint16_t val_u = (uint16_t)((Data[idx_u] & 0x3F) + 512);
                uint16_t val_v = (uint16_t)((Data[idx_v] & 0x3F) + 512);
                
                // Calculate byte offset
                size_t byte_offset_u = i * img.stride[1] + j * 2;
                size_t byte_offset_v = i * img.stride[2] + j * 2;
                
                // Check bounds (though loop limits should prevent this)
                if (byte_offset_u + 1 < img.sz) {
                    *(uint16_t*)((uint8_t*)img.planes[1] + byte_offset_u) = val_u;
                }
                if (byte_offset_v + 1 < img.sz) {
                    *(uint16_t*)((uint8_t*)img.planes[2] + byte_offset_v) = val_v;
                }
            }
        }
    }

    // Test aom_codec_set_option
    const char *option_name = "cpu-used";
    const char *option_value = "0";
    if (Data[10] % 2 == 0) {
        aom_codec_set_option(&ctx, option_name, option_value);
    } else {
        aom_codec_set_option(&ctx, "invalid-option", "invalid-value");
    }

    // Test aom_codec_set_cx_data_buf
    uint8_t *output_buf = malloc(1024 * 1024);
    if (output_buf) {
        aom_fixed_buf_t buf = { .buf = output_buf, .sz = 1024 * 1024 };
        unsigned int pad_before = Data[11] % 10;
        unsigned int pad_after = Data[12] % 10;
        aom_codec_set_cx_data_buf(&ctx, &buf, pad_before, pad_after);
    }

    // Test aom_codec_enc_config_set
    if (Data[13] % 3 == 0) {
        aom_codec_enc_config_set(&ctx, &cfg);
    }

    // Encode frames
    aom_codec_pts_t pts = 0;
    unsigned long duration = (Data[14] % 100) + 1;
    aom_enc_frame_flags_t flags = 0;

    // Encode a few frames
    for (int i = 0; i < 3; i++) {
        aom_codec_err_t res = aom_codec_encode(&ctx, &img, pts, duration, flags);
        if (res != AOM_CODEC_OK && res != AOM_CODEC_ERROR) {
            // Check error detail
            const char *err_detail = aom_codec_error_detail(&ctx);
            (void)err_detail; 
        }
        
        // Drain output
        aom_codec_cx_pkt_t pkt;
        while (aom_codec_get_cx_data(&ctx, &pkt) == AOM_CODEC_OK) {
            // Process packet if needed
        }

        pts += duration;
        if (i + 15 < Size) {
            duration = (Data[15 + i] % 100) + 1;
        }
    }

    // Flush the encoder
    while (1) {
        aom_codec_err_t res = aom_codec_encode(&ctx, NULL, 0, 0, 0);
        if (res != AOM_CODEC_OK) break;
        
        aom_codec_cx_pkt_t pkt;
        int got_data = 0;
        while (aom_codec_get_cx_data(&ctx, &pkt) == AOM_CODEC_OK) {
            got_data = 1;
        }
        if (!got_data) break;
    }

    // Test error detail retrieval
    const char *detail = aom_codec_error_detail(&ctx);
    (void)detail;

    // Cleanup
    if (output_buf) free(output_buf);
    cleanup_image(&img);
    cleanup_codec(&ctx);

    return 0;
}