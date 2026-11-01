// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control_typechecked_AV1E_SET_ENABLE_ORDER_HINT at aomcx.h:2078:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_COEFF_COST_UPD_FREQ at aomcx.h:2219:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_ENABLE_INTERINTER_WEDGE at aomcx.h:2120:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_MIN_PARTITION_SIZE at aomcx.h:2069:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_ENABLE_INTRA_EDGE_FILTER at aomcx.h:2075:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_RTC_EXTERNAL_RC at aomcx.h:2306:1 in aomcx.h
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

static int initialize_encoder(aom_codec_ctx_t *codec, aom_codec_enc_cfg_t *cfg) {
    const aom_codec_iface_t *iface;
    aom_codec_err_t ret;

    if (!codec || !cfg) return 0;

    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    ret = aom_codec_enc_config_default(iface, cfg, 0);
    if (ret != AOM_CODEC_OK) return 0;

    cfg->g_w = 128;
    cfg->g_h = 128;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_threads = 1;
    cfg->g_error_resilient = 0;

    ret = aom_codec_enc_init(codec, iface, cfg, 0);
    if (ret != AOM_CODEC_OK) return 0;

    return 1;
}

int LLVMFuzzerTestOneInput_134(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t codec;
    aom_codec_enc_cfg_t cfg;
    int val;
    
    if (Size < sizeof(int)) {
        return 0;
    }

    memcpy(&val, Data, sizeof(int));

    if (!initialize_encoder(&codec, &cfg)) {
        return 0;
    }

    // The typechecked functions require 3 arguments: ctx, ctrl_id, data
    // We pass the control ID explicitly as the second argument.
    
    // 1. AV1E_SET_ENABLE_ORDER_HINT
    aom_codec_control_typechecked_AV1E_SET_ENABLE_ORDER_HINT(&codec, AV1E_SET_ENABLE_ORDER_HINT, val);

    // 2. AV1E_SET_COEFF_COST_UPD_FREQ
    aom_codec_control_typechecked_AV1E_SET_COEFF_COST_UPD_FREQ(&codec, AV1E_SET_COEFF_COST_UPD_FREQ, (unsigned int)val);

    // 3. AV1E_SET_ENABLE_INTERINTER_WEDGE
    aom_codec_control_typechecked_AV1E_SET_ENABLE_INTERINTER_WEDGE(&codec, AV1E_SET_ENABLE_INTERINTER_WEDGE, val);

    // 4. AV1E_SET_MIN_PARTITION_SIZE
    aom_codec_control_typechecked_AV1E_SET_MIN_PARTITION_SIZE(&codec, AV1E_SET_MIN_PARTITION_SIZE, val);

    // 5. AV1E_SET_ENABLE_INTRA_EDGE_FILTER
    aom_codec_control_typechecked_AV1E_SET_ENABLE_INTRA_EDGE_FILTER(&codec, AV1E_SET_ENABLE_INTRA_EDGE_FILTER, val);

    // 6. AV1E_SET_RTC_EXTERNAL_RC
    aom_codec_control_typechecked_AV1E_SET_RTC_EXTERNAL_RC(&codec, AV1E_SET_RTC_EXTERNAL_RC, val);

    aom_codec_destroy(&codec);

    return 0;
}