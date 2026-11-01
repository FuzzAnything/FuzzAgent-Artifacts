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
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>
#include <aom/aomcx.h>
#include <aom/aom_image.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom.h>

static int fuzz_encoder_controls(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx) return 0;

    int idx = 0;
    int ret = 0;

    // 1. AV1E_SET_RTC_EXTERNAL_RC
    if (idx + 4 <= size) {
        int val = *(int32_t *)(data + idx);
        idx += 4;
        ret = aom_codec_control(ctx, AV1E_SET_RTC_EXTERNAL_RC, val);
        (void)ret;
    }

    // 2. AV1E_SET_MIN_PARTITION_SIZE
    if (idx + 4 <= size) {
        int val = *(int32_t *)(data + idx);
        idx += 4;
        ret = aom_codec_control(ctx, AV1E_SET_MIN_PARTITION_SIZE, val);
        (void)ret;
    }

    // 3. AV1E_SET_ENABLE_INTRA_EDGE_FILTER
    if (idx + 4 <= size) {
        int val = *(int32_t *)(data + idx);
        idx += 4;
        ret = aom_codec_control(ctx, AV1E_SET_ENABLE_INTRA_EDGE_FILTER, val);
        (void)ret;
    }

    // 4. AV1E_SET_COEFF_COST_UPD_FREQ
    if (idx + 4 <= size) {
        int val = *(int32_t *)(data + idx);
        idx += 4;
        ret = aom_codec_control(ctx, AV1E_SET_COEFF_COST_UPD_FREQ, val);
        (void)ret;
    }

    // 5. AV1E_SET_ENABLE_SMOOTH_INTERINTRA
    if (idx + 4 <= size) {
        int val = *(int32_t *)(data + idx);
        idx += 4;
        ret = aom_codec_control(ctx, AV1E_SET_ENABLE_SMOOTH_INTERINTRA, val);
        (void)ret;
    }

    // 6. AV1E_SET_ENABLE_INTERINTER_WEDGE
    if (idx + 4 <= size) {
        int val = *(int32_t *)(data + idx);
        idx += 4;
        ret = aom_codec_control(ctx, AV1E_SET_ENABLE_INTERINTER_WEDGE, val);
        (void)ret;
    }

    return 0;
}

int LLVMFuzzerTestOneInput_242(const uint8_t *Data, size_t Size) {
    if (Size < 16) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);
    
    // Use input data to configure encoder parameters
    cfg.g_w = (Data[0] % 256) + 16;
    cfg.g_h = (Data[1] % 256) + 16;
    cfg.g_timebase.num = (Data[2] % 100) + 1;
    cfg.g_timebase.den = (Data[3] % 100) + 1;
    cfg.rc_target_bitrate = (Data[4] % 1000) + 100;
    cfg.g_threads = (Data[5] % 16) + 1;
    cfg.g_error_resilient = (Data[6] % 2);
    cfg.g_lag_in_frames = (Data[7] % 20);
    cfg.rc_end_usage = AOM_CBR;
    cfg.g_profile = Data[8] % 4;
    cfg.g_pass = AOM_RC_ONE_PASS;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz the control functions
    fuzz_encoder_controls(&ctx, Data, Size);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}