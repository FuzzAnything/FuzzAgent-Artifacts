// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aom_codec.h"
#include "aom/aomcx.h"
#include "aom/aomdx.h"
#include "aom/aom_integer.h"
#include "aom/aom_image.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_external_partition.h"

static int fuzz_control_id(const uint8_t *data, size_t size) {
    if (size < 1) return 0;
    int id = data[0] % 10;
    switch (id) {
        case 0: return AV1E_SET_REDUCED_REFERENCE_SET;
        case 1: return AV1E_SET_ENABLE_CFL_INTRA;
        case 2: return AV1E_SET_ENABLE_INTRABC;
        case 3: return AV1E_SET_TILE_ROWS;
        case 4: return AV1E_SET_QUANTIZER_ONE_PASS;
        case 5: return AOME_SET_CPUUSED;
        case 6: return AV1E_SET_ENABLE_TX64;
        case 7: return AV1E_SET_EXTERNAL_PARTITION;
        case 8: return AV1E_SET_ENABLE_TX64;
        case 9: return AV1E_SET_ENABLE_1TO4_PARTITIONS;
        default: return 0;
    }
}

static int fuzz_control_arg(const uint8_t *data, size_t size, int ctrl_id) {
    if (size < 2) return 0;
    int val = (data[1] << 8) | data[2];
    if (size > 3) val = (val << 8) | data[3];
    if (size > 4) val = (val << 8) | data[4];
    
    if (ctrl_id == AV1E_SET_REDUCED_REFERENCE_SET ||
        ctrl_id == AV1E_SET_ENABLE_CFL_INTRA ||
        ctrl_id == AV1E_SET_ENABLE_INTRABC ||
        ctrl_id == AV1E_SET_QUANTIZER_ONE_PASS ||
        ctrl_id == AV1E_SET_ENABLE_TX64 ||
        ctrl_id == AV1E_SET_EXTERNAL_PARTITION ||
        ctrl_id == AV1E_SET_ENABLE_1TO4_PARTITIONS) {
        return val % 2;
    }
    if (ctrl_id == AV1E_SET_TILE_ROWS) {
        return val % 8;
    }
    if (ctrl_id == AOME_SET_CPUUSED) {
        return (val % 9);
    }
    return val;
}

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    if (Size < 5) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t err;

    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    err = aom_codec_enc_config_default(iface, &cfg, 0);
    if (err != AOM_CODEC_OK) return 0;

    cfg.g_w = (Data[0] % 256) + 16;
    cfg.g_h = (Data[1] % 256) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = (Data[2] % 4) + 1;
    cfg.rc_target_bitrate = (Data[3] % 1000) + 100;
    cfg.g_profile = (Data[4] % 4);

    err = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        aom_codec_destroy(&ctx);
        return 0;
    }

    int ctrl_id = fuzz_control_id(Data, Size);
    if (ctrl_id == 0) {
        aom_codec_destroy(&ctx);
        return 0;
    }

    int arg_val = fuzz_control_arg(Data, Size, ctrl_id);

    err = aom_codec_control(&ctx, ctrl_id, arg_val);

    aom_codec_destroy(&ctx);

    return 0;
}