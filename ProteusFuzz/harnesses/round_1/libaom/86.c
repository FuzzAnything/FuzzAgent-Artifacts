// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_decoder.h>
#include <aom/aomdx.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

static void cleanup_decoder(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_86(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Initialize encoder context
    aom_codec_ctx_t enc_ctx;
    memset(&enc_ctx, 0, sizeof(enc_ctx));
    aom_codec_iface_t *enc_iface = aom_codec_av1_cx();
    if (!enc_iface) return 0;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(enc_iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.g_error_resilient = 0;

    if (aom_codec_enc_init(&enc_ctx, enc_iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Initialize decoder context
    aom_codec_ctx_t dec_ctx;
    memset(&dec_ctx, 0, sizeof(dec_ctx));
    aom_codec_iface_t *dec_iface = aom_codec_av1_dx();
    if (!dec_iface) {
        cleanup_encoder(&enc_ctx);
        return 0;
    }

    aom_codec_dec_cfg_t dec_cfg;
    memset(&dec_cfg, 0, sizeof(dec_cfg));
    if (aom_codec_dec_init(&dec_ctx, dec_iface, &dec_cfg, 0) != AOM_CODEC_OK) {
        cleanup_encoder(&enc_ctx);
        return 0;
    }

    // Prepare dummy image data for encoding
    aom_img_fmt_t fmt = AOM_IMG_FMT_I420;
    aom_image_t img;
    memset(&img, 0, sizeof(img));
    if (aom_img_alloc(&img, fmt, 64, 64, 1) != 0) {
        cleanup_encoder(&enc_ctx);
        cleanup_decoder(&dec_ctx);
        return 0;
    }

    // Fill image with fuzz data if available
    if (Size >= 64 * 64 * 3 / 2) {
        memcpy(img.planes[0], Data, 64 * 64);
        memcpy(img.planes[1], Data + 64 * 64, 32 * 32);
        memcpy(img.planes[2], Data + 64 * 64 + 32 * 32, 32 * 32);
    } else {
        memset(img.planes[0], 0, 64 * 64);
        memset(img.planes[1], 0, 32 * 32);
        memset(img.planes[2], 0, 32 * 32);
    }

    // Encode a frame
    aom_codec_control(&enc_ctx, AOME_SET_CPUUSED, 0);
    // Fixed: Use 5 arguments as per aom_codec_encode signature
    aom_codec_encode(&enc_ctx, &img, 0, 1, 0);

    // Decode the encoded data (optional, just to exercise decoder path)
    // AOMD_SET_CPUUSED is not a standard control, so we omit it.

    // Fuzz target functions with typechecked controls
    // Note: These functions are internal and may not be directly callable in all builds.
    // We attempt to call them via aom_codec_control with specific control IDs.
    // The actual control IDs are not provided, so we use placeholders.

    // Attempt to call AOME_GET_LAST_QUANTIZER
    int quantizer = 0;
    aom_codec_control(&enc_ctx, AOME_GET_LAST_QUANTIZER, &quantizer);

    // Attempt to call AOMD_GET_LAST_QUANTIZER
    int dec_quantizer = 0;
    aom_codec_control(&dec_ctx, AOMD_GET_LAST_QUANTIZER, &dec_quantizer);

    // Attempt to call AOME_GET_LAST_QUANTIZER_64
    int64_t quantizer64 = 0;
    aom_codec_control(&enc_ctx, AOME_GET_LAST_QUANTIZER_64, &quantizer64);

    // Attempt to call AV1E_SET_QUANTIZER_ONE_PASS
    int q_idx = (Size % 64);
    aom_codec_control(&enc_ctx, AV1E_SET_QUANTIZER_ONE_PASS, q_idx);

    // Attempt to call AOME_SET_CQ_LEVEL
    int cq_level = (Size % 64);
    aom_codec_control(&enc_ctx, AOME_SET_CQ_LEVEL, cq_level);

    // Attempt to call AOMD_GET_BASE_Q_IDX
    int base_q_idx = 0;
    aom_codec_control(&dec_ctx, AOMD_GET_BASE_Q_IDX, &base_q_idx);

    // Cleanup
    aom_img_free(&img);
    cleanup_encoder(&enc_ctx);
    cleanup_decoder(&dec_ctx);

    return 0;
}