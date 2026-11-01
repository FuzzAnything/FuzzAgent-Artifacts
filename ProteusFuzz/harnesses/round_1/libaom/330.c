// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_decoder.h>
#include <aom/aomdx.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
    if (ctx) {
        aom_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_330(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface = NULL;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t ret;

    // Select encoder or decoder based on first byte
    int is_encoder = Data[0] & 0x01;

    if (is_encoder) {
        iface = aom_codec_av1_cx();
        if (!iface) return 0;

        ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
        if (!ctx) return 0;
        memset(ctx, 0, sizeof(aom_codec_ctx_t));

        ret = aom_codec_enc_config_default(iface, &cfg, 0);
        if (ret != AOM_CODEC_OK) {
            cleanup_codec(ctx);
            return 0;
        }

        // Use fuzz data to tweak config slightly
        cfg.g_w = (Data[1] % 512) + 16;
        cfg.g_h = ((Data[1] >> 1) % 512) + 16;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 30;
        cfg.g_threads = 1;

        ret = aom_codec_enc_init(ctx, iface, &cfg, 0);
        if (ret != AOM_CODEC_OK) {
            cleanup_codec(ctx);
            return 0;
        }

        // Fuzz AV1E_SET_QM_MAX
        int qm_max = (Data[1] % 64) + 1;
        ret = aom_codec_control(ctx, AV1E_SET_QM_MAX, &qm_max);
        (void)ret;

        // Fuzz AOME_SET_ENABLEAUTOALTREF
        int enable_auto_altref = (Data[1] & 0x01);
        ret = aom_codec_control(ctx, AOME_SET_ENABLEAUTOALTREF, &enable_auto_altref);
        (void)ret;

        // Fuzz AV1E_SET_QM_Y
        int qm_y = (Data[1] % 64) + 1;
        ret = aom_codec_control(ctx, AV1E_SET_QM_Y, &qm_y);
        (void)ret;

        // Fuzz AOME_SET_CQ_LEVEL
        int cq_level = (Data[1] % 64) + 1;
        ret = aom_codec_control(ctx, AOME_SET_CQ_LEVEL, &cq_level);
        (void)ret;

        // Fuzz AV1_SET_REFERENCE (requires av1_ref_frame_t)
        av1_ref_frame_t ref_frame;
        ref_frame.idx = Data[1] % 3; // Valid indices 0-2
        ref_frame.use_external_ref = 0;
        memset(&ref_frame.img, 0, sizeof(aom_image_t));
        ret = aom_codec_control(ctx, AV1_SET_REFERENCE, &ref_frame);
        (void)ret;

    } else {
        iface = aom_codec_av1_dx();
        if (!iface) return 0;

        ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
        if (!ctx) return 0;
        memset(ctx, 0, sizeof(aom_codec_ctx_t));

        aom_codec_dec_cfg_t dec_cfg;
        memset(&dec_cfg, 0, sizeof(aom_codec_dec_cfg_t));
        dec_cfg.threads = 1;

        ret = aom_codec_dec_init(ctx, iface, &dec_cfg, 0);
        if (ret != AOM_CODEC_OK) {
            cleanup_codec(ctx);
            return 0;
        }

        // AV1_GET_REFERENCE requires a valid frame to be decoded first.
        // Calling it immediately after init on an uninitialized context causes a crash.
        // We skip this control call to prevent the null pointer dereference.
        // In a real fuzzing scenario, one would decode a packet first.
        
        // Fuzz AV1_GET_REFERENCE (Removed to prevent crash)
        // av1_ref_frame_t ref_frame;
        // ref_frame.idx = Data[1] % 3;
        // ref_frame.use_external_ref = 0;
        // memset(&ref_frame.img, 0, sizeof(aom_image_t));
        // ret = aom_codec_control(ctx, AV1_GET_REFERENCE, &ref_frame);
        // (void)ret;
    }

    cleanup_codec(ctx);
    return 0;
}