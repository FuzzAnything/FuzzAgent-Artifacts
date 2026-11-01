// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
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

#include "aom/aom_decoder.h"
#include "aom/aomdx.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_integer.h"
#include "aom/aom_image.h"
#include "aom/aom_encoder.h"
#include "aom/aomcx.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_frame_buffer.h"

static int init_encoder(aom_codec_ctx_t *codec, aom_codec_enc_cfg_t *cfg) {
    const aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) return 0;

    if (aom_codec_enc_init(codec, iface, cfg, 0) != AOM_CODEC_OK) return 0;

    return 1;
}

static void cleanup_encoder(aom_codec_ctx_t *codec) {
    aom_codec_destroy(codec);
}

static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_301(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t codec;
    aom_codec_enc_cfg_t cfg;
    memset(&codec, 0, sizeof(codec));
    memset(&cfg, 0, sizeof(cfg));

    if (!init_encoder(&codec, &cfg)) return 0;

    // Use input data to derive parameters for control functions
    int val = 0;
    if (Size > 0) val = Data[0];
    int val2 = 0;
    if (Size > 1) val2 = Data[1];

    // Fuzz AV1E_SET_EXTERNAL_RATE_CONTROL (Corrected identifier)
    aom_rc_funcs_t rc_funcs;
    memset(&rc_funcs, 0, sizeof(rc_funcs));
    rc_funcs.rc_type = (aom_rc_type_t)(val & 0x7);
    aom_codec_control(&codec, AV1E_SET_EXTERNAL_RATE_CONTROL, &rc_funcs);

    // Fuzz AV1E_SET_ENABLE_TX_SIZE_SEARCH
    aom_codec_control(&codec, AV1E_SET_ENABLE_TX_SIZE_SEARCH, val2);

    // Fuzz AV1E_ENABLE_EXT_TILE_DEBUG
    aom_codec_control(&codec, AV1E_ENABLE_EXT_TILE_DEBUG, val);

    // Fuzz AV1E_SET_ENABLE_INTRABC
    aom_codec_control(&codec, AV1E_SET_ENABLE_INTRABC, val2);

    // Fuzz AV1E_SET_ENABLE_TX64
    aom_codec_control(&codec, AV1E_SET_ENABLE_TX64, val);

    // Fuzz AOME_SET_CQ_LEVEL
    int cq_level = val * 10;
    if (cq_level > 255) cq_level = 255;
    aom_codec_control(&codec, AOME_SET_CQ_LEVEL, cq_level);

    // Additional diversity: try with different values derived from data
    for (size_t i = 0; i < Size && i < 10; i++) {
        int test_val = Data[i];
        aom_codec_control(&codec, AV1E_SET_ENABLE_TX_SIZE_SEARCH, test_val);
        aom_codec_control(&codec, AV1E_SET_ENABLE_INTRABC, test_val);
        aom_codec_control(&codec, AV1E_SET_ENABLE_TX64, test_val);
    }

    // Write dummy file if data exists
    if (Size > 0) {
        write_dummy_file(Data, Size);
    }

    cleanup_encoder(&codec);
    return 0;
}