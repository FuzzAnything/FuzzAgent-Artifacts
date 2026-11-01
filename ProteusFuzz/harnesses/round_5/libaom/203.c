// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_integer.h>

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, int control_id, int value) {
    if (!ctx) return 0;
    aom_codec_err_t err = aom_codec_control(ctx, control_id, value);
    (void)err;
    return 0;
}

static int fuzz_encoder_control_bool(aom_codec_ctx_t *ctx, int control_id, int value) {
    if (!ctx) return 0;
    int val = value ? 1 : 0;
    aom_codec_err_t err = aom_codec_control(ctx, control_id, val);
    (void)err;
    return 0;
}

static aom_codec_ctx_t* create_encoder_context(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!ctx) return NULL;
    memset(ctx, 0, sizeof(aom_codec_ctx_t));

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        free(ctx);
        return NULL;
    }

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    if (size > 0) cfg.g_w = data[0] + 1;
    if (size > 1) cfg.g_h = data[1] + 1;
    if (size > 2) cfg.g_timebase.num = data[2] + 1;
    if (size > 3) cfg.g_timebase.den = data[3] + 1;
    if (size > 4) cfg.rc_target_bitrate = (data[4] * 1000) + 1000;
    if (size > 5) cfg.g_threads = (data[5] % 4) + 1;
    if (size > 6) cfg.g_profile = data[6] % 4;

    if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

int LLVMFuzzerTestOneInput_203(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    aom_codec_ctx_t *ctx = create_encoder_context(Data, Size);
    if (!ctx) return 0;

    int val1 = (Size > 0) ? Data[0] : 0;
    int val2 = (Size > 1) ? Data[1] : 0;
    int val3 = (Size > 2) ? Data[2] : 0;
    int val4 = (Size > 3) ? Data[3] : 0;
    int val5 = (Size > 4) ? Data[4] : 0;
    int val6 = (Size > 5) ? Data[5] : 0;

    fuzz_encoder_control_bool(ctx, AV1E_SET_REDUCED_REFERENCE_SET, val1);
    fuzz_encoder_control(ctx, AV1E_SET_COLOR_PRIMARIES, val2);
    fuzz_encoder_control(ctx, AV1E_SET_TRANSFER_CHARACTERISTICS, val3);
    fuzz_encoder_control_bool(ctx, AV1E_SET_ALLOW_WARPED_MOTION, val4);
    fuzz_encoder_control(ctx, AV1E_SET_COLOR_RANGE, val5);
    fuzz_encoder_control(ctx, AV1E_SET_MATRIX_COEFFICIENTS, val6);

    aom_codec_destroy(ctx);
    free(ctx);

    return 0;
}