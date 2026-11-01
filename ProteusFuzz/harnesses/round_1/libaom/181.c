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

static int init_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg, const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) return 0;

    if (size > 0) cfg->g_w = data[0] & 0x7F;
    if (size > 1) cfg->g_h = data[1] & 0x7F;
    if (cfg->g_w == 0 || cfg->g_h == 0) {
        cfg->g_w = 64;
        cfg->g_h = 64;
    }
    cfg->g_w = (cfg->g_w + 15) & ~15;
    cfg->g_h = (cfg->g_h + 15) & ~15;

    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = 30;
    cfg->g_threads = 0;
    cfg->g_lag_in_frames = 0;
    cfg->rc_target_bitrate = 1000;

    if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) return 0;
    return 1;
}

int LLVMFuzzerTestOneInput_181(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    if (Size < 2) return 0;

    if (!init_encoder(&ctx, &cfg, Data, Size)) return 0;

    int val1 = (Size > 0) ? (Data[0] % 2) : 0;
    int val2 = (Size > 1) ? (Data[1] % 64) : 0;
    int val3 = (Size > 2) ? (Data[2] % 10) : 0;
    int val4 = (Size > 3) ? (Data[3] % 2) : 0;
    int val5 = (Size > 4) ? (Data[4] % 100) : 0;
    int val6 = (Size > 5) ? (Data[5] % 100) : 0;

    aom_codec_control(&ctx, AV1E_SET_ENABLE_TX_SIZE_SEARCH, val1);
    aom_codec_control(&ctx, AV1E_SET_QM_MIN, val2);
    aom_codec_control(&ctx, AV1E_SET_DELTAQ_STRENGTH, val3);
    aom_codec_control(&ctx, AV1E_SET_ENABLE_OVERLAY, val4);
    aom_codec_control(&ctx, AV1E_SET_MV_COST_UPD_FREQ, val5);
    aom_codec_control(&ctx, AV1E_SET_MODE_COST_UPD_FREQ, val6);

    aom_codec_destroy(&ctx);

    return 0;
}