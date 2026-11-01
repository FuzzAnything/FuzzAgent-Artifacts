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
    aom_codec_err_t res = aom_codec_control(ctx, control_id, value);
    (void)res;
    return 0;
}

int LLVMFuzzerTestOneInput_154(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    int width = Data[0] % 128 + 16;
    int height = Data[1] % 128 + 16;
    int bit_depth = (Data[2] % 2) ? 10 : 8;
    int profile = Data[3] % 4;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;

    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_bit_depth = bit_depth;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.g_profile = profile;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    uint32_t val = 0;
    if (Size > 4) {
        val = *(uint32_t *)(Data + 4);
    }

    int control_id = -1;
    int value = 0;

    switch (Data[0] % 6) {
        case 0:
            control_id = AV1E_SET_INTRA_DCT_ONLY;
            value = val & 0x1;
            break;
        case 1:
            control_id = AV1E_SET_FRAME_PERIODIC_BOOST;
            value = val & 0xFFFF;
            break;
        case 2:
            // AV1E_SET_ENABLE_DIST_8X8 is not a valid constant in current libaom, skipping or mapping to a valid one
            // Using AV1E_SET_ENABLE_RECT_TX as a placeholder for testing control mechanism if needed, 
            // but strictly following the prompt's intent to test the specific API if it existed.
            // Since it doesn't exist, we skip this case to avoid compilation error, or map to a valid one.
            // Mapping to a valid control to maintain fuzzing coverage.
            control_id = AV1E_SET_ENABLE_RECT_TX; 
            value = val & 0x1;
            break;
        case 3:
            control_id = AV1E_SET_COLOR_PRIMARIES;
            value = val & 0xFF;
            break;
        case 4:
            control_id = AV1E_SET_ENABLE_REF_FRAME_MVS;
            value = val & 0x1;
            break;
        case 5:
            control_id = AV1E_SET_ENABLE_TPL_MODEL;
            value = val & 0x1;
            break;
    }

    if (control_id != -1) {
        fuzz_encoder_control(&ctx, control_id, value);
    }

    aom_codec_destroy(&ctx);

    return 0;
}