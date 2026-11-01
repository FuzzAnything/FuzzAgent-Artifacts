// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
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
#include <limits.h>

#include "aom/aomcx.h"
#include "aom/aomdx.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_decoder.h"
#include "aom/aom_image.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_encoder.h"
#include "aom/aom_integer.h"

static int is_valid_int(const uint8_t *data, size_t size) {
    if (size < 4) return 0;
    return 1;
}

int LLVMFuzzerTestOneInput_126(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    const aom_codec_iface_t *codec_iface = aom_codec_av1_cx();
    if (codec_iface == NULL) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(codec_iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;

    if (aom_codec_enc_init(&ctx, codec_iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    uint8_t cmd_selector = Data[0] % 6;

    switch (cmd_selector) {
        case 0: {
            // AV1E_SET_ENABLE_DIFF_WTD_COMP: expects int (0 or 1)
            if (is_valid_int(Data + 1, Size - 1)) {
                int32_t val;
                memcpy(&val, Data + 1, 4);
                int enable = (val != 0) ? 1 : 0;
                aom_codec_control(&ctx, AV1E_SET_ENABLE_DIFF_WTD_COMP, enable);
            }
            break;
        }
        case 1: {
            // AV1E_SET_ENABLE_TX_SIZE_SEARCH: expects int (0 or 1)
            if (is_valid_int(Data + 1, Size - 1)) {
                int32_t val;
                memcpy(&val, Data + 1, 4);
                int enable = (val != 0) ? 1 : 0;
                aom_codec_control(&ctx, AV1E_SET_ENABLE_TX_SIZE_SEARCH, enable);
            }
            break;
        }
        case 2: {
            // AV1E_SET_DELTALF_MODE: expects int (mode)
            if (is_valid_int(Data + 1, Size - 1)) {
                int32_t val;
                memcpy(&val, Data + 1, 4);
                aom_codec_control(&ctx, AV1E_SET_DELTALF_MODE, val);
            }
            break;
        }
        case 3: {
            // AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE: expects int (0 or 1)
            if (is_valid_int(Data + 1, Size - 1)) {
                int32_t val;
                memcpy(&val, Data + 1, 4);
                int enable = (val != 0) ? 1 : 0;
                aom_codec_control(&ctx, AV1E_SET_ENABLE_LOW_COMPLEXITY_DECODE, enable);
            }
            break;
        }
        case 4: {
            // AV1E_SET_MODE_COST_UPD_FREQ: expects int (frequency)
            if (is_valid_int(Data + 1, Size - 1)) {
                int32_t val;
                memcpy(&val, Data + 1, 4);
                aom_codec_control(&ctx, AV1E_SET_MODE_COST_UPD_FREQ, val);
            }
            break;
        }
        case 5: {
            // AOME_SET_MAX_INTRA_BITRATE_PCT: expects int (percentage)
            if (is_valid_int(Data + 1, Size - 1)) {
                int32_t val;
                memcpy(&val, Data + 1, 4);
                aom_codec_control(&ctx, AOME_SET_MAX_INTRA_BITRATE_PCT, val);
            }
            break;
        }
    }

    aom_codec_destroy(&ctx);
    return 0;
}