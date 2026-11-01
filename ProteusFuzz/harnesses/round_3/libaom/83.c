// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_decoder.h>
#include <aom/aom.h>
#include <aom/aomcx.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_frame_buffer.h>

static aom_codec_ctx_t *init_encoder_context(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface = NULL;
    aom_codec_enc_cfg_t cfg;
    int err;

    iface = aom_codec_av1_cx();
    if (!iface) return NULL;

    ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    err = aom_codec_enc_config_default(iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    // Use fuzzing data to configure width/height within reasonable bounds
    if (size >= 4) {
        uint32_t w = ((uint32_t)data[0] << 8) | data[1];
        uint32_t h = ((uint32_t)data[2] << 8) | data[3];
        if (w == 0) w = 16;
        if (h == 0) h = 16;
        // Limit to reasonable max to avoid OOM
        if (w > 4096) w = 4096;
        if (h > 4096) h = 4096;
        cfg.g_w = w;
        cfg.g_h = h;
    } else {
        cfg.g_w = 16;
        cfg.g_h = 16;
    }

    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.g_profile = 0;

    err = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

static void cleanup_encoder_context(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_83(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    aom_codec_ctx_t *ctx = init_encoder_context(Data, Size);
    if (!ctx) return 0;

    // Fuzz various control commands
    // Note: We pass the enum value directly as the control ID.
    // The library expects the raw enum value for these specific controls.
    
    // 1. AV1E_SET_ENABLE_REF_FRAME_MVS
    int val = Data[0] & 0xFF;
    aom_codec_control(ctx, AV1E_SET_ENABLE_REF_FRAME_MVS, val);

    // 2. AV1E_SET_COLOR_PRIMARIES
    int cp = Data[1] & 0xFF;
    aom_codec_control(ctx, AV1E_SET_COLOR_PRIMARIES, cp);

    // 3. AV1E_SET_FRAME_PERIODIC_BOOST
    int boost = Data[2] & 0xFF;
    aom_codec_control(ctx, AV1E_SET_FRAME_PERIODIC_BOOST, boost);

    // 4. AV1E_SET_ENABLE_SUPERRES
    int superres = Data[3] & 0xFF;
    aom_codec_control(ctx, AV1E_SET_ENABLE_SUPERRES, superres);

    // 5. AV1E_SET_ENABLE_FILTER_INTRA
    int filter_intra = Data[4] & 0xFF;
    aom_codec_control(ctx, AV1E_SET_ENABLE_FILTER_INTRA, filter_intra);

    // 6. AV1E_SET_ALLOW_WARPED_MOTION
    int warped = Data[5] & 0xFF;
    aom_codec_control(ctx, AV1E_SET_ALLOW_WARPED_MOTION, warped);

    // Attempt to encode a dummy frame to exercise the codec state
    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, ctx->config.enc->g_w, ctx->config.enc->g_h, 1);
    
    // Fill image with fuzzing data
    size_t bytes_per_row = img.stride[0];
    for (int i = 0; i < img.d_h; i++) {
        size_t copy_len = (Size > bytes_per_row) ? bytes_per_row : Size;
        memcpy(img.planes[0] + i * bytes_per_row, Data, copy_len);
        // UV planes can be zeroed or filled similarly
        memset(img.planes[1] + i * img.stride[1], 0, img.stride[1]);
        memset(img.planes[2] + i * img.stride[2], 0, img.stride[2]);
    }

    // Corrected call: aom_codec_encode(ctx, img, pts, duration, flags)
    aom_codec_encode(ctx, &img, 0, 1, 0);

    // Drain remaining packets
    aom_codec_iter_t iter = NULL;
    const aom_codec_cx_pkt_t *pkt;
    while ((pkt = aom_codec_get_cx_data(ctx, &iter)) != NULL) {
        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
            // Packet data is managed by libaom, no manual free needed here
        }
    }

    aom_img_free(&img);
    cleanup_encoder_context(ctx);

    return 0;
}