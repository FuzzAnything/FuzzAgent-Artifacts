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
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>
#include <aom/aomcx.h>
#include <aom/aom_image.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom.h>

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, int control_id, int value) {
    if (!ctx) return 0;
    aom_codec_err_t res = aom_codec_control(ctx, control_id, value);
    (void)res;
    return 0;
}

int LLVMFuzzerTestOneInput_147(const uint8_t *Data, size_t Size) {
    if (Size < 11) return 0;

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use input data to vary configuration
    cfg.g_w = (Data[0] % 512) + 16;
    cfg.g_h = (Data[1] % 512) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = (Data[2] % 60) + 1;
    cfg.rc_target_bitrate = (Data[3] % 10000) + 1000;
    cfg.g_threads = (Data[4] % 4) + 1;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Extract values from input to drive control parameters
    int val1 = Data[5] % 256;
    int val2 = Data[6] % 256;
    int val3 = Data[7] % 256;
    int val4 = Data[8] % 256;
    int val5 = Data[9] % 256;
    int val6 = Data[10] % 256;

    // AV1E_SET_MAX_PARTITION_SIZE
    fuzz_encoder_control(&ctx, AV1E_SET_MAX_PARTITION_SIZE, val1);

    // AV1E_SET_RTC_EXTERNAL_RC
    fuzz_encoder_control(&ctx, AV1E_SET_RTC_EXTERNAL_RC, val2);

    // AV1E_SET_TIER_MASK
    fuzz_encoder_control(&ctx, AV1E_SET_TIER_MASK, val3);

    // AV1E_SET_SUPERBLOCK_SIZE
    fuzz_encoder_control(&ctx, AV1E_SET_SUPERBLOCK_SIZE, val4);

    // AV1E_SET_MTU
    fuzz_encoder_control(&ctx, AV1E_SET_MTU, val5);

    // AV1E_SET_FORCE_VIDEO_MODE
    fuzz_encoder_control(&ctx, AV1E_SET_FORCE_VIDEO_MODE, val6);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}