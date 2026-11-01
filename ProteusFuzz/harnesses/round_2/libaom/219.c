// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>
#include <aom/aomcx.h>
#include <aom/aom_image.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_219(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    aom_codec_ctx_t ctx = {0};
    aom_codec_iface_t *iface = NULL;
    aom_codec_enc_cfg_t cfg;
    aom_codec_dec_cfg_t dec_cfg = {0};
    aom_codec_err_t err;
    int32_t quantizer_val;
    int64_t quantizer_val_64;
    int base_q_idx;
    int cdef_strength;

    // Extract parameters from fuzz input
    int width = Data[0] + 1;
    int height = Data[1] + 1;
    int bit_depth = (Data[2] % 2) + 8; // 8 or 9
    int profile = Data[3] % 4;

    // Initialize Encoder
    if (aom_codec_enc_config_default(NULL, &cfg, 0) == AOM_CODEC_OK) {
        cfg.g_w = width;
        cfg.g_h = height;
        cfg.g_bit_depth = bit_depth;
        cfg.g_profile = profile;
        cfg.rc_target_bitrate = 1000;
        cfg.rc_undershoot_pct = 10;
        cfg.rc_overshoot_pct = 10;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 30;
        cfg.g_threads = 1;
        cfg.g_usage = AOM_CQ;
        // Removed cfg.cq_level as it is not a direct member of aom_codec_enc_cfg
        // It is typically set via aom_codec_control(AV1E_SET_CQ_LEVEL) or similar

        iface = aom_codec_av1_cx();
        if (iface) {
            err = aom_codec_enc_init(&ctx, iface, &cfg, 0);
            if (err == AOM_CODEC_OK) {
                // Test AV1E_SET_QUANTIZER_ONE_PASS
                int32_t set_quant = (int32_t)(Data[4] * 256);
                aom_codec_control(&ctx, AV1E_SET_QUANTIZER_ONE_PASS, set_quant);

                // Test AOME_GET_LAST_QUANTIZER
                aom_codec_control(&ctx, AOME_GET_LAST_QUANTIZER, &quantizer_val);

                // Test AOME_GET_LAST_QUANTIZER_64
                aom_codec_control(&ctx, AOME_GET_LAST_QUANTIZER_64, &quantizer_val_64);

                // Test AV1E_GET_LUMA_CDEF_STRENGTH
                aom_codec_control(&ctx, AV1E_GET_LUMA_CDEF_STRENGTH, &cdef_strength);

                // Encode a dummy frame to trigger state changes
                aom_image_t img = {0};
                aom_img_alloc(&img, AOM_IMG_FMT_I420, width, height, 1);
                // Fill with dummy data from input if available
                if (Size > 4 + width * height * 3 / 2) {
                    memcpy(img.planes[0], Data + 4, width * height);
                    memcpy(img.planes[1], Data + 4 + width * height, width * height / 4);
                    memcpy(img.planes[2], Data + 4 + width * height * 5 / 4, width * height / 4);
                }

                // Fixed: aom_codec_encode signature is (ctx, img, pts, duration, flags)
                aom_codec_encode(&ctx, &img, 0, 1, 0);

                // Re-check values after encoding
                aom_codec_control(&ctx, AOME_GET_LAST_QUANTIZER, &quantizer_val);
                aom_codec_control(&ctx, AOME_GET_LAST_QUANTIZER_64, &quantizer_val_64);
                aom_codec_control(&ctx, AV1E_GET_LUMA_CDEF_STRENGTH, &cdef_strength);

                cleanup_codec(&ctx);
            }
        }
    }

    // Initialize Decoder
    memset(&dec_cfg, 0, sizeof(dec_cfg));
    dec_cfg.threads = 1;
    dec_cfg.allow_lowbitdepth = 1;

    iface = aom_codec_av1_dx();
    if (iface) {
        err = aom_codec_dec_init(&ctx, iface, &dec_cfg, 0);
        if (err == AOM_CODEC_OK) {
            // Test AOMD_GET_BASE_Q_IDX
            aom_codec_control(&ctx, AOMD_GET_BASE_Q_IDX, &base_q_idx);

            // Test AOMD_GET_LAST_QUANTIZER
            aom_codec_control(&ctx, AOMD_GET_LAST_QUANTIZER, &quantizer_val);

            // Feed dummy data to decoder to trigger state
            if (Size > 4) {
                aom_codec_iter_t iter = NULL;
                aom_codec_decode(&ctx, Data + 4, Size - 4, NULL);
                aom_image_t *frame = aom_codec_get_frame(&ctx, &iter);
                (void)frame; // Suppress unused variable warning
                
                // Re-check values after decode attempt
                aom_codec_control(&ctx, AOMD_GET_BASE_Q_IDX, &base_q_idx);
                aom_codec_control(&ctx, AOMD_GET_LAST_QUANTIZER, &quantizer_val);
            }

            cleanup_codec(&ctx);
        }
    }

    return 0;
}