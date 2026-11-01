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
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aomcx.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_199(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    uint32_t width = (Data[0] % 128) + 16;
    uint32_t height = (Data[1] % 128) + 16;
    uint32_t fps_num = (Data[2] % 120) + 1;
    uint32_t fps_den = (Data[3] % 120) + 1;

    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = fps_num;
    cfg.g_timebase.den = fps_den;
    cfg.rc_target_bitrate = 1000;
    cfg.rc_end_usage = AOM_CBR;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    aom_image_t img;
    if (aom_img_alloc(&img, AOM_IMG_FMT_I420, width, height, 1) != 0) {
        cleanup_codec(&ctx);
        return 0;
    }

    if (img.planes[0]) {
        for (int i = 0; i < 10 && i < Size; i++) {
            img.planes[0][i] = Data[i % Size];
        }
    }

    int val1 = Data[4 % Size];
    aom_codec_control(&ctx, AV1E_SET_ENABLE_CFL_INTRA, val1);

    int val2 = Data[(4 + 1) % Size];
    aom_codec_control(&ctx, AV1E_SET_SCREEN_CONTENT_DETECTION_MODE, val2);

    int val3 = Data[(4 + 2) % Size];
    aom_codec_control(&ctx, AV1E_SET_ENABLE_INTRABC, val3);

    if (Size > 10) {
        int val4 = Data[(4 + 3) % Size];
        aom_codec_control(&ctx, AV1E_SET_FILM_GRAIN_TEST_VECTOR, &val4);
    }

    int val5 = Data[(4 + 4) % Size];
    aom_codec_control(&ctx, AV1E_SET_ENABLE_DIST_WTD_COMP, val5);

    int val6 = Data[(4 + 5) % Size];
    aom_codec_control(&ctx, AV1E_SET_ENABLE_ANGLE_DELTA, val6);

    aom_codec_cx_pkt_t pkt;
    memset(&pkt, 0, sizeof(pkt));
    
    // Fixed: aom_codec_encode takes 5 arguments (ctx, img, pts, duration, flags)
    aom_codec_encode(&ctx, &img, 0, 1, 0);
    
    while (aom_codec_get_cx_data(&ctx, &pkt) == AOM_CODEC_OK) {
        if (pkt.kind == AOM_CODEC_CX_FRAME_PKT) {
            // Consume packet
        }
    }

    aom_img_free(&img);
    cleanup_codec(&ctx);

    return 0;
}