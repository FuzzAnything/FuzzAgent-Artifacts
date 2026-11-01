// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "aomcx.h"
#include "aomdx.h"
#include "aom.h"
#include "aom_codec.h"
#include "aom_external_partition.h"
#include "aom_decoder.h"
#include "aom_image.h"
#include "aom_frame_buffer.h"
#include "aom_encoder.h"
#include "aom_integer.h"

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (!ctx || !data || size < 4) return 0;

    uint32_t val = *(uint32_t *)data;
    
    // AOME_SET_ARNR_STRENGTH
    if (size >= 4) {
        aom_codec_control(ctx, AOME_SET_ARNR_STRENGTH, val);
    }
    
    // AOME_SET_ARNR_MAXFRAMES
    if (size >= 8) {
        uint32_t val2 = *(uint32_t *)(data + 4);
        aom_codec_control(ctx, AOME_SET_ARNR_MAXFRAMES, val2);
    }

    // AV1E_SET_ENABLE_TX_SIZE_SEARCH (0 or 1)
    if (size >= 12) {
        int enable = *(uint8_t *)(data + 8);
        aom_codec_control(ctx, AV1E_SET_ENABLE_TX_SIZE_SEARCH, enable);
    }

    // AV1E_SET_DELTALF_MODE (0, 1, 2)
    if (size >= 13) {
        int mode = *(uint8_t *)(data + 9) % 3;
        aom_codec_control(ctx, AV1E_SET_DELTALF_MODE, mode);
    }

    // AV1E_SET_DISABLE_TRELLIS_QUANT (0 or 1)
    if (size >= 14) {
        int disable = *(uint8_t *)(data + 10);
        aom_codec_control(ctx, AV1E_SET_DISABLE_TRELLIS_QUANT, disable);
    }

    // AV1E_SET_COLOR_RANGE (0 or 1)
    if (size >= 15) {
        int range = *(uint8_t *)(data + 11);
        aom_codec_control(ctx, AV1E_SET_COLOR_RANGE, range);
    }

    return 0;
}

int LLVMFuzzerTestOneInput_275(const uint8_t *Data, size_t Size) {
    if (Size < 16) return 0;

    aom_codec_ctx_t ctx = {0};
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    aom_codec_enc_config_default(iface, &cfg, 0);
    
    // Set minimal config from fuzz data
    cfg.g_w = (Data[0] % 256) + 16;
    cfg.g_h = (Data[1] % 256) + 16;
    cfg.g_timebase.num = (Data[2] % 256) + 1;
    cfg.g_timebase.den = (Data[3] % 256) + 1;
    cfg.g_threads = (Data[4] % 16) + 1;
    cfg.g_lag_in_frames = (Data[5] % 32);
    cfg.rc_target_bitrate = (Data[6] % 256) * 1000;
    cfg.g_profile = Data[7] % 4;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Perform control operations
    fuzz_encoder_control(&ctx, Data + 8, Size - 8);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}