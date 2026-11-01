// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
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

static void cleanup_codec(aom_codec_ctx_t *codec) {
    if (codec) {
        aom_codec_destroy(codec);
    }
}

static int init_encoder(aom_codec_ctx_t *codec, const aom_codec_enc_cfg_t *cfg) {
    if (!codec || !cfg) return 0;
    
    const aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) return 0;
    
    if (aom_codec_enc_init(codec, iface, cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }
    return 1;
}

int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    aom_codec_ctx_t codec;
    memset(&codec, 0, sizeof(codec));

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(aom_codec_av1_dx(), &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use input data to diversify configuration
    if (Size > 0) {
        cfg.g_w = (Data[0] % 256) + 16; // Minimum 16
        cfg.g_h = (Size > 1) ? (Data[1] % 256) + 16 : 16;
        cfg.g_timebase.num = (Size > 2) ? (Data[2] % 256) + 1 : 1;
        cfg.g_timebase.den = (Size > 3) ? (Data[3] % 256) + 1 : 1;
        cfg.rc_target_bitrate = (Size > 4) ? (Data[4] * 1000) : 1000;
        cfg.g_threads = (Size > 5) ? (Data[5] % 16) + 1 : 1;
    }

    if (!init_encoder(&codec, &cfg)) {
        return 0;
    }

    // Fuzz various control functions based on input data
    int val;
    if (Size > 0) val = Data[0];
    else val = 0;

    // AV1E_SET_ENABLE_1TO4_PARTITIONS
    aom_codec_control(&codec, AV1E_SET_ENABLE_1TO4_PARTITIONS, val % 2);

    if (Size > 1) val = Data[1];
    // AV1E_SET_ENABLE_INTRABC
    aom_codec_control(&codec, AV1E_SET_ENABLE_INTRABC, val % 2);

    if (Size > 2) val = Data[2];
    // AV1E_SET_ENABLE_DIAGONAL_INTRA
    aom_codec_control(&codec, AV1E_SET_ENABLE_DIAGONAL_INTRA, val % 2);

    if (Size > 3) val = Data[3];
    // AV1E_SET_REDUCED_TX_TYPE_SET
    aom_codec_control(&codec, AV1E_SET_REDUCED_TX_TYPE_SET, val % 2);

    if (Size > 4) val = Data[4];
    // AV1E_SET_ENABLE_CFL_INTRA
    aom_codec_control(&codec, AV1E_SET_ENABLE_CFL_INTRA, val % 2);

    if (Size > 5) val = Data[5];
    // AV1E_SET_ENABLE_ANGLE_DELTA
    aom_codec_control(&codec, AV1E_SET_ENABLE_ANGLE_DELTA, val % 2);

    cleanup_codec(&codec);
    return 0;
}