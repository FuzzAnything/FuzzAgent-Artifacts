// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aom_integer.h>
#include <aom/aom_codec.h>
#include <aom/aom_external_partition.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_130(const uint8_t *Data, size_t Size) {
    if (Size < 11) {
        return 0;
    }

    // Extract parameters from input data
    int width = Data[0] + 1;
    int height = Data[1] + 1;
    int bit_depth = (Data[2] % 2) + 8; // 8 or 10
    int color_space = Data[3];

    // Initialize codec context
    aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) {
        return 0;
    }
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    // Get AV1 encoder interface
    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        cleanup_codec(ctx);
        return 0;
    }

    // Initialize encoder configuration
    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);
    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_bit_depth = bit_depth;
    // cfg.g_color_space is not a valid member in this struct version, skipping
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;

    // Initialize encoder context
    if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        cleanup_codec(ctx);
        return 0;
    }

    // Fuzz various control functions
    // 1. AV1E_SET_INTRA_DCT_ONLY
    int intra_dct_only = (Data[4] % 2);
    aom_codec_control(ctx, AV1E_SET_INTRA_DCT_ONLY, intra_dct_only);

    // 2. AV1E_SET_FRAME_PERIODIC_BOOST
    int boost_period = (Data[5] % 100) + 1;
    int boost_qd = (Data[6] % 64);
    aom_codec_control(ctx, AV1E_SET_FRAME_PERIODIC_BOOST, boost_period, boost_qd);

    // 3. AV1E_SET_FORCE_VIDEO_MODE
    int force_video_mode = (Data[7] % 2);
    aom_codec_control(ctx, AV1E_SET_FORCE_VIDEO_MODE, force_video_mode);

    // 4. AV1E_SET_COLOR_PRIMARIES
    int color_primaries = Data[8] % 256;
    aom_codec_control(ctx, AV1E_SET_COLOR_PRIMARIES, color_primaries);

    // 5. AV1E_SET_ENABLE_CHROMA_DELTAQ
    int enable_chroma_deltaq = (Data[9] % 2);
    aom_codec_control(ctx, AV1E_SET_ENABLE_CHROMA_DELTAQ, enable_chroma_deltaq);

    // 6. AV1E_SET_ENABLE_SUPERRES
    int enable_superres = (Data[10] % 2);
    aom_codec_control(ctx, AV1E_SET_ENABLE_SUPERRES, enable_superres);

    // Encode a dummy frame
    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, width, height, 1);
    memset(img.planes[0], 128, width * height);
    memset(img.planes[1], 128, (width * height) / 4);
    memset(img.planes[2], 128, (width * height) / 4);

    aom_enc_frame_flags_t flags = 0;
    aom_codec_encode(ctx, &img, 0, 1, flags);

    // Cleanup
    aom_img_free(&img);
    cleanup_codec(ctx);

    return 0;
}