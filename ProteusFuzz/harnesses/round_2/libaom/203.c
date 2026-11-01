// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aom_decoder.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>
#include <aom/aomcx.h>
#include <aom/aom_image.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom.h>

int LLVMFuzzerTestOneInput_203(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    aom_codec_ctx_t codec_ctx;
    memset(&codec_ctx, 0, sizeof(codec_ctx));

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.rc_target_bitrate = 100;
    cfg.g_profile = 0;

    aom_codec_err_t res = aom_codec_enc_init(&codec_ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        return 0;
    }

    uint8_t val = Data[0];

    // Fuzz AV1E_SET_ENABLE_CDEF
    aom_codec_control(&codec_ctx, AV1E_SET_ENABLE_CDEF, val);

    // Fuzz AV1E_SET_QM_U
    aom_codec_control(&codec_ctx, AV1E_SET_QM_U, val);

    // Fuzz AOME_SET_TUNING
    aom_codec_control(&codec_ctx, AOME_SET_TUNING, val);

    // Fuzz AV1E_SET_ENABLE_KEYFRAME_FILTERING
    aom_codec_control(&codec_ctx, AV1E_SET_ENABLE_KEYFRAME_FILTERING, val);

    // Fuzz AV1E_SET_SKIP_POSTPROC_FILTERING
    aom_codec_control(&codec_ctx, AV1E_SET_SKIP_POSTPROC_FILTERING, val);

    // Fuzz AV1E_SET_FORCE_VIDEO_MODE
    aom_codec_control(&codec_ctx, AV1E_SET_FORCE_VIDEO_MODE, val);

    aom_codec_destroy(&codec_ctx);

    return 0;
}