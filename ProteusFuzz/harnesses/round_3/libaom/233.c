// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aom_codec.h"
#include "aom/aom_integer.h"
#include "aom/aom_image.h"
#include "aom/aomdx.h"
#include "aom/aomcx.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom.h"

static int init_encoder(aom_codec_ctx_t *codec, aom_codec_enc_cfg_t *cfg,
                        const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface;
    aom_codec_err_t err;

    iface = aom_codec_av1_cx();
    if (iface == NULL) {
        return 0;
    }

    if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    if (size >= 2) {
        cfg->g_w = 16 + (data[0] % 384);
        cfg->g_h = 16 + (data[1] % 384);
    }
    if (size >= 3) {
        cfg->g_timebase.num = 1;
        cfg->g_timebase.den = 1 + (data[2] % 1000);
    }
    cfg->rc_target_bitrate = 1000 + (size % 10000);

    err = aom_codec_enc_init(codec, iface, cfg, 0);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    return 1;
}

int LLVMFuzzerTestOneInput_233(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t codec;
    aom_codec_enc_cfg_t cfg;
    int initialized = 0;
    int val1 = 0, val2 = 0, val3 = 0, val4 = 0, val5 = 0, val6 = 0;

    if (Size == 0) return 0;

    memset(&codec, 0, sizeof(codec));
    memset(&cfg, 0, sizeof(cfg));

    if (init_encoder(&codec, &cfg, Data, Size)) {
        initialized = 1;
    } else {
        return 0;
    }

    if (Size >= 1) val1 = Data[0];
    if (Size >= 2) val2 = Data[1];
    if (Size >= 3) val3 = Data[2];
    if (Size >= 4) val4 = Data[3];
    if (Size >= 5) val5 = Data[4];
    if (Size >= 6) val6 = Data[5];

    // Use the enum values directly as the control ID, as AOM_CODEC_CTRL_ID is a macro
    // that might not be available or defined in the specific include path context.
    // The aom_codec_control function expects the control ID as the second argument.
    
    aom_codec_control(&codec, AV1E_SET_ENABLE_RECT_TX, val1);
    aom_codec_control(&codec, AV1E_SET_ENABLE_CFL_INTRA, val2);
    aom_codec_control(&codec, AOME_SET_TUNING, val3);
    aom_codec_control(&codec, AV1E_SET_MAX_CONSEC_FRAME_DROP_MS_CBR, val4);
    aom_codec_control(&codec, AV1E_SET_ENABLE_INTRABC, val5);
    aom_codec_control(&codec, AV1E_SET_TUNE_CONTENT, val6);

    aom_codec_destroy(&codec);

    return 0;
}