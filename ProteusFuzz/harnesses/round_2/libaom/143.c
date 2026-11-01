// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
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

static void cleanup_codec(aom_codec_ctx_t *codec) {
    if (codec) {
        aom_codec_destroy(codec);
    }
}

int LLVMFuzzerTestOneInput_143(const uint8_t *Data, size_t Size) {
    if (Size < 2) {
        return 0;
    }

    aom_codec_ctx_t codec;
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t ret;

    // Initialize encoder configuration
    ret = aom_codec_enc_config_default(NULL, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    // Set minimal configuration to allow initialization
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;

    // Get AV1 encoder interface
    iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    // Initialize the codec context
    ret = aom_codec_enc_init(&codec, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    // Extract control values from fuzz input
    int32_t val1 = Data[0];
    int32_t val2 = Data[1];
    int32_t val3 = (Size > 2) ? Data[2] : 0;
    int32_t val4 = (Size > 3) ? Data[3] : 0;
    int32_t val5 = (Size > 4) ? Data[4] : 0;
    int32_t val6 = (Size > 5) ? Data[5] : 0;

    // Fuzz AV1E_SET_MAX_PARTITION_SIZE
    ret = aom_codec_control(&codec, AV1E_SET_MAX_PARTITION_SIZE, val1);
    (void)ret;

    // Fuzz AV1E_SET_ENABLE_SMOOTH_INTRA
    ret = aom_codec_control(&codec, AV1E_SET_ENABLE_SMOOTH_INTRA, val2);
    (void)ret;

    // Fuzz AV1E_SET_ENABLE_RECT_PARTITIONS
    ret = aom_codec_control(&codec, AV1E_SET_ENABLE_RECT_PARTITIONS, val3);
    (void)ret;

    // Fuzz AV1E_SET_ENABLE_AB_PARTITIONS
    ret = aom_codec_control(&codec, AV1E_SET_ENABLE_AB_PARTITIONS, val4);
    (void)ret;

    // Fuzz AV1E_SET_MIN_PARTITION_SIZE
    ret = aom_codec_control(&codec, AV1E_SET_MIN_PARTITION_SIZE, val5);
    (void)ret;

    // Fuzz AV1E_SET_ENABLE_1TO4_PARTITIONS
    ret = aom_codec_control(&codec, AV1E_SET_ENABLE_1TO4_PARTITIONS, val6);
    (void)ret;

    // Cleanup
    cleanup_codec(&codec);

    return 0;
}