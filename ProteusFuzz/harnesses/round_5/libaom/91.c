// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
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

static int init_encoder(aom_codec_ctx_t *codec, aom_codec_enc_cfg_t *cfg) {
    const aom_codec_iface_t *iface;
    aom_codec_err_t res;

    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    res = aom_codec_enc_config_default(iface, cfg, 0);
    if (res != AOM_CODEC_OK) return 0;

    res = aom_codec_enc_init(codec, iface, cfg, 0);
    if (res != AOM_CODEC_OK) return 0;

    return 1;
}

int LLVMFuzzerTestOneInput_91(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t codec;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t res;
    int val;
    int idx;

    if (Size < 1) return 0;

    memset(&codec, 0, sizeof(codec));
    memset(&cfg, 0, sizeof(cfg));

    if (!init_encoder(&codec, &cfg)) return 0;

    // Use input data to drive fuzzing decisions
    val = Data[0];
    idx = (Size > 1) ? (Data[1] % 6) : 0;

    switch (idx) {
        case 0:
            // Fuzz AV1E_SET_FORCE_VIDEO_MODE
            res = aom_codec_control(&codec, AV1E_SET_FORCE_VIDEO_MODE, val);
            if (res != AOM_CODEC_OK) {
                aom_codec_destroy(&codec);
                return 0;
            }
            break;
        case 1:
            // Fuzz AV1E_SET_TIER_MASK
            res = aom_codec_control(&codec, AV1E_SET_TIER_MASK, val);
            if (res != AOM_CODEC_OK) {
                aom_codec_destroy(&codec);
                return 0;
            }
            break;
        case 2:
            // Fuzz AV1E_SET_COEFF_COST_UPD_FREQ
            res = aom_codec_control(&codec, AV1E_SET_COEFF_COST_UPD_FREQ, val);
            if (res != AOM_CODEC_OK) {
                aom_codec_destroy(&codec);
                return 0;
            }
            break;
        case 3:
            // Fuzz AOME_SET_ENABLEAUTOBWDREF
            res = aom_codec_control(&codec, AOME_SET_ENABLEAUTOBWDREF, val);
            if (res != AOM_CODEC_OK) {
                aom_codec_destroy(&codec);
                return 0;
            }
            break;
        case 4:
            // Fuzz AV1E_SET_ENABLE_QM
            res = aom_codec_control(&codec, AV1E_SET_ENABLE_QM, val);
            if (res != AOM_CODEC_OK) {
                aom_codec_destroy(&codec);
                return 0;
            }
            break;
        case 5:
            // Fuzz AV1E_SET_MTU
            res = aom_codec_control(&codec, AV1E_SET_MTU, val);
            if (res != AOM_CODEC_OK) {
                aom_codec_destroy(&codec);
                return 0;
            }
            break;
        default:
            break;
    }

    aom_codec_destroy(&codec);
    return 0;
}