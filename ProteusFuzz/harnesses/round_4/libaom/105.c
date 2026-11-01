// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
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

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (ctx == NULL || data == NULL || size == 0) {
        return 0;
    }

    int32_t val;
    size_t offset = 0;

    if (offset + sizeof(int32_t) <= size) {
        memcpy(&val, data + offset, sizeof(int32_t));
        offset += sizeof(int32_t);
        aom_codec_control(ctx, AV1E_SET_FRAME_PERIODIC_BOOST, val);
    }

    if (offset + sizeof(int32_t) <= size) {
        memcpy(&val, data + offset, sizeof(int32_t));
        offset += sizeof(int32_t);
        aom_codec_control(ctx, AV1E_SET_QM_MAX, val);
    }

    if (offset + sizeof(int32_t) <= size) {
        memcpy(&val, data + offset, sizeof(int32_t));
        offset += sizeof(int32_t);
        aom_codec_control(ctx, AV1E_SET_ENABLE_FILTER_INTRA, val);
    }

    if (offset + sizeof(int32_t) <= size) {
        memcpy(&val, data + offset, sizeof(int32_t));
        offset += sizeof(int32_t);
        aom_codec_control(ctx, AV1E_SET_SVC_FRAME_DROP_MODE, val);
    }

    if (offset + sizeof(int32_t) <= size) {
        memcpy(&val, data + offset, sizeof(int32_t));
        offset += sizeof(int32_t);
        aom_codec_control(ctx, AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE, val);
    }

    if (offset + sizeof(int32_t) <= size) {
        memcpy(&val, data + offset, sizeof(int32_t));
        offset += sizeof(int32_t);
        aom_codec_control(ctx, AV1E_SET_FP_MT_UNIT_TEST, val);
    }

    return 0;
}

int LLVMFuzzerTestOneInput_105(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (iface == NULL) {
        return 0;
    }

    aom_codec_enc_config_default(iface, &cfg, 0);
    
    cfg.g_w = 16;
    cfg.g_h = 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.g_error_resilient = 0;
    cfg.rc_target_bitrate = 1000;
    cfg.rc_min_quantizer = 0;
    cfg.rc_max_quantizer = 63;
    cfg.g_lag_in_frames = 0;

    aom_codec_err_t res = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        return 0;
    }

    fuzz_encoder_control(&ctx, Data, Size);

    aom_codec_destroy(&ctx);

    return 0;
}