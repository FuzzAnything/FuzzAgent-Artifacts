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

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, int control_id, int value) {
    if (ctx == NULL) return 0;
    return aom_codec_control(ctx, control_id, value);
}

int LLVMFuzzerTestOneInput_82(const uint8_t *Data, size_t Size) {
    if (Size < 8) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (iface == NULL) return 0;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use fuzzing data to set basic config parameters safely
    cfg.g_w = (Data[0] % 1024) + 16;
    cfg.g_h = (Data[1] % 1024) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = (Data[2] % 60) + 1;
    cfg.g_threads = 1;
    cfg.g_lag_in_frames = 0;
    cfg.rc_target_bitrate = 1000;
    cfg.rc_min_quantizer = 0;
    cfg.rc_max_quantizer = 63;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Extract control ID and value from input data safely
    int control_id = 0;
    int value = 0;

    if (Size >= 8) {
        control_id = *(int32_t *)(Data + 0);
        value = *(int32_t *)(Data + 4);
    }

    // Map control_id to specific AV1 encoder controls
    int mapped_control = 0;
    switch (control_id % 6) {
        case 0: mapped_control = AV1E_SET_ENABLE_ONESIDED_COMP; break;
        case 1: mapped_control = AV1E_SET_ENABLE_1TO4_PARTITIONS; break;
        case 2: mapped_control = AV1E_SET_ENABLE_DNL_DENOISING; break;
        case 3: mapped_control = AV1E_SET_ENABLE_DUAL_FILTER; break;
        case 4: mapped_control = AV1E_SET_ENABLE_CFL_INTRA; break;
        case 5: mapped_control = AV1E_SET_ENABLE_ANGLE_DELTA; break;
        default: mapped_control = AV1E_SET_ENABLE_ONESIDED_COMP; break;
    }

    // Normalize value to 0 or 1 for boolean controls
    int normalized_value = (value != 0) ? 1 : 0;

    // Invoke the control function
    fuzz_encoder_control(&ctx, mapped_control, normalized_value);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}