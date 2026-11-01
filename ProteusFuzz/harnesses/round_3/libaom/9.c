// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>

static void cleanup_codec(aom_codec_ctx_t *ctx, aom_codec_iface_t *iface) {
    if (ctx && iface) {
        aom_codec_destroy(ctx);
    }
}

static int fuzz_encoder_controls(const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t err;

    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz configuration parameters based on input data
    if (size > 0) cfg.g_w = (data[0] % 256) + 16;
    if (size > 1) cfg.g_h = (data[1] % 256) + 16;
    if (size > 2) cfg.g_timebase.num = (data[2] % 256) + 1;
    if (size > 3) cfg.g_timebase.den = (data[3] % 256) + 1;
    if (size > 4) cfg.rc_target_bitrate = (data[4] % 1000) + 1;
    if (size > 5) cfg.g_threads = (data[5] % 16) + 1;
    if (size > 6) cfg.g_error_resilient = (data[6] % 2);

    err = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz AV1E_SET_REDUCED_REFERENCE_SET
    if (size > 7) {
        int val = data[7];
        aom_codec_control(&ctx, AV1E_SET_REDUCED_REFERENCE_SET, val);
    }

    // Fuzz AV1E_SET_COEFF_COST_UPD_FREQ
    if (size > 8) {
        int val = data[8];
        aom_codec_control(&ctx, AV1E_SET_COEFF_COST_UPD_FREQ, val);
    }

    // Fuzz AV1E_SET_ENABLE_INTRA_EDGE_FILTER
    if (size > 9) {
        int val = data[9];
        aom_codec_control(&ctx, AV1E_SET_ENABLE_INTRA_EDGE_FILTER, val);
    }

    // Fuzz AV1E_SET_ENABLE_ORDER_HINT
    if (size > 10) {
        int val = data[10];
        aom_codec_control(&ctx, AV1E_SET_ENABLE_ORDER_HINT, val);
    }

    cleanup_codec(&ctx, iface);
    return 1;
}

static int fuzz_decoder_controls(const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_dec_cfg_t cfg;
    aom_codec_err_t err;

    memset(&cfg, 0, sizeof(cfg));
    if (size > 0) cfg.threads = (data[0] % 16) + 1;
    if (size > 1) cfg.allow_lowbitdepth = (data[1] % 2);

    err = aom_codec_dec_init(&ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz AOMD_GET_ORDER_HINT
    int order_hint;
    err = aom_codec_control(&ctx, AOMD_GET_ORDER_HINT, &order_hint);
    (void)err; // Ignore errors for fuzzing robustness

    cleanup_codec(&ctx, iface);
    return 1;
}

int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Determine mode: 0 for encoder, 1 for decoder
    int mode = Data[0] % 2;

    if (mode == 0) {
        return fuzz_encoder_controls(Data, Size);
    } else {
        return fuzz_decoder_controls(Data, Size);
    }
}