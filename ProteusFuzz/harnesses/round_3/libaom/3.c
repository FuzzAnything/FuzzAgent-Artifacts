// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>

static int init_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg, int width, int height) {
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) return 0;

    cfg->g_w = width;
    cfg->g_h = height;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_error_resilient = 1;
    cfg->rc_target_bitrate = 1000;

    if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) {
        aom_codec_destroy(ctx);
        return 0;
    }
    return 1;
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    aom_codec_destroy(ctx);
}

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    int width = (Data[0] % 16 + 1) * 16;
    int height = (Data[1] % 16 + 1) * 16;
    if (width > 4096 || height > 4096) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    if (!init_encoder(&ctx, &cfg, width, height)) return 0;

    // Create a dummy frame
    aom_image_t *img = aom_img_alloc(NULL, AOM_IMG_FMT_I420, width, height, 1);
    if (!img) {
        cleanup_encoder(&ctx);
        return 0;
    }

    // Fill frame with fuzz data
    for (int i = 0; i < img->h; i++) {
        for (int j = 0; j < img->w; j++) {
            size_t offset = (i * img->stride[0]) + j;
            if (offset < Size) {
                img->planes[0][offset] = Data[offset];
            }
        }
    }
    
    // Fill UV planes
    int uv_h = (height + 1) / 2;
    int uv_w = (width + 1) / 2;
    for (int i = 0; i < uv_h; i++) {
        for (int j = 0; j < uv_w; j++) {
            size_t offset = (i * img->stride[1]) + j;
            if (offset < Size) {
                img->planes[1][offset] = Data[(offset + width * height) % Size];
                img->planes[2][offset] = Data[(offset + width * height + 1) % Size];
            }
        }
    }

    int val = 0;
    int ret = AOM_CODEC_OK;

    // Fuzz AV1E_SET_REDUCED_REFERENCE_SET
    if (Size > 2) {
        val = Data[2] % 2;
        ret = aom_codec_control(&ctx, AV1E_SET_REDUCED_REFERENCE_SET, val);
    }

    // Fuzz AV1E_SET_ENABLE_RECT_PARTITIONS
    if (Size > 3) {
        val = Data[3] % 2;
        ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_RECT_PARTITIONS, val);
    }

    // Fuzz AV1E_SET_ENABLE_PALETTE
    if (Size > 4) {
        val = Data[4] % 2;
        ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_PALETTE, val);
    }

    // Fuzz AV1E_SET_QM_MIN
    if (Size > 5) {
        val = Data[5];
        ret = aom_codec_control(&ctx, AV1E_SET_QM_MIN, val);
    }

    // Fuzz AV1E_SET_CHROMA_SAMPLE_POSITION
    if (Size > 6) {
        val = Data[6] % 3; // Assuming valid range 0-2
        ret = aom_codec_control(&ctx, AV1E_SET_CHROMA_SAMPLE_POSITION, val);
    }

    // Fuzz AV1E_SET_LOSSLESS
    if (Size > 7) {
        val = Data[7] % 2;
        ret = aom_codec_control(&ctx, AV1E_SET_LOSSLESS, val);
    }

    // Encode the frame to ensure state consistency
    // Fixed: aom_codec_encode expects 5 arguments: ctx, img, pts, duration, flags
    aom_codec_encode(&ctx, img, 0, 1, 0);
    
    aom_codec_iter_t iter = NULL;
    aom_codec_cx_pkt_t *pkt;
    while ((pkt = aom_codec_get_cx_data(&ctx, &iter)) != NULL) {
        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
            // Process packet if needed
        }
    }

    aom_img_free(img);
    cleanup_encoder(&ctx);
    return 0;
}