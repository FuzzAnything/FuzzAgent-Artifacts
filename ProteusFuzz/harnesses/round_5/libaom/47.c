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

#include "aom/aomdx.h"
#include "aom/aomcx.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_decoder.h"
#include "aom/aom_image.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_encoder.h"
#include "aom/aom_integer.h"

static void cleanup_codec(aom_codec_ctx_t *codec) {
    if (codec) {
        aom_codec_destroy(codec);
    }
}

int LLVMFuzzerTestOneInput_47(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
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
    cfg.g_w = 32;
    cfg.g_h = 32;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;

    aom_codec_err_t ret = aom_codec_enc_init(&codec, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        cleanup_codec(&codec);
        return 0;
    }

    // Extract fuzzing data to control parameters
    int val = 0;
    if (Size > 0) {
        val = Data[0];
    }
    int val2 = 0;
    if (Size > 1) {
        val2 = Data[1];
    }

    // Fuzz aom_codec_control_typechecked_AV1E_SET_QM_U
    // Assuming it takes an int value for quantization matrix
    ret = aom_codec_control(&codec, AV1E_SET_QM_U, val);

    // Fuzz aom_codec_control_typechecked_AV1E_SET_ENABLE_INTRABC
    // Assuming it takes an int (0 or 1)
    ret = aom_codec_control(&codec, AV1E_SET_ENABLE_INTRABC, val & 1);

    // Fuzz aom_codec_control_typechecked_AV1E_SET_ENABLE_PALETTE
    // Assuming it takes an int (0 or 1)
    ret = aom_codec_control(&codec, AV1E_SET_ENABLE_PALETTE, val2 & 1);

    // Fuzz aom_codec_control_typechecked_AV1E_SET_MAX_CONSEC_FRAME_DROP_MS_CBR
    // Assuming it takes an int (milliseconds)
    ret = aom_codec_control(&codec, AV1E_SET_MAX_CONSEC_FRAME_DROP_MS_CBR, val);

    // Fuzz aom_codec_control_typechecked_AOME_SET_TUNING
    // Assuming it takes an int (tuning mode)
    ret = aom_codec_control(&codec, AOME_SET_TUNING, val % 5);

    // Fuzz aom_codec_control_typechecked_AV1E_SET_ENABLE_CHROMA_DELTAQ
    // Assuming it takes an int (0 or 1)
    ret = aom_codec_control(&codec, AV1E_SET_ENABLE_CHROMA_DELTAQ, (val + val2) & 1);

    cleanup_codec(&codec);
    return 0;
}