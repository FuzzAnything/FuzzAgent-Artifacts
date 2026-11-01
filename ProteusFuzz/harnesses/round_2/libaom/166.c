// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
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
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom_integer.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>

static int init_encoder(aom_codec_ctx_t *codec, const aom_codec_enc_cfg_t *cfg) {
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    if (aom_codec_enc_init(codec, iface, cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    return 1;
}

static int init_decoder(aom_codec_ctx_t *codec) {
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) return 0;

    aom_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    // Fixed: Changed allow_highbitdepth to allow_lowbitdepth as per error message
    cfg.allow_lowbitdepth = 1;

    if (aom_codec_dec_init(codec, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    return 1;
}

static void cleanup_encoder(aom_codec_ctx_t *codec) {
    if (codec && codec->iface) {
        aom_codec_destroy(codec);
    }
}

static void cleanup_decoder(aom_codec_ctx_t *codec) {
    if (codec && codec->iface) {
        aom_codec_destroy(codec);
    }
}

static int fuzz_encoder_controls(aom_codec_ctx_t *codec, const uint8_t *data, size_t size) {
    int ret = 1;
    int val;

    // AV1E_SET_ENABLE_INTERINTRA_WEDGE
    if (size >= sizeof(int)) {
        memcpy(&val, data, sizeof(int));
        if (aom_codec_control(codec, AV1E_SET_ENABLE_INTERINTRA_WEDGE, val) != AOM_CODEC_OK) {
            // Error is expected sometimes, not a failure of the fuzzer
        }
    }

    // AV1E_SET_ENABLE_OBMC
    if (size >= sizeof(int)) {
        memcpy(&val, data + sizeof(int), sizeof(int));
        if (aom_codec_control(codec, AV1E_SET_ENABLE_OBMC, val) != AOM_CODEC_OK) {
            // Ignore error
        }
    }

    // AV1E_SET_ALLOW_WARPED_MOTION
    if (size >= sizeof(int)) {
        memcpy(&val, data + 2 * sizeof(int), sizeof(int));
        if (aom_codec_control(codec, AV1E_SET_ALLOW_WARPED_MOTION, val) != AOM_CODEC_OK) {
            // Ignore error
        }
    }

    // AOME_SET_SHARPNESS
    if (size >= sizeof(int)) {
        memcpy(&val, data + 3 * sizeof(int), sizeof(int));
        if (aom_codec_control(codec, AOME_SET_SHARPNESS, val) != AOM_CODEC_OK) {
            // Ignore error
        }
    }

    // AV1E_SET_ROW_MT
    if (size >= sizeof(int)) {
        memcpy(&val, data + 4 * sizeof(int), sizeof(int));
        if (aom_codec_control(codec, AV1E_SET_ROW_MT, val) != AOM_CODEC_OK) {
            // Ignore error
        }
    }

    return ret;
}

static int fuzz_decoder_controls(aom_codec_ctx_t *codec, const uint8_t *data, size_t size) {
    int val;

    // AV1D_SET_ROW_MT
    if (size >= sizeof(int)) {
        memcpy(&val, data, sizeof(int));
        if (aom_codec_control(codec, AV1D_SET_ROW_MT, val) != AOM_CODEC_OK) {
            // Ignore error
        }
    }

    return 1;
}

int LLVMFuzzerTestOneInput_166(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Determine mode: 0 for encoder, 1 for decoder, or random based on data
    int mode = Data[0] % 2;

    if (mode == 0) {
        // Encoder path
        aom_codec_ctx_t codec;
        memset(&codec, 0, sizeof(codec));

        aom_codec_enc_cfg_t cfg;
        if (aom_codec_enc_config_default(aom_codec_av1_cx(), &cfg, 0) != AOM_CODEC_OK) {
            return 0;
        }

        // Use input data to tweak config slightly
        if (Size > 1) cfg.g_w = 16 + (Data[1] % 256);
        if (Size > 2) cfg.g_h = 16 + (Data[2] % 256);
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 30;
        cfg.rc_target_bitrate = 1000;

        if (!init_encoder(&codec, &cfg)) {
            return 0;
        }

        fuzz_encoder_controls(&codec, Data + 1, Size > 1 ? Size - 1 : 0);

        cleanup_encoder(&codec);
    } else {
        // Decoder path
        aom_codec_ctx_t codec;
        memset(&codec, 0, sizeof(codec));

        if (!init_decoder(&codec)) {
            return 0;
        }

        fuzz_decoder_controls(&codec, Data + 1, Size > 1 ? Size - 1 : 0);

        cleanup_decoder(&codec);
    }

    return 0;
}