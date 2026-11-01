// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_set_cx_data_buf at aom_encoder.c:244:17 in aom_encoder.h
// aom_codec_get_global_headers at aom_encoder.c:281:18 in aom_encoder.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_enc_config_set at aom_encoder.c:298:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_set_cx_data_buf at aom_encoder.c:244:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom_image.h>
#include <aom/aom_integer.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>

static int fuzz_encoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iter_t iter = NULL;
    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    
    if (!iface) return 0;

    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz configuration from input data
    if (size > 0) cfg.g_w = data[0] + 16;
    if (size > 1) cfg.g_h = data[1] + 16;
    if (size > 2) cfg.g_threads = data[2] % 8 + 1;
    if (size > 3) cfg.g_profile = data[3] % 4;
    if (size > 4) cfg.g_bit_depth = (data[4] % 3 == 0) ? AOM_BITS_8 : (data[4] % 3 == 1 ? AOM_BITS_10 : AOM_BITS_12);
    if (size > 5) cfg.g_input_bit_depth = cfg.g_bit_depth;
    if (size > 6) cfg.g_timebase.num = data[6] + 1;
    if (size > 7) cfg.g_timebase.den = data[7] + 1;
    if (size > 8) cfg.rc_end_usage = data[8] % 4;
    if (size > 9) cfg.rc_target_bitrate = (data[9] * 100) + 100;
    if (size > 10) cfg.rc_min_quantizer = data[10] % 64;
    if (size > 11) cfg.rc_max_quantizer = (data[11] % 64) + 10;
    if (size > 12) cfg.kf_mode = data[12] % 3;
    if (size > 13) cfg.kf_min_dist = data[13] * 10;
    if (size > 14) cfg.kf_max_dist = data[14] * 10 + 100;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Test aom_codec_set_option
    if (size > 15) {
        char key[32], val[32];
        int k_len = (data[15] % 10) + 1;
        int v_len = (data[16] % 10) + 1;
        if (k_len < sizeof(key)) {
            strncpy(key, (const char*)(data + 17), k_len);
            key[k_len] = '\0';
            if (v_len < sizeof(val)) {
                strncpy(val, (const char*)(data + 17 + k_len), v_len);
                val[v_len] = '\0';
                aom_codec_set_option(&ctx, key, val);
            }
        }
    }

    // Test aom_codec_enc_config_set
    if (size > 18) {
        aom_codec_enc_cfg_t new_cfg = cfg;
        new_cfg.g_w = (data[18] % 100) + 16;
        new_cfg.g_h = (data[19] % 100) + 16;
        aom_codec_enc_config_set(&ctx, &new_cfg);
    }

    // Create a dummy image to encode
    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
    memset(img.planes[0], 0x80, img.stride[0] * cfg.g_h);
    memset(img.planes[1], 0x80, img.stride[1] * (cfg.g_h/2));
    memset(img.planes[2], 0x80, img.stride[2] * (cfg.g_h/2));

    // Encode one frame. aom_codec_encode takes 5 arguments: ctx, img, pts, duration, flags
    if (aom_codec_encode(&ctx, &img, 0, 1, 0) == AOM_CODEC_OK) {
        // Test aom_codec_get_cx_data
        const aom_codec_cx_pkt_t *pkt;
        while ((pkt = aom_codec_get_cx_data(&ctx, &iter)) != NULL) {
            // Consume packet to avoid leaks or state issues
            if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
                // Do nothing, just iterate
            }
        }
    }

    // Test aom_codec_set_cx_data_buf
    uint8_t *out_buf = malloc(1024);
    if (out_buf) {
        aom_fixed_buf_t buf = { .buf = out_buf, .sz = 1024 };
        aom_codec_set_cx_data_buf(&ctx, &buf, 10, 10);
        free(out_buf);
        // Reset to default
        aom_codec_set_cx_data_buf(&ctx, NULL, 0, 0);
    }

    // Test aom_codec_get_global_headers
    aom_fixed_buf_t *hdr = aom_codec_get_global_headers(&ctx);
    if (hdr) {
        free(hdr->buf);
        free(hdr);
    }

    // Test aom_codec_error
    const char *err = aom_codec_error(&ctx);
    (void)err; // Suppress unused warning

    // Cleanup
    aom_codec_destroy(&ctx);
    aom_img_free(&img);

    return 0;
}

static int fuzz_decoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t ctx;
    aom_codec_dec_cfg_t cfg;
    aom_codec_iter_t iter = NULL;
    const aom_codec_iface_t *iface = aom_codec_av1_dx();

    if (!iface) return 0;

    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = (size > 0) ? (data[0] % 8) + 1 : 1;

    if (aom_codec_dec_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Test aom_codec_set_option on decoder
    if (size > 1) {
        char key[16] = "some_option";
        char val[16] = "some_value";
        aom_codec_set_option(&ctx, key, val);
    }

    // Test aom_codec_error with null and valid
    const char *err1 = aom_codec_error(NULL);
    const char *err2 = aom_codec_error(&ctx);
    (void)err1;
    (void)err2;

    // Try to decode garbage data
    aom_codec_dec_cfg_t dummy_cfg;
    memset(&dummy_cfg, 0, sizeof(dummy_cfg));
    if (size > 2) {
        aom_codec_dec_init(&ctx, iface, &dummy_cfg, 0); // Re-init to test state
    }

    aom_codec_destroy(&ctx);
    return 0;
}

int LLVMFuzzerTestOneInput_99(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Switch between encoder and decoder fuzzing based on first byte
    if (Data[0] % 2 == 0) {
        fuzz_encoder(Data, Size);
    } else {
        fuzz_decoder(Data, Size);
    }

    return 0;
}