// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
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

static int fuzz_encoder_control(const uint8_t *data, size_t size) {
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    aom_codec_err_t res;
    int ret = 0;

    if (!iface) return 0;

    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    res = aom_codec_enc_config_default(iface, &cfg, 0);
    if (res != AOM_CODEC_OK) return 0;

    cfg.g_w = 16;
    cfg.g_h = 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;

    res = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) return 0;

    // AV1E_GET_NUM_OPERATING_POINTS
    int num_ops;
    res = aom_codec_control(&ctx, AV1E_GET_NUM_OPERATING_POINTS, &num_ops);
    (void)res;

    // AV1E_SET_SVC_FRAME_DROP_MODE
    int drop_mode = 0;
    if (size > 0) drop_mode = data[0] % 4;
    res = aom_codec_control(&ctx, AV1E_SET_SVC_FRAME_DROP_MODE, &drop_mode);
    (void)res;

    // AV1E_SET_INTRA_DEFAULT_TX_ONLY
    int intra_tx = 0;
    if (size > 1) intra_tx = data[1] % 2;
    res = aom_codec_control(&ctx, AV1E_SET_INTRA_DEFAULT_TX_ONLY, &intra_tx);
    (void)res;

    // AV1E_SET_ENABLE_DIFF_WTD_COMP
    int diff_wtd = 0;
    if (size > 2) diff_wtd = data[2] % 2;
    res = aom_codec_control(&ctx, AV1E_SET_ENABLE_DIFF_WTD_COMP, &diff_wtd);
    (void)res;

    // AV1_GET_ACCOUNTING (Attempt on encoder context, may fail or be ignored)
    Accounting *acc = NULL;
    res = aom_codec_control(&ctx, AV1_GET_ACCOUNTING, &acc);
    (void)res;

    aom_codec_destroy(&ctx);
    return ret;
}

static int fuzz_decoder_control(const uint8_t *data, size_t size) {
    aom_codec_ctx_t ctx;
    aom_codec_dec_cfg_t cfg;
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    aom_codec_err_t res;
    int ret = 0;

    if (!iface) return 0;

    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    cfg.threads = 1;
    // Removed cfg.cpu_used as it is not a member of aom_codec_dec_cfg in newer versions

    res = aom_codec_dec_init(&ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) return 0;

    // AV1_GET_ACCOUNTING
    Accounting *acc = NULL;
    res = aom_codec_control(&ctx, AV1_GET_ACCOUNTING, &acc);
    (void)res;

    aom_codec_destroy(&ctx);
    return ret;
}

int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Fuzz encoder controls
    fuzz_encoder_control(Data, Size);

    // Fuzz decoder controls
    fuzz_decoder_control(Data, Size);

    return 0;
}