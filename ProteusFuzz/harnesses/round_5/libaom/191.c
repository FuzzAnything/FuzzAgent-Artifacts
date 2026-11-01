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
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_image.h>
#include <aom/aom_integer.h>

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, int32_t value, int control_id) {
    if (!ctx) return 0;
    int ret = aom_codec_control(ctx, control_id, value);
    (void)ret;
    return 0;
}

int LLVMFuzzerTestOneInput_191(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use input data to configure encoder settings
    cfg.g_w = (Data[0] % 256) + 16;
    cfg.g_h = (Data[1] % 256) + 16;
    cfg.g_timebase.num = Data[2] + 1;
    cfg.g_timebase.den = Data[3] + 1;
    cfg.rc_target_bitrate = (Data[4] % 100) * 1000 + 1000;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Extract values from input to fuzz different control parameters
    int32_t val1 = (Data[4] % 2); // 0 or 1
    int32_t val2 = (Data[5] % 2); // 0 or 1
    int32_t val3 = (Data[6] % 2); // 0 or 1
    int32_t val4 = (Data[7] % 4) + 1; // 1 to 4 for partition size
    int32_t val5 = (Data[8] % 3); // 0 to 2 for tune content
    int32_t val6 = (Data[9] % 2); // 0 or 1

    // Fuzz AV1E_SET_CDF_UPDATE_MODE
    fuzz_encoder_control(&ctx, val1, AV1E_SET_CDF_UPDATE_MODE);

    // Fuzz AV1E_SET_ENABLE_TX_SIZE_SEARCH
    fuzz_encoder_control(&ctx, val2, AV1E_SET_ENABLE_TX_SIZE_SEARCH);

    // Fuzz AV1E_SET_INTRA_DEFAULT_TX_ONLY
    fuzz_encoder_control(&ctx, val3, AV1E_SET_INTRA_DEFAULT_TX_ONLY);

    // Fuzz AV1E_SET_MAX_PARTITION_SIZE
    fuzz_encoder_control(&ctx, val4, AV1E_SET_MAX_PARTITION_SIZE);

    // Fuzz AV1E_SET_TUNE_CONTENT
    fuzz_encoder_control(&ctx, val5, AV1E_SET_TUNE_CONTENT);

    // Fuzz AV1E_SET_ENABLE_SMOOTH_INTRA
    fuzz_encoder_control(&ctx, val6, AV1E_SET_ENABLE_SMOOTH_INTRA);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}