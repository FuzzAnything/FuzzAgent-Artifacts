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
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aom_decoder.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>
#include <aom/aomcx.h>
#include <aom/aom_image.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom.h>

static int fuzz_codec_control(aom_codec_ctx_t *ctx, int control_id, int value) {
    if (!ctx) return 0;
    return aom_codec_control(ctx, control_id, value);
}

int LLVMFuzzerTestOneInput_91(const uint8_t *Data, size_t Size) {
    if (Size < 8) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) return 0;

    // Use fuzzer data to set some config values with bounds checking
    cfg.g_w = (Data[0] % 1024) + 16;
    cfg.g_h = (Data[1] % 1024) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = (Data[2] % 10000) + 100;

    aom_codec_err_t err = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    // Extract values from input to control parameters with bounds checking
    int val1 = (Data[3] % 2);
    int val2 = (Data[4] % 2);
    int val3 = (Data[5] % 2);
    int val4 = (Data[6] % 2);
    int val5 = (Data[7] % 2);
    int val6 = (Size > 8) ? (Data[8] % 2) : 0;

    // Fuzz the control functions
    fuzz_codec_control(&ctx, AV1E_SET_ENABLE_DIAGONAL_INTRA, val1);
    fuzz_codec_control(&ctx, AV1E_SET_ENABLE_INTERINTRA_COMP, val2);
    fuzz_codec_control(&ctx, AV1E_SET_ENABLE_RECT_TX, val3);
    fuzz_codec_control(&ctx, AV1E_SET_ENABLE_INTERINTRA_WEDGE, val4);
    fuzz_codec_control(&ctx, AV1E_SET_ENABLE_RECT_PARTITIONS, val5);
    fuzz_codec_control(&ctx, AV1E_SET_ENABLE_MASKED_COMP, val6);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}