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
#include "aom/aom_frame_buffer.h"
#include "aom/aom_external_partition.h"
#include "aom/aomcx.h"
#include "aom/aom_image.h"
#include "aom/aomdx.h"
#include "aom/aom_integer.h"
#include "aom/aom.h"

static int extract_int_from_data(const uint8_t *data, size_t size, size_t *offset) {
    if (*offset + sizeof(int) > size) {
        return 0;
    }
    int val;
    memcpy(&val, data + *offset, sizeof(int));
    *offset += sizeof(int);
    return val;
}

int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);
    cfg.g_w = 32;
    cfg.g_h = 32;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    size_t offset = 0;
    int val1 = extract_int_from_data(Data, Size, &offset);
    int val2 = extract_int_from_data(Data, Size, &offset);
    int val3 = extract_int_from_data(Data, Size, &offset);
    int val4 = extract_int_from_data(Data, Size, &offset);
    int val5 = extract_int_from_data(Data, Size, &offset);
    int val6 = extract_int_from_data(Data, Size, &offset);

    // Fuzz AV1E_SET_MV_COST_UPD_FREQ
    aom_codec_control(&ctx, AV1E_SET_MV_COST_UPD_FREQ, val1);

    // Fuzz AV1E_SET_ENABLE_TPL_MODEL
    aom_codec_control(&ctx, AV1E_SET_ENABLE_TPL_MODEL, val2);

    // Fuzz AV1E_SET_COEFF_COST_UPD_FREQ
    aom_codec_control(&ctx, AV1E_SET_COEFF_COST_UPD_FREQ, val3);

    // Fuzz AV1E_SET_ENABLE_QM
    aom_codec_control(&ctx, AV1E_SET_ENABLE_QM, val4);

    // Fuzz AV1E_SET_QM_MAX
    aom_codec_control(&ctx, AV1E_SET_QM_MAX, val5);

    // Fuzz AV1E_SET_MODE_COST_UPD_FREQ
    aom_codec_control(&ctx, AV1E_SET_MODE_COST_UPD_FREQ, val6);

    aom_codec_destroy(&ctx);

    return 0;
}