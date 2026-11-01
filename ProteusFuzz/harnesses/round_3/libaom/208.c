// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>

/*
 * Fuzz driver for libaom codec control functions.
 */

static aom_codec_ctx_t *init_decoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;

    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) {
        free(ctx);
        return NULL;
    }

    aom_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;

    aom_codec_err_t err = aom_codec_dec_init(ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    // If data is provided, attempt to decode a frame to set up state
    if (data && size > 0) {
        aom_codec_err_t decode_err = aom_codec_decode(ctx, data, size, NULL);
        if (decode_err == AOM_CODEC_OK) {
            aom_codec_iter_t iter = NULL;
            aom_image_t *img = aom_codec_get_frame(ctx, &iter);
            if (img) {
                // Frame is owned by the codec, no need to free
            }
        }
    }

    return ctx;
}

static aom_codec_ctx_t *init_encoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;

    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        free(ctx);
        return NULL;
    }

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    // Use fuzz data to set some config parameters if possible, otherwise defaults
    if (size >= 4) {
        uint32_t val = *(const uint32_t *)data;
        cfg.g_w = (val % 256) + 16;
        cfg.g_h = ((val >> 8) % 256) + 16;
        cfg.g_timebase.num = ((val >> 16) % 1000) + 1;
        cfg.g_timebase.den = ((val >> 24) % 1000) + 1;
        cfg.rc_target_bitrate = ((val >> 8) % 10000) + 100;
    } else {
        cfg.g_w = 16;
        cfg.g_h = 16;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 1000;
        cfg.rc_target_bitrate = 100;
    }

    cfg.g_threads = 1;
    cfg.g_error_resilient = 1;

    aom_codec_err_t err = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    // Encode a dummy frame to ensure state is ready for quantizer queries
    if (size > 0) {
        aom_image_t img;
        aom_img_alloc(&img, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
        // Fill with fuzz data if large enough, else random
        if (size >= cfg.g_w * cfg.g_h * 3 / 2) {
            memcpy(img.planes[0], data, cfg.g_w * cfg.g_h);
        } else {
            memset(img.planes[0], 0xFF, cfg.g_w * cfg.g_h);
        }
        // Fixed: aom_codec_encode takes 5 arguments (ctx, img, pts, duration, flags)
        aom_codec_encode(ctx, &img, 0, 1, 0);
        aom_img_free(&img);
    }

    return ctx;
}

static void cleanup_decoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

/*
 * Helper to safely call aom_codec_control with a given type and argument.
 */
static void test_control(aom_codec_ctx_t *ctx, int control_type, int32_t arg) {
    if (!ctx) return;
    aom_codec_control(ctx, control_type, arg);
}

/*
 * Helper to safely call aom_codec_control with a pointer argument.
 */
static void test_control_ptr(aom_codec_ctx_t *ctx, int control_type, void *arg) {
    if (!ctx) return;
    aom_codec_control(ctx, control_type, arg);
}

int LLVMFuzzerTestOneInput_208(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Initialize contexts
    aom_codec_ctx_t *dec_ctx = init_decoder(Data, Size);
    aom_codec_ctx_t *enc_ctx = init_encoder(Data, Size);

    // Extract values from fuzz input to drive control arguments
    int32_t val1 = 0, val2 = 0;
    if (Size >= 4) {
        val1 = *(const int32_t *)Data;
    }
    if (Size >= 8) {
        val2 = *(const int32_t *)(Data + 4);
    }

    // 1. AOMD_GET_LAST_QUANTIZER (Decoder)
    if (dec_ctx) {
        int32_t quant = 0;
        test_control_ptr(dec_ctx, AOMD_GET_LAST_QUANTIZER, &quant);
    }

    // 2. AOMD_GET_BASE_Q_IDX (Decoder)
    if (dec_ctx) {
        int32_t base_q = 0;
        test_control_ptr(dec_ctx, AOMD_GET_BASE_Q_IDX, &base_q);
    }

    // 3. AV1_SET_INSPECTION_CALLBACK (Encoder)
    if (enc_ctx) {
        aom_inspect_init inspect_cfg;
        memset(&inspect_cfg, 0, sizeof(inspect_cfg));
        test_control_ptr(enc_ctx, AV1_SET_INSPECTION_CALLBACK, &inspect_cfg);
        
        if (Size >= 16) {
            void *dummy_cb = (void *)(uintptr_t)(*(const uint64_t *)Data);
            inspect_cfg.inspect_cb = (aom_inspect_cb)dummy_cb;
            inspect_cfg.inspect_ctx = (void *)(uintptr_t)(*(const uint64_t *)(Data + 8));
            test_control_ptr(enc_ctx, AV1_SET_INSPECTION_CALLBACK, &inspect_cfg);
        }
    }

    // 4. AOME_GET_LAST_QUANTIZER_64 (Encoder)
    if (enc_ctx) {
        int64_t quant64 = 0;
        test_control_ptr(enc_ctx, AOME_GET_LAST_QUANTIZER_64, &quant64);
    }

    // 5. AV1E_SET_QUANTIZER_ONE_PASS (Encoder)
    if (enc_ctx) {
        test_control(enc_ctx, AV1E_SET_QUANTIZER_ONE_PASS, val1);
        test_control(enc_ctx, AV1E_SET_QUANTIZER_ONE_PASS, val2);
        test_control(enc_ctx, AV1E_SET_QUANTIZER_ONE_PASS, 0);
        test_control(enc_ctx, AV1E_SET_QUANTIZER_ONE_PASS, 255);
        test_control(enc_ctx, AV1E_SET_QUANTIZER_ONE_PASS, -1);
    }

    // 6. AOME_GET_LAST_QUANTIZER (Encoder)
    if (enc_ctx) {
        int32_t quant = 0;
        test_control_ptr(enc_ctx, AOME_GET_LAST_QUANTIZER, &quant);
    }

    // Cleanup
    cleanup_decoder(dec_ctx);
    cleanup_encoder(enc_ctx);

    return 0;
}