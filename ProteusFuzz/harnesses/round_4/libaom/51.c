// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
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

static int fuzzer_init_encoder(aom_codec_ctx_t *codec, aom_codec_enc_cfg_t *cfg,
                               const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface;
    aom_codec_err_t ret;

    if (!data || size < 6) {
        return 0;
    }

    iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    ret = aom_codec_enc_config_default(iface, cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    // Use fuzz data to configure width/height and other parameters safely
    // Ensure we have enough data before accessing indices
    cfg->g_w = (data[0] % 255) + 16;
    cfg->g_h = (data[1] % 255) + 16;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = (data[2] % 255) + 1;
    cfg->g_threads = (data[3] % 4) + 1;
    cfg->g_lag_in_frames = (data[4] % 10) + 1;
    cfg->rc_target_bitrate = (data[5] % 100) + 100;

    ret = aom_codec_enc_init(codec, iface, cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    return 1;
}

int LLVMFuzzerTestOneInput_51(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t codec;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t ret;
    int initialized = 0;

    // Need at least 6 bytes for initialization
    if (Size < 6) {
        return 0;
    }

    memset(&codec, 0, sizeof(codec));
    memset(&cfg, 0, sizeof(cfg));

    initialized = fuzzer_init_encoder(&codec, &cfg, Data, Size);
    if (!initialized) {
        return 0;
    }

    // Extract control parameters from fuzz data
    uint8_t *ptr = (uint8_t *)Data;
    size_t idx = 6; // Start after the 6 bytes used for initialization

    // 1. AV1E_SET_FRAME_PERIODIC_BOOST
    if (idx + 1 <= Size) {
        int val = ptr[idx++];
        ret = aom_codec_control(&codec, AV1E_SET_FRAME_PERIODIC_BOOST, val);
        (void)ret;
    }

    // 2. AV1E_SET_TIMING_INFO_TYPE
    if (idx + 1 <= Size) {
        int val = ptr[idx++];
        ret = aom_codec_control(&codec, AV1E_SET_TIMING_INFO_TYPE, val);
        (void)ret;
    }

    // 3. AV1E_SET_TARGET_SEQ_LEVEL_IDX
    if (idx + 1 <= Size) {
        int val = ptr[idx++];
        ret = aom_codec_control(&codec, AV1E_SET_TARGET_SEQ_LEVEL_IDX, val);
        (void)ret;
    }

    // 4. AV1E_SET_ENABLE_REF_FRAME_MVS
    if (idx + 1 <= Size) {
        int val = ptr[idx++];
        ret = aom_codec_control(&codec, AV1E_SET_ENABLE_REF_FRAME_MVS, val);
        (void)ret;
    }

    // 5. AV1E_SET_ENABLE_FILTER_INTRA
    if (idx + 1 <= Size) {
        int val = ptr[idx++];
        ret = aom_codec_control(&codec, AV1E_SET_ENABLE_FILTER_INTRA, val);
        (void)ret;
    }

    // 6. AV1E_SET_ENABLE_SUPERRES
    if (idx + 1 <= Size) {
        int val = ptr[idx++];
        ret = aom_codec_control(&codec, AV1E_SET_ENABLE_SUPERRES, val);
        (void)ret;
    }

    // Cleanup
    aom_codec_destroy(&codec);

    return 0;
}