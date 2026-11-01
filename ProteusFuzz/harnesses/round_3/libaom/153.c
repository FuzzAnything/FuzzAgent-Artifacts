// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_free at aom_image.c:304:6 in aom_image.h
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
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom_image.h>
#include <aom/aom_integer.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_153(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface = NULL;
    aom_codec_enc_cfg_t cfg;
    aom_image_t img;
    int ret;

    // Select encoder interface using the correct function name
    iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) {
        return 0;
    }

    // Initialize default config
    aom_codec_enc_config_default(iface, &cfg, 0);
    cfg.g_w = 16;
    cfg.g_h = 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;

    // Initialize codec context
    ret = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        cleanup_codec(ctx);
        return 0;
    }

    // Prepare a dummy image frame
    if (aom_img_alloc(&img, AOM_IMG_FMT_I420, 16, 16, 1) != 0) {
        cleanup_codec(ctx);
        return 0;
    }
    
    // Fill image planes with fuzz data
    if (img.planes[0]) {
        memset(img.planes[0], Data[0] & 0xFF, 16 * 16);
    }
    if (img.planes[1]) {
        memset(img.planes[1], Data[1] & 0xFF, 8 * 8);
    }
    if (img.planes[2]) {
        memset(img.planes[2], Data[1] & 0xFF, 8 * 8);
    }

    // Fuzz control commands based on input data
    int val = (Size > 2) ? (Data[2] % 2) : 0;
    int val2 = (Size > 3) ? (Data[3] % 2) : 0;

    // AOME_SET_SHARPNESS
    aom_codec_control(ctx, AOME_SET_SHARPNESS, (val * 10));

    // AV1E_SET_ENABLE_INTERINTER_WEDGE
    aom_codec_control(ctx, AV1E_SET_ENABLE_INTERINTER_WEDGE, val);

    // AV1E_SET_ENABLE_OBMC
    aom_codec_control(ctx, AV1E_SET_ENABLE_OBMC, val2);

    // AV1E_SET_ENABLE_AB_PARTITIONS
    aom_codec_control(ctx, AV1E_SET_ENABLE_AB_PARTITIONS, val);

    // AOME_SET_ENABLEAUTOBWDREF
    aom_codec_control(ctx, AOME_SET_ENABLEAUTOBWDREF, val2);

    // AV1E_SET_ALLOW_WARPED_MOTION
    aom_codec_control(ctx, AV1E_SET_ALLOW_WARPED_MOTION, val);

    // Encode a frame to exercise the state (fixed argument count)
    aom_codec_encode(ctx, &img, 0, 1, 0);

    // Cleanup
    aom_img_free(&img);
    cleanup_codec(ctx);

    return 0;
}