// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
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

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, int control_id, int32_t value) {
    if (!ctx) return 0;
    aom_codec_err_t res = aom_codec_control(ctx, control_id, value);
    if (res != AOM_CODEC_OK) {
        return 0;
    }
    return 1;
}

int LLVMFuzzerTestOneInput_114(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) return 0;

    cfg.g_w = (Data[0] % 1024) + 16;
    cfg.g_h = (Data[1] % 1024) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = (Data[0] % 1000) + 1;
    cfg.g_threads = 1;
    cfg.rc_target_bitrate = 1000;
    cfg.g_profile = 0;

    aom_codec_err_t init_res = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (init_res != AOM_CODEC_OK) {
        return 0;
    }

    int32_t qm_min = (int32_t)(Data[0] % 256);
    int32_t qm_max = (int32_t)(Data[1] % 256);
    int32_t mtu = (int32_t)(Size % 65536);
    int32_t enable_overlay = (int32_t)(Data[0] % 2);
    int32_t enable_mv_unit_test = (int32_t)(Data[1] % 2);
    int32_t enable_sb_multipass_unit_test = (int32_t)(Data[0] % 2);

    fuzz_encoder_control(&ctx, AV1E_SET_QM_MIN, qm_min);
    fuzz_encoder_control(&ctx, AV1E_SET_QM_MAX, qm_max);
    fuzz_encoder_control(&ctx, AV1E_SET_MTU, mtu);
    fuzz_encoder_control(&ctx, AV1E_SET_ENABLE_OVERLAY, enable_overlay);
    fuzz_encoder_control(&ctx, AV1E_ENABLE_MOTION_VECTOR_UNIT_TEST, enable_mv_unit_test);
    fuzz_encoder_control(&ctx, AV1E_ENABLE_SB_MULTIPASS_UNIT_TEST, enable_sb_multipass_unit_test);

    aom_codec_destroy(&ctx);

    return 0;
}