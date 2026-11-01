// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_get_caps at aom_codec.c:84:18 in aom_codec.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
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

static int get_int(const uint8_t **d, size_t *s) {
    if (*s < sizeof(int)) return 0;
    int val;
    memcpy(&val, *d, sizeof(int));
    *d += sizeof(int);
    *s -= sizeof(int);
    return val;
}

static int fuzz_encoder_controls(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx) return 0;

    const uint8_t *ptr = data;
    size_t remaining = size;

    // 1. aom_codec_control_typechecked_AV1E_SET_SVC_REF_FRAME_COMP_PRED
    if (remaining >= sizeof(aom_svc_ref_frame_comp_pred_t)) {
        aom_svc_ref_frame_comp_pred_t comp_pred;
        memcpy(&comp_pred, ptr, sizeof(comp_pred));
        ptr += sizeof(comp_pred);
        remaining -= sizeof(comp_pred);
        aom_codec_control(ctx, AV1E_SET_SVC_REF_FRAME_COMP_PRED, &comp_pred);
    }

    // 2. aom_codec_control_typechecked_AV1E_SET_FRAME_PERIODIC_BOOST
    if (remaining >= sizeof(int)) {
        int boost = get_int(&ptr, &remaining);
        aom_codec_control(ctx, AV1E_SET_FRAME_PERIODIC_BOOST, &boost);
    }

    // 3. aom_codec_control_typechecked_AV1E_SET_ENABLE_REF_FRAME_MVS
    if (remaining >= sizeof(int)) {
        int mvs = get_int(&ptr, &remaining);
        aom_codec_control(ctx, AV1E_SET_ENABLE_REF_FRAME_MVS, &mvs);
    }

    // 4. aom_codec_control_typechecked_AV1E_SET_SVC_FRAME_DROP_MODE
    if (remaining >= sizeof(int)) {
        int drop_mode = get_int(&ptr, &remaining);
        aom_codec_control(ctx, AV1E_SET_SVC_FRAME_DROP_MODE, &drop_mode);
    }

    // 5. aom_codec_control_typechecked_AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE
    if (remaining >= sizeof(int)) {
        int low_complexity = get_int(&ptr, &remaining);
        aom_codec_control(ctx, AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE, &low_complexity);
    }

    // 6. aom_codec_control_typechecked_AV1E_SET_FP_MT_UNIT_TEST
    if (remaining >= sizeof(int)) {
        int fp_mt = get_int(&ptr, &remaining);
        aom_codec_control(ctx, AV1E_SET_FP_MT_UNIT_TEST, &fp_mt);
    }

    return 0;
}

int LLVMFuzzerTestOneInput_137(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    // Initialize encoder configuration
    aom_codec_enc_cfg_t cfg;
    
    // Get the interface first to ensure it exists and is an encoder
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    // Check if the interface supports encoding
    if (!(aom_codec_get_caps(iface) & AOM_CODEC_CAP_ENCODER)) {
        return 0;
    }

    // Now it is safe to call config_default
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Set some basic parameters to ensure valid initialization
    cfg.g_w = 16;
    cfg.g_h = 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;

    // Initialize the codec
    aom_codec_err_t res = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz the control functions
    fuzz_encoder_controls(&ctx, Data, Size);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}