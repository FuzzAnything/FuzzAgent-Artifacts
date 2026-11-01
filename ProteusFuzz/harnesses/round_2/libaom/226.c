// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
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
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aom_image.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>

static int fuzz_encoder_controls(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx) return 0;

    int val;
    aom_codec_err_t res;

    // AV1E_SET_CHROMA_SUBSAMPLING_X
    if (size > 0) {
        val = (int)(data[0] % 3); // 0, 1, or 2
        res = aom_codec_control(ctx, AV1E_SET_CHROMA_SUBSAMPLING_X, val);
        (void)res;
    }

    // AV1E_SET_CHROMA_SUBSAMPLING_Y
    if (size > 1) {
        val = (int)(data[1] % 3);
        res = aom_codec_control(ctx, AV1E_SET_CHROMA_SUBSAMPLING_Y, val);
        (void)res;
    }

    // AV1E_SET_ENABLE_CDEF
    if (size > 2) {
        val = (int)(data[2] % 2);
        res = aom_codec_control(ctx, AV1E_SET_ENABLE_CDEF, val);
        (void)res;
    }

    // AV1E_SET_ENABLE_KEYFRAME_FILTERING
    if (size > 3) {
        val = (int)(data[3] % 2);
        res = aom_codec_control(ctx, AV1E_SET_ENABLE_KEYFRAME_FILTERING, val);
        (void)res;
    }

    // AV1E_SET_ENABLE_INTERINTER_WEDGE
    if (size > 4) {
        val = (int)(data[4] % 2);
        res = aom_codec_control(ctx, AV1E_SET_ENABLE_INTERINTER_WEDGE, val);
        (void)res;
    }

    return 1;
}

static int fuzz_decoder_controls(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx) return 0;

    int bit_depth;
    aom_codec_err_t res;

    // AV1D_GET_BIT_DEPTH
    res = aom_codec_control(ctx, AV1D_GET_BIT_DEPTH, &bit_depth);
    (void)res;
    (void)data;
    (void)size;

    return 1;
}

int LLVMFuzzerTestOneInput_226(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Determine if we should fuzz encoder or decoder based on input
    int is_encoder = (Data[0] % 2) == 0;

    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface;
    aom_codec_err_t res;
    aom_codec_enc_cfg_t cfg;
    aom_codec_dec_cfg_t dec_cfg;

    memset(&ctx, 0, sizeof(ctx));

    if (is_encoder) {
        // Initialize Encoder
        iface = aom_codec_av1_cx();
        if (!iface) return 0;

        res = aom_codec_enc_config_default(iface, &cfg, 0);
        if (res != AOM_CODEC_OK) return 0;

        cfg.g_w = 16;
        cfg.g_h = 16;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 30;
        cfg.g_threads = 1;
        cfg.g_usage = AOM_USAGE_REALTIME;
        cfg.rc_target_bitrate = 1000;

        res = aom_codec_enc_init(&ctx, iface, &cfg, 0);
        if (res != AOM_CODEC_OK) {
            aom_codec_destroy(&ctx);
            return 0;
        }

        fuzz_encoder_controls(&ctx, Data, Size);

        aom_codec_destroy(&ctx);
    } else {
        // Initialize Decoder
        iface = aom_codec_av1_dx();
        if (!iface) return 0;

        memset(&dec_cfg, 0, sizeof(dec_cfg));
        dec_cfg.threads = 1;

        res = aom_codec_dec_init(&ctx, iface, &dec_cfg, 0);
        if (res != AOM_CODEC_OK) {
            aom_codec_destroy(&ctx);
            return 0;
        }

        fuzz_decoder_controls(&ctx, Data, Size);

        aom_codec_destroy(&ctx);
    }

    return 0;
}