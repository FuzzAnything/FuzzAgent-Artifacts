// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_encoder.h>
#include <aom/aom_integer.h>

static int fuzz_encoder_controls(aom_codec_ctx_t *codec, const uint8_t *data, size_t size) {
    int ret;
    int val;

    // AV1E_SET_INTRA_DCT_ONLY
    if (size >= 1) {
        val = data[0] & 0x1;
        ret = aom_codec_control(codec, AV1E_SET_INTRA_DCT_ONLY, val);
        (void)ret;
    }

    // AV1E_SET_ENABLE_TX_SIZE_SEARCH
    if (size >= 2) {
        val = data[1] & 0x1;
        ret = aom_codec_control(codec, AV1E_SET_ENABLE_TX_SIZE_SEARCH, val);
        (void)ret;
    }

    // AV1E_SET_DELTALF_MODE
    if (size >= 3) {
        val = data[2] & 0x7; // Assuming valid range 0-7 for delta LF mode
        ret = aom_codec_control(codec, AV1E_SET_DELTALF_MODE, val);
        (void)ret;
    }

    // AV1E_SET_ENABLE_INTERINTRA_COMP
    if (size >= 4) {
        val = data[3] & 0x1;
        ret = aom_codec_control(codec, AV1E_SET_ENABLE_INTERINTRA_COMP, val);
        (void)ret;
    }

    // AV1E_GET_BASELINE_GF_INTERVAL
    if (size >= 5) {
        int interval = 0;
        ret = aom_codec_control(codec, AV1E_GET_BASELINE_GF_INTERVAL, &interval);
        (void)interval;
        (void)ret;
    }

    // AV1E_SET_CDF_UPDATE_MODE
    if (size >= 6) {
        val = data[5] & 0x1;
        ret = aom_codec_control(codec, AV1E_SET_CDF_UPDATE_MODE, val);
        (void)ret;
    }

    return 0;
}

int LLVMFuzzerTestOneInput_270(const uint8_t *Data, size_t Size) {
    // Ensure we have enough data to safely access at least the first few bytes
    if (Size < 2) {
        return 0;
    }

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    aom_codec_ctx_t codec;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t err;

    err = aom_codec_enc_config_default(iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    // Use fuzz data to configure width/height safely
    // Ensure values are within a reasonable range and non-zero
    cfg.g_w = (Data[0] % 1024) + 16;
    cfg.g_h = (Data[1] % 1024) + 16;
    
    if (Size > 2) {
        cfg.g_timebase.num = Data[2] ? Data[2] : 1;
    } else {
        cfg.g_timebase.num = 1;
    }
    
    if (Size > 3) {
        cfg.g_timebase.den = Data[3] ? Data[3] : 1;
    } else {
        cfg.g_timebase.den = 1;
    }

    err = aom_codec_enc_init(&codec, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    // Invoke fuzzing logic for encoder controls
    fuzz_encoder_controls(&codec, Data, Size);

    // Cleanup
    aom_codec_destroy(&codec);

    return 0;
}