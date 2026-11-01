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
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aom_image.h>
#include <aom/aom_integer.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>

static int fuzz_encoder_controls(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    int ret = AOM_CODEC_OK;
    int val;
    size_t offset = 0;

    // Helper to safely extract int from fuzz data
    #define SAFE_GET_INT(_out) \
        do { \
            if (size >= offset + sizeof(int)) { \
                memcpy(&val, data + offset, sizeof(int)); \
                offset += sizeof(int); \
            } else { \
                val = 0; \
            } \
        } while(0)

    // 1. aom_codec_control_typechecked_AV1E_SET_COEFF_COST_UPD_FREQ
    SAFE_GET_INT(val);
    ret = aom_codec_control(ctx, AV1E_SET_COEFF_COST_UPD_FREQ, val);
    (void)ret;

    // 2. aom_codec_control_typechecked_AOME_SET_ENABLEAUTOBWDREF
    SAFE_GET_INT(val);
    ret = aom_codec_control(ctx, AOME_SET_ENABLEAUTOBWDREF, val);
    (void)ret;

    // 3. aom_codec_control_typechecked_AV1E_SET_DELTAQ_MODE
    SAFE_GET_INT(val);
    ret = aom_codec_control(ctx, AV1E_SET_DELTAQ_MODE, val);
    (void)ret;

    // 4. aom_codec_control_typechecked_AOME_SET_CQ_LEVEL
    SAFE_GET_INT(val);
    ret = aom_codec_control(ctx, AOME_SET_CQ_LEVEL, val);
    (void)ret;

    // 5. aom_codec_control_typechecked_AOME_SET_ENABLEAUTOALTREF
    SAFE_GET_INT(val);
    ret = aom_codec_control(ctx, AOME_SET_ENABLEAUTOALTREF, val);
    (void)ret;

    // 6. aom_codec_control_typechecked_AV1E_SET_ENABLE_TPL_MODEL
    SAFE_GET_INT(val);
    ret = aom_codec_control(ctx, AV1E_SET_ENABLE_TPL_MODEL, val);
    (void)ret;

    return 0;
}

int LLVMFuzzerTestOneInput_161(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Set minimal valid config
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.g_error_resilient = 0;
    cfg.rc_target_bitrate = 1000;
    cfg.rc_min_quantizer = 0;
    cfg.rc_max_quantizer = 63;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Invoke the fuzzing logic for encoder controls
    fuzz_encoder_controls(&ctx, Data, Size);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}