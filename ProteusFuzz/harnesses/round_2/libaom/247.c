// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aom_decoder.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>
#include <aom/aomcx.h>
#include <aom/aom_image.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom.h>

static int fuzz_encode(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx || !data || size == 0) return 0;

    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, 64, 64, 1);

    if (size < img.d_w * img.d_h * 3 / 2) {
        aom_img_free(&img);
        return 0;
    }

    memcpy(img.planes[0], data, img.d_w * img.d_h);
    memcpy(img.planes[1], data + img.d_w * img.d_h, img.d_w * img.d_h / 4);
    memcpy(img.planes[2], data + img.d_w * img.d_h * 5 / 4, img.d_w * img.d_h / 4);

    aom_codec_enc_cfg_t cfg;
    // Use aom_codec_av1_cx() to get the interface, then pass it to config_default
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        aom_img_free(&img);
        return 0;
    }
    
    aom_codec_enc_config_default(&cfg, iface, 0);
    cfg.g_w = img.d_w;
    cfg.g_h = img.d_h;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;

    if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        aom_img_free(&img);
        return 0;
    }

    // Use the correct control IDs defined in aomcx.h
    aom_codec_control(ctx, AV1E_SET_LOSSLESS, 1);
    aom_codec_control(ctx, AV1E_SET_ENABLE_CDEF, 1);
    aom_codec_control(ctx, AV1E_SET_TIER_MASK, 1);
    aom_codec_control(ctx, AV1E_SET_MTU, 1500);
    aom_codec_control(ctx, AV1E_SET_FORCE_VIDEO_MODE, 1);
    aom_codec_control(ctx, AV1E_SET_ENABLE_INTERINTER_WEDGE, 1);

    // Toggle settings to explore different states
    aom_codec_control(ctx, AV1E_SET_LOSSLESS, 0);
    aom_codec_control(ctx, AV1E_SET_ENABLE_CDEF, 0);
    aom_codec_control(ctx, AV1E_SET_TIER_MASK, 0);
    aom_codec_control(ctx, AV1E_SET_MTU, 1400);
    aom_codec_control(ctx, AV1E_SET_FORCE_VIDEO_MODE, 0);
    aom_codec_control(ctx, AV1E_SET_ENABLE_INTERINTER_WEDGE, 0);

    aom_codec_encode(ctx, &img, 0, 1, 0);

    aom_img_free(&img);
    return 1;
}

int LLVMFuzzerTestOneInput_247(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    if (fuzz_encode(&ctx, Data, Size) == 0) {
        return 0;
    }

    aom_codec_destroy(&ctx);
    return 0;
}