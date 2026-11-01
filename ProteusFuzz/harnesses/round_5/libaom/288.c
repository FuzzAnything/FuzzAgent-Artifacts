// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom.h"
#include "aom_codec.h"
#include "aom_encoder.h"
#include "aom_decoder.h"
#include "aom_image.h"
#include "aomcx.h"
#include "aomdx.h"
#include "aom_external_partition.h"
#include "aom_frame_buffer.h"
#include "aom_integer.h"

static int init_encoder(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return -1;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) return -1;

    if (size >= 2) {
        cfg.g_w = data[0] + 16;
        cfg.g_h = data[1] + 16;
    } else {
        cfg.g_w = 16;
        cfg.g_h = 16;
    }
    cfg.g_threads = 1;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;

    if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) return -1;

    return 0;
}

static int init_decoder(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) return -1;

    aom_codec_dec_cfg_t cfg;
    // aom_codec_dec_config_default does not exist in standard libaom headers.
    // We must manually initialize the structure.
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;

    if (aom_codec_dec_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) return -1;

    return 0;
}

int LLVMFuzzerTestOneInput_288(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    aom_codec_err_t err;
    int ret = 0;

    int is_encoder = (Data[0] % 2);

    if (is_encoder) {
        if (init_encoder(&ctx, Data, Size) != 0) return 0;
    } else {
        if (init_decoder(&ctx, Data, Size) != 0) return 0;
    }

    for (size_t i = 0; i < Size; i += 4) {
        if (i + 4 > Size) break;
        
        uint32_t cmd = 0;
        memcpy(&cmd, Data + i, 4);
        
        void *arg = (void *)(Data + i + 4);
        size_t arg_size = Size - (i + 4);
        
        if (arg_size == 0) arg = NULL;
        
        err = aom_codec_control(&ctx, cmd, arg);
        (void)err;
    }

    aom_codec_destroy(&ctx);

    return 0;
}