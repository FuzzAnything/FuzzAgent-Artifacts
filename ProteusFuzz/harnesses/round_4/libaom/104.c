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

static int fuzz_codec_control(aom_codec_ctx_t *ctx, int control_id, int value) {
    if (!ctx) return 0;
    aom_codec_err_t res = aom_codec_control(ctx, control_id, value);
    (void)res;
    return 1;
}

int LLVMFuzzerTestOneInput_104(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);

    // Use input data to configure width/height within safe bounds
    uint16_t width = (Data[0] % 512) + 16;
    uint16_t height = (Data[1] % 512) + 16;
    if (width % 2 != 0) width++;
    if (height % 2 != 0) height++;

    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.g_lag_in_frames = 0;
    cfg.rc_target_bitrate = 1000;

    aom_codec_err_t err = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    // Extract control values from input
    int val1 = (Size > 2) ? (Data[2] % 2) : 0;
    int val2 = (Size > 3) ? (Data[3] % 2) : 0;
    int val3 = (Size > 4) ? (Data[4] % 2) : 0;
    int val4 = (Size > 5) ? (Data[5] % 2) : 0;
    int val5 = (Size > 6) ? (Data[6] % 2) : 0;
    int val6 = (Size > 7) ? (Data[7] % 2) : 0;

    // Fuzz the target control functions using the correct macro
    fuzz_codec_control(&ctx, AV1E_SET_ENABLE_ONESIDED_COMP, val1);
    fuzz_codec_control(&ctx, AV1E_SET_ENABLE_INTERINTER_WEDGE, val2);
    fuzz_codec_control(&ctx, AV1E_SET_ENABLE_REF_FRAME_MVS, val3);
    fuzz_codec_control(&ctx, AV1E_SET_ALLOW_WARPED_MOTION, val4);
    fuzz_codec_control(&ctx, AV1E_SET_ENABLE_INTRA_EDGE_FILTER, val5);
    fuzz_codec_control(&ctx, AV1E_SET_ENABLE_FILTER_INTRA, val6);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}