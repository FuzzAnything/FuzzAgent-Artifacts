// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aom_image.h>
#include <aom/aom_integer.h>
#include <aom/aomcx.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_187(const uint8_t *Data, size_t Size) {
    if (Size < 10) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    cfg.g_w = (Data[0] % 256) + 16;
    cfg.g_h = (Data[1] % 256) + 16;
    cfg.g_bit_depth = 8;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = (Data[2] % 60) + 1;
    cfg.rc_target_bitrate = (Data[3] % 1000) + 100;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
    memset(img.planes[0], 0, img.d_w * img.d_h);
    memset(img.planes[1], 0, (img.d_w * img.d_h) / 4);
    memset(img.planes[2], 0, (img.d_w * img.d_h) / 4);

    int disable_trellis = (Data[4] % 2);
    aom_codec_control(&ctx, AV1E_SET_DISABLE_TRELLIS_QUANT, disable_trellis);

    int qm_u = (Data[5] % 64);
    aom_codec_control(&ctx, AV1E_SET_QM_U, qm_u);

    int cdf_update_mode = (Data[6] % 2);
    aom_codec_control(&ctx, AV1E_SET_CDF_UPDATE_MODE, cdf_update_mode);

    int row_mt = (Data[7] % 2);
    aom_codec_control(&ctx, AV1E_SET_ROW_MT, row_mt);

    int fp_mt = (Data[8] % 2);
    aom_codec_control(&ctx, AV1E_SET_FP_MT, fp_mt);

    int qm_y = (Data[9] % 64);
    aom_codec_control(&ctx, AV1E_SET_QM_Y, qm_y);

    aom_enc_frame_flags_t flags = 0;
    if (Size > 10 && Data[10] % 2) {
        flags |= AOM_EFLAG_FORCE_KF;
    }

    aom_codec_encode(&ctx, &img, 0, 1, flags);

    aom_codec_iter_t iter = NULL;
    aom_codec_cx_pkt_t *pkt;
    while ((pkt = aom_codec_get_cx_data(&ctx, &iter)) != NULL) {
        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
            // Process encoded data if needed
        }
    }

    aom_img_free(&img);
    cleanup_codec(&ctx);

    return 0;
}