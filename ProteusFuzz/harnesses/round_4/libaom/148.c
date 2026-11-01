// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aomdx.h"
#include "aom_encoder.h"
#include "aom_frame_buffer.h"
#include "aom_decoder.h"
#include "aom_image.h"
#include "aom.h"
#include "aom_integer.h"
#include "aom_codec.h"
#include "aomcx.h"
#include "aom_external_partition.h"

static int init_encoder(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t ret;
    aom_image_t img;
    aom_codec_cx_pkt_t *pkt;
    aom_codec_iter_t iter = NULL;

    if (!ctx) return -1;

    iface = aom_codec_av1_cx();
    if (!iface) return -1;

    ret = aom_codec_enc_config_default(iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) return -1;

    // Use fuzzing data to configure width/height safely
    if (size >= 8) {
        uint32_t w = *(uint32_t *)(data);
        uint32_t h = *(uint32_t *)(data + 4);
        // Sanity check to avoid huge allocations
        if (w > 0 && w < 4096 && h > 0 && h < 4096) {
            cfg.g_w = w;
            cfg.g_h = h;
        } else {
            cfg.g_w = 128;
            cfg.g_h = 128;
        }
    } else {
        cfg.g_w = 128;
        cfg.g_h = 128;
    }

    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;

    ret = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) return -1;

    // Create a dummy frame
    aom_img_alloc(&img, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);

    // Fill frame with fuzzing data
    if (size >= img.planes[0][0]) {
        memcpy(img.planes[0], data, img.planes[0][0]);
    } else if (size > 0) {
        memset(img.planes[0], 0xAA, img.planes[0][0]);
    }

    // Encode one frame
    ret = aom_codec_encode(ctx, &img, 0, 1, 0);
    if (ret != AOM_CODEC_OK) {
        aom_img_free(&img);
        aom_codec_destroy(ctx);
        return -1;
    }

    // Check for output packets
    while ((pkt = aom_codec_get_cx_data(ctx, &iter)) != NULL) {
        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
            // Just consume
        }
    }

    aom_img_free(&img);
    return 0;
}

static int init_decoder(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface;
    aom_codec_dec_cfg_t cfg;
    aom_codec_err_t ret;
    aom_codec_iter_t iter = NULL;
    aom_image_t *img = NULL;

    if (!ctx) return -1;

    iface = aom_codec_av1_dx();
    if (!iface) return -1;

    // Initialize config manually
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    // Removed invalid members: allow_high_bitdepth, allow_multiref, allow_intrabc, allow_screen_content_tools
    // Only using valid members like allow_lowbitdepth if needed, but keeping default 0 is fine for fuzzing

    ret = aom_codec_dec_init(ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) return -1;

    // Feed some data if available (even if invalid, to trigger state)
    if (size > 0) {
        aom_codec_decode(ctx, data, size, NULL);
    }

    // Try to get a frame
    while ((img = aom_codec_get_frame(ctx, &iter)) != NULL) {
        // Do nothing with image, just ensure state is reached
        aom_img_free(img);
    }

    return 0;
}

int LLVMFuzzerTestOneInput_148(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t enc_ctx = {0};
    aom_codec_ctx_t dec_ctx = {0};
    aom_codec_err_t ret;
    int quantizer;
    int64_t quantizer64;
    int base_q_idx;
    int cq_level;
    int one_pass_q;

    // Initialize Encoder
    if (init_encoder(&enc_ctx, Data, Size) == 0) {
        // Test 1: AOME_GET_LAST_QUANTIZER
        ret = aom_codec_control(&enc_ctx, AOME_GET_LAST_QUANTIZER, &quantizer);
        (void)ret;

        // Test 2: AOME_GET_LAST_QUANTIZER_64
        ret = aom_codec_control(&enc_ctx, AOME_GET_LAST_QUANTIZER_64, &quantizer64);
        (void)ret;

        // Test 3: AOME_SET_CQ_LEVEL (Set to a value derived from input)
        if (Size > 0) {
            cq_level = Data[0] % 100; // Clamp to reasonable range
            ret = aom_codec_control(&enc_ctx, AOME_SET_CQ_LEVEL, cq_level);
            (void)ret;
        }

        // Test 4: AV1E_SET_QUANTIZER_ONE_PASS
        if (Size > 1) {
            one_pass_q = (Data[0] * 256 + Data[1]) % 256;
            ret = aom_codec_control(&enc_ctx, AV1E_SET_QUANTIZER_ONE_PASS, one_pass_q);
            (void)ret;
        }

        aom_codec_destroy(&enc_ctx);
    }

    // Initialize Decoder
    if (init_decoder(&dec_ctx, Data, Size) == 0) {
        // Test 5: AOMD_GET_BASE_Q_IDX
        ret = aom_codec_control(&dec_ctx, AOMD_GET_BASE_Q_IDX, &base_q_idx);
        (void)ret;

        // Test 6: AOMD_GET_LAST_QUANTIZER
        ret = aom_codec_control(&dec_ctx, AOMD_GET_LAST_QUANTIZER, &quantizer);
        (void)ret;

        aom_codec_destroy(&dec_ctx);
    }

    return 0;
}