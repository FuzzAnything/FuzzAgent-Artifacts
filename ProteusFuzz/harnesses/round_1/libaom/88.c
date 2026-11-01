// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
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

static int fuzz_encoder_controls(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (ctx == NULL || data == NULL) return 0;

    int val;
    int ret;

    // AV1E_SET_ENABLE_QM
    if (size >= 1) {
        val = data[0];
        ret = aom_codec_control(ctx, AV1E_SET_ENABLE_QM, val);
        (void)ret; // Suppress unused variable warning if needed, but check for errors in real scenarios
    }

    // AV1E_SET_DISABLE_TRELLIS_QUANT
    if (size >= 2) {
        val = data[1];
        ret = aom_codec_control(ctx, AV1E_SET_DISABLE_TRELLIS_QUANT, val);
        (void)ret;
    }

    // AV1E_SET_ENABLE_TPL_MODEL
    if (size >= 3) {
        val = data[2];
        ret = aom_codec_control(ctx, AV1E_SET_ENABLE_TPL_MODEL, val);
        (void)ret;
    }

    // AV1E_SET_DELTAQ_MODE
    if (size >= 4) {
        val = data[3];
        ret = aom_codec_control(ctx, AV1E_SET_DELTAQ_MODE, val);
        (void)ret;
    }

    // AV1E_SET_ENABLE_REF_FRAME_MVS
    if (size >= 5) {
        val = data[4];
        ret = aom_codec_control(ctx, AV1E_SET_ENABLE_REF_FRAME_MVS, val);
        (void)ret;
    }

    // AV1E_SET_FRAME_PERIODIC_BOOST
    if (size >= 6) {
        val = data[5];
        ret = aom_codec_control(ctx, AV1E_SET_FRAME_PERIODIC_BOOST, val);
        (void)ret;
    }

    return 0;
}

int LLVMFuzzerTestOneInput_88(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (iface == NULL) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;

    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Set minimal configuration
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.rc_target_bitrate = 1000;
    cfg.g_profile = 0;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz the control functions
    fuzz_encoder_controls(&ctx, Data, Size);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}