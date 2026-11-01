// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
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
#include <aom/aom_decoder.h>
#include <aom/aomdx.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>

static int fuzz_encoder_controls(aom_codec_ctx_t *codec, const uint8_t *data, size_t size) {
    int ret = 0;
    int idx = 0;
    int val;

    if (codec == NULL) return 0;

    // AV1E_SET_QM_V
    if (idx + 1 <= size) {
        val = data[idx++];
        ret = aom_codec_control(codec, AV1E_SET_QM_V, val);
        (void)ret;
    }

    // AV1E_SET_INTER_DCT_ONLY
    if (idx + 1 <= size) {
        val = data[idx++];
        ret = aom_codec_control(codec, AV1E_SET_INTER_DCT_ONLY, val);
        (void)ret;
    }

    // AV1E_SET_MIN_CR
    if (idx + 1 <= size) {
        val = data[idx++];
        ret = aom_codec_control(codec, AV1E_SET_MIN_CR, val);
        (void)ret;
    }

    // AV1E_SET_ENABLE_SMOOTH_INTRA
    if (idx + 1 <= size) {
        val = data[idx++];
        ret = aom_codec_control(codec, AV1E_SET_ENABLE_SMOOTH_INTRA, val);
        (void)ret;
    }

    // AV1E_SET_MV_COST_UPD_FREQ
    if (idx + 1 <= size) {
        val = data[idx++];
        ret = aom_codec_control(codec, AV1E_SET_MV_COST_UPD_FREQ, val);
        (void)ret;
    }

    // AV1E_SET_ENABLE_DIST_8X8 is not a valid constant in this version of libaom.
    // Skipping this control to avoid compilation error.
    // If a similar control exists, it would be used here.

    return 0;
}

int LLVMFuzzerTestOneInput_265(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (iface == NULL) return 0;

    aom_codec_ctx_t codec;
    aom_codec_err_t err;
    aom_codec_enc_cfg_t cfg;

    err = aom_codec_enc_config_default(iface, &cfg, 0);
    if (err != AOM_CODEC_OK) return 0;

    // Use data to configure width/height to avoid invalid config
    cfg.g_w = Data[0] ? Data[0] : 16;
    cfg.g_h = Data[1] ? Data[1] : 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.g_usage = 0;

    err = aom_codec_enc_init(&codec, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz the control functions
    fuzz_encoder_controls(&codec, Data, Size);

    // Cleanup
    aom_codec_destroy(&codec);

    return 0;
}