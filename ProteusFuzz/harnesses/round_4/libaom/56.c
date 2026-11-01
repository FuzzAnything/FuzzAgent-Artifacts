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
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aomdx.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aom_integer.h>
#include <aom/aomcx.h>
#include <aom/aom_external_partition.h>

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, int control_id, int value) {
    aom_codec_err_t res;
    res = aom_codec_control(ctx, control_id, value);
    if (res != AOM_CODEC_OK) {
        return 0;
    }
    return 1;
}

int LLVMFuzzerTestOneInput_56(const uint8_t *Data, size_t Size) {
    if (Size < 10) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    // Use aom_codec_av1_cx() to get the AV1 encoder interface
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);

    // Use input data to configure width/height within reasonable bounds
    uint32_t width = Data[0] % 256 + 16;
    uint32_t height = Data[1] % 256 + 16;
    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = Data[2] % 60 + 1;
    cfg.g_threads = Data[3] % 8 + 1;
    cfg.rc_target_bitrate = 1000;
    cfg.rc_min_quantizer = 10;
    cfg.rc_max_quantizer = 63;

    aom_codec_err_t res = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) {
        return 0;
    }

    // Extract values from input for control parameters
    int val1 = Data[4] % 2;
    int val2 = Data[5] % 2;
    int val3 = (Data[6] % 16) + 1; // Min partition size
    int val4 = (Data[7] % 16) + 1; // Max partition size
    int val5 = Data[8] % 2;
    int val6 = Data[9] % 2;

    // Fuzz AV1E_SET_ENABLE_AB_PARTITIONS
    fuzz_encoder_control(&ctx, AV1E_SET_ENABLE_AB_PARTITIONS, val1);

    // Fuzz AV1E_SET_ENABLE_INTERINTER_WEDGE
    fuzz_encoder_control(&ctx, AV1E_SET_ENABLE_INTERINTER_WEDGE, val2);

    // Fuzz AV1E_SET_MIN_PARTITION_SIZE
    fuzz_encoder_control(&ctx, AV1E_SET_MIN_PARTITION_SIZE, val3);

    // Fuzz AV1E_SET_MAX_PARTITION_SIZE
    fuzz_encoder_control(&ctx, AV1E_SET_MAX_PARTITION_SIZE, val4);

    // Fuzz AV1E_SET_ENABLE_RECT_PARTITIONS
    fuzz_encoder_control(&ctx, AV1E_SET_ENABLE_RECT_PARTITIONS, val5);

    // Fuzz AV1E_SET_ENABLE_SMOOTH_INTERINTRA
    fuzz_encoder_control(&ctx, AV1E_SET_ENABLE_SMOOTH_INTERINTRA, val6);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}