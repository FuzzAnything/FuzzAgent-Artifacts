// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aomdx.h>
#include <aom/aom_encoder.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aom_integer.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aom_external_partition.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_220(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    // Use input data to determine codec type and configuration
    int is_encoder = (Data[0] % 2);
    int frame_idx = Data[1] % 8;
    int enable_auto_altref = (Data[2] % 2);
    int s_frame_mode = Data[3] % 3;
    int max_ref_frames = (Data[1] % 8) + 1;

    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface = NULL;
    aom_codec_err_t res;

    if (is_encoder) {
        iface = aom_codec_av1_cx();
        if (!iface) return 0;

        ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
        if (!ctx) return 0;

        aom_codec_enc_cfg_t cfg;
        aom_codec_enc_config_default(iface, &cfg, 0);
        cfg.g_w = 64;
        cfg.g_h = 64;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 30;
        cfg.g_threads = 1;
        cfg.g_profile = 0;

        res = aom_codec_enc_init(ctx, iface, &cfg, 0);
        if (res != AOM_CODEC_OK) {
            cleanup_codec(ctx);
            return 0;
        }

        // Fuzz AV1E_SET_MAX_REFERENCE_FRAMES
        res = aom_codec_control(ctx, AV1E_SET_MAX_REFERENCE_FRAMES, max_ref_frames);
        (void)res;

        // Fuzz AOME_SET_ENABLEAUTOALTREF
        res = aom_codec_control(ctx, AOME_SET_ENABLEAUTOALTREF, enable_auto_altref);
        (void)res;

        // Fuzz AV1E_SET_S_FRAME_MODE
        res = aom_codec_control(ctx, AV1E_SET_S_FRAME_MODE, s_frame_mode);
        (void)res;

        // Prepare a dummy image for reference frame operations if needed
        aom_image_t img;
        aom_img_fmt_t fmt = AOM_IMG_FMT_I420;
        aom_img_alloc(&img, fmt, 64, 64, 1);
        
        av1_ref_frame_t ref_frame;
        ref_frame.idx = frame_idx;
        ref_frame.use_external_ref = 0;
        ref_frame.img = img;

        // Fuzz AV1_SET_REFERENCE (Encoder only)
        res = aom_codec_control(ctx, AV1_SET_REFERENCE, &ref_frame);
        (void)res;

        // Fuzz AV1_COPY_REFERENCE (Encoder only)
        res = aom_codec_control(ctx, AV1_COPY_REFERENCE, &ref_frame);
        (void)res;

        // Fuzz AV1_GET_REFERENCE (Encoder only)
        res = aom_codec_control(ctx, AV1_GET_REFERENCE, &ref_frame);
        (void)res;

        aom_img_free(&img);
    } else {
        iface = aom_codec_av1_dx();
        if (!iface) return 0;

        ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
        if (!ctx) return 0;

        aom_codec_dec_cfg_t cfg;
        cfg.threads = 1;
        cfg.allow_lowbitdepth = 1;

        res = aom_codec_dec_init(ctx, iface, &cfg, 0);
        if (res != AOM_CODEC_OK) {
            cleanup_codec(ctx);
            return 0;
        }

        // Decoder does not support AV1_SET_REFERENCE, AV1_COPY_REFERENCE, or AV1_GET_REFERENCE.
        // These controls are specific to the encoder's reference frame management.
        // Attempting to call them on a decoder context causes a crash due to invalid internal state access.
        // We skip these controls for the decoder path to prevent the crash.

        // Optional: Fuzz valid decoder controls if needed, e.g., AV1D_SET_DISABLE_TILE_PARTITIONING
        // res = aom_codec_control(ctx, AV1D_SET_DISABLE_TILE_PARTITIONING, 0);
        // (void)res;
    }

    cleanup_codec(ctx);
    return 0;
}