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
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aom_integer.h>
#include <aom/aom_codec.h>
#include <aom/aom_external_partition.h>

static int fuzz_codec_control(aom_codec_ctx_t *ctx, int control, int value) {
    if (!ctx) return 0;
    int ret = aom_codec_control(ctx, control, value);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }
    return 1;
}

int LLVMFuzzerTestOneInput_79(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);
    cfg.g_w = 128;
    cfg.g_h = 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    int value = (Data[0] % 2) ? 1 : 0;
    int control_idx = Data[1] % 6;

    switch (control_idx) {
        case 0:
            fuzz_codec_control(&ctx, AV1E_SET_ENABLE_PAETH_INTRA, value);
            break;
        case 1:
            fuzz_codec_control(&ctx, AV1E_SET_ENABLE_INTERINTRA_COMP, value);
            break;
        case 2:
            fuzz_codec_control(&ctx, AV1E_SET_ENABLE_DIAGONAL_INTRA, value);
            break;
        case 3:
            fuzz_codec_control(&ctx, AV1E_SET_ENABLE_INTERINTRA_WEDGE, value);
            break;
        case 4:
            fuzz_codec_control(&ctx, AV1E_SET_TUNE_CONTENT, value);
            break;
        case 5:
            fuzz_codec_control(&ctx, AV1E_SET_ENABLE_SMOOTH_INTERINTRA, value);
            break;
    }

    aom_codec_destroy(&ctx);
    return 0;
}