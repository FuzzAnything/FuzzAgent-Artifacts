// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
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
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx != NULL) {
        aom_codec_destroy(ctx);
    }
}

static aom_codec_ctx_t* init_encoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t res;
    int width = 16;
    int height = 16;

    if (size > 0) {
        if (data[0] < 2) width = 16;
        else if (data[0] < 4) width = 32;
        else width = 64;
        
        if (size > 1) {
            if (data[1] < 2) height = 16;
            else if (data[1] < 4) height = 32;
            else height = 64;
        }
    }

    iface = aom_codec_av1_cx();
    if (iface == NULL) return NULL;

    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return NULL;
    }

    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;

    ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (ctx == NULL) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    res = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        cleanup_codec(ctx);
        return NULL;
    }

    return ctx;
}

int LLVMFuzzerTestOneInput_61(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t *ctx = init_encoder(Data, Size);
    if (ctx == NULL) return 0;

    // Fuzz AV1E_SET_INTRA_DCT_ONLY
    int intra_dct_only = (Data[0] % 2);
    aom_codec_control(ctx, AV1E_SET_INTRA_DCT_ONLY, &intra_dct_only);

    // Fuzz AV1E_SET_ENABLE_OVERLAY
    int enable_overlay = (Data[1 % Size] % 2);
    aom_codec_control(ctx, AV1E_SET_ENABLE_OVERLAY, &enable_overlay);

    // Fuzz AV1E_SET_ENABLE_WARPED_MOTION
    int enable_warped_motion = (Data[2 % Size] % 2);
    aom_codec_control(ctx, AV1E_SET_ENABLE_WARPED_MOTION, &enable_warped_motion);

    // Fuzz AV1E_SET_ENABLE_RECT_PARTITIONS
    int enable_rect_partitions = (Data[3 % Size] % 2);
    aom_codec_control(ctx, AV1E_SET_ENABLE_RECT_PARTITIONS, &enable_rect_partitions);

    // Fuzz AV1E_SET_ENABLE_PALETTE
    int enable_palette = (Data[4 % Size] % 2);
    aom_codec_control(ctx, AV1E_SET_ENABLE_PALETTE, &enable_palette);

    // Fuzz AV1E_SET_LOOPFILTER_CONTROL
    typedef struct {
        int level;
        int sharpness;
        int mode; 
    } aom_loopfilter_t;

    aom_loopfilter_t lf_control;
    lf_control.level = Data[5 % Size] % 64;
    lf_control.sharpness = Data[6 % Size] % 8;
    lf_control.mode = (Data[7 % Size] % 2) ? 1 : 0;
    aom_codec_control(ctx, AV1E_SET_LOOPFILTER_CONTROL, &lf_control);

    // Attempt to encode a frame to exercise state
    aom_image_t img;
    aom_img_alloc(&img, AOM_IMG_FMT_I420, 16, 16, 1);
    memset(img.planes[0], Data[8 % Size], 16 * 16);
    memset(img.planes[1], Data[9 % Size], 8 * 8);
    memset(img.planes[2], Data[10 % Size], 8 * 8);

    // Use correct type aom_enc_frame_flags_t
    aom_enc_frame_flags_t flags = 0;
    if (Size > 11 && Data[11 % Size] % 2) flags |= AOM_EFLAG_FORCE_KF;

    // Fixed: Removed the 6th argument to match the function signature
    aom_codec_encode(ctx, &img, 0, 1, flags);

    aom_img_free(&img);

    cleanup_codec(ctx);
    return 0;
}