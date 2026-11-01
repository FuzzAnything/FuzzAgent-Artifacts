// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aomdx.h"
#include "aom_encoder.h"
#include "aom_frame_buffer.h"
#include "aom_decoder.h"
#include "aom_image.h"
#include "aom.h"
#include "aom_integer.h"
#include "aom_codec.h"
#include "aomcx.h"
#include "aom_external_partition.h"

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    int ret = 0;
    int val = 0;

    if (size > 0) {
        val = data[0] & 0x1;
    }

    // AV1E_SET_ENABLE_PALETTE
    ret = aom_codec_control(ctx, AV1E_SET_ENABLE_PALETTE, val);
    (void)ret;

    // AV1E_SET_INTRA_DCT_ONLY
    if (size > 1) val = data[1] & 0x1;
    ret = aom_codec_control(ctx, AV1E_SET_INTRA_DCT_ONLY, val);
    (void)ret;

    // AV1E_SET_ENABLE_INTRABC
    if (size > 2) val = data[2] & 0x1;
    ret = aom_codec_control(ctx, AV1E_SET_ENABLE_INTRABC, val);
    (void)ret;

    // AV1E_SET_INTRA_DEFAULT_TX_ONLY
    if (size > 3) val = data[3] & 0x1;
    ret = aom_codec_control(ctx, AV1E_SET_INTRA_DEFAULT_TX_ONLY, val);
    (void)ret;

    // AV1E_SET_ENABLE_PAETH_INTRA
    if (size > 4) val = data[4] & 0x1;
    ret = aom_codec_control(ctx, AV1E_SET_ENABLE_PAETH_INTRA, val);
    (void)ret;

    // AV1E_SET_ENABLE_CHROMA_DELTAQ
    if (size > 5) val = data[5] & 0x1;
    ret = aom_codec_control(ctx, AV1E_SET_ENABLE_CHROMA_DELTAQ, val);
    (void)ret;

    return 0;
}

int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    aom_codec_iface_t *iface;
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t err;
    int ret = 0;

    if (Size < 2) {
        return 0;
    }

    iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    err = aom_codec_enc_config_default(iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    // Use input data to configure some parameters safely
    cfg.g_w = (Data[0] % 256) + 16;
    cfg.g_h = (Data[1] % 256) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    
    if (Size > 2) {
        cfg.rc_target_bitrate = (Data[2] % 100) + 1000;
    } else {
        cfg.rc_target_bitrate = 1000;
    }

    err = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        return 0;
    }

    ret = fuzz_encoder_control(&ctx, Data, Size);

    aom_codec_destroy(&ctx);

    return ret;
}