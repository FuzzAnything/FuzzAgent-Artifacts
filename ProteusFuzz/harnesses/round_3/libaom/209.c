// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aom_image.h>
#include <aom/aom_integer.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>
#include <aom/aom.h>

static void cleanup_codec(aom_codec_ctx_t *codec) {
    if (codec) {
        aom_codec_destroy(codec);
    }
}

int LLVMFuzzerTestOneInput_209(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    aom_codec_ctx_t codec;
    memset(&codec, 0, sizeof(codec));

    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 500;

    aom_codec_err_t err = aom_codec_enc_init(&codec, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    // Extract fuzzing data to control parameters safely
    uint8_t val = Data[0] % 2; // 0 or 1 for boolean-like settings
    uint32_t partition_size = (Data[1] % 16) + 8; // Reasonable range for partition size

    // Test AV1E_SET_ENABLE_DNL_DENOISING
    err = aom_codec_control(&codec, AV1E_SET_ENABLE_DNL_DENOISING, val);
    // Ignore error, just exercising the path

    // Test AV1E_SET_ENABLE_SMOOTH_INTRA
    err = aom_codec_control(&codec, AV1E_SET_ENABLE_SMOOTH_INTRA, val);

    // Test AV1E_SET_ENABLE_INTRA_EDGE_FILTER
    err = aom_codec_control(&codec, AV1E_SET_ENABLE_INTRA_EDGE_FILTER, val);

    // Test AV1E_SET_REDUCED_TX_TYPE_SET
    // Assuming this takes an integer flag set, we use the first byte
    err = aom_codec_control(&codec, AV1E_SET_REDUCED_TX_TYPE_SET, Data[0]);

    // Test AV1E_SET_MIN_PARTITION_SIZE
    // Ensure value is within valid bounds if the API enforces them
    if (partition_size >= 8 && partition_size <= 128) {
        err = aom_codec_control(&codec, AV1E_SET_MIN_PARTITION_SIZE, partition_size);
    }

    // Test AV1E_SET_ENABLE_ORDER_HINT
    err = aom_codec_control(&codec, AV1E_SET_ENABLE_ORDER_HINT, val);

    // Cleanup
    cleanup_codec(&codec);

    return 0;
}