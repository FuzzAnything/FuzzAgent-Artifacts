// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom.h"
#include "aom_codec.h"
#include "aom_encoder.h"
#include "aom_image.h"
#include "aom_integer.h"
#include "aomcx.h"
#include "aomdx.h"
#include "aom_external_partition.h"
#include "aom_decoder.h"
#include "aom_frame_buffer.h"

static int fuzz_encoder_controls(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx || !data || size == 0) return 0;

    int val;
    int ret;

    // AV1E_SET_INTER_DCT_ONLY
    if (size >= sizeof(int)) {
        memcpy(&val, data, sizeof(int));
        ret = aom_codec_control(ctx, AV1E_SET_INTER_DCT_ONLY, val);
        (void)ret;
    }

    // AV1E_SET_ENABLE_INTERINTER_WEDGE
    if (size >= sizeof(int) + 1) {
        memcpy(&val, data + sizeof(int), sizeof(int));
        ret = aom_codec_control(ctx, AV1E_SET_ENABLE_INTERINTER_WEDGE, val);
        (void)ret;
    }

    // AV1E_SET_ENABLE_AB_PARTITIONS
    if (size >= sizeof(int) * 2) {
        memcpy(&val, data + sizeof(int), sizeof(int));
        ret = aom_codec_control(ctx, AV1E_SET_ENABLE_AB_PARTITIONS, val);
        (void)ret;
    }

    // AV1E_SET_ENABLE_MASKED_COMP
    if (size >= sizeof(int) * 3) {
        memcpy(&val, data + sizeof(int) * 2, sizeof(int));
        ret = aom_codec_control(ctx, AV1E_SET_ENABLE_MASKED_COMP, val);
        (void)ret;
    }

    // AV1E_SET_ENABLE_SMOOTH_INTERINTRA
    if (size >= sizeof(int) * 4) {
        memcpy(&val, data + sizeof(int) * 3, sizeof(int));
        ret = aom_codec_control(ctx, AV1E_SET_ENABLE_SMOOTH_INTERINTRA, val);
        (void)ret;
    }

    // AV1E_SET_REDUCED_TX_TYPE_SET
    if (size >= sizeof(int) * 5) {
        memcpy(&val, data + sizeof(int) * 4, sizeof(int));
        ret = aom_codec_control(ctx, AV1E_SET_REDUCED_TX_TYPE_SET, val);
        (void)ret;
    }

    return 0;
}

int LLVMFuzzerTestOneInput_110(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_image_t img;
    const aom_codec_cx_pkt_t *pkt;
    aom_codec_err_t res;

    // Initialize context
    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));
    memset(&img, 0, sizeof(img));

    // Get AV1 encoder interface
    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    // Set default configuration
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use fuzz data to set width/height if possible, otherwise use defaults
    if (Size >= 8) {
        uint32_t w = *(const uint32_t *)(Data);
        uint32_t h = *(const uint32_t *)(Data + 4);
        if (w > 0 && w < 4096 && h > 0 && h < 4096) {
            cfg.g_w = w;
            cfg.g_h = h;
        }
    }
    // Ensure valid defaults if data is insufficient
    if (cfg.g_w == 0) cfg.g_w = 64;
    if (cfg.g_h == 0) cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;
    cfg.g_threads = 1;
    cfg.g_error_resilient = 0;

    // Initialize encoder
    res = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        return 0;
    }

    // Perform fuzzing on controls
    fuzz_encoder_controls(&ctx, Data, Size);

    // Attempt to encode a dummy frame to exercise state
    // Allocate image
    if (aom_img_alloc(&img, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1) != 0) {
        aom_codec_destroy(&ctx);
        return 0;
    }

    // Fill image with fuzz data
    // img.d is not a member, use stride and height
    size_t plane_size = (size_t)img.stride[0] * img.h;
    size_t copy_size = Size > plane_size ? plane_size : Size;
    if (copy_size > 0) {
        memcpy(img.planes[0], Data, copy_size);
        // Simple downscaling for chroma planes
        int h_chroma = img.h / 2;
        int w_chroma = img.w / 2;
        for (int y = 0; y < h_chroma; y++) {
            for (int x = 0; x < w_chroma; x++) {
                img.planes[1][y * img.stride[1] + x] = img.planes[0][y * 2 * img.stride[0] + x * 2];
                img.planes[2][y * img.stride[2] + x] = img.planes[0][y * 2 * img.stride[0] + x * 2];
            }
        }
    }

    aom_codec_frame_flags_t flags = 0;
    res = aom_codec_encode(&ctx, &img, 0, 1, flags);
    if (res != AOM_CODEC_OK) {
        aom_img_free(&img);
        aom_codec_destroy(&ctx);
        return 0;
    }

    // Drain encoder
    while ((pkt = aom_codec_get_cx_data(&ctx, &pkt)) != NULL) {
        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
            // Process frame packet (ignore for fuzzing)
        }
    }

    // Cleanup
    aom_img_free(&img);
    aom_codec_destroy(&ctx);

    return 0;
}