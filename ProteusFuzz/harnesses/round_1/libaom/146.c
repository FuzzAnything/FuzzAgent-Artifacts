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

static int initialize_encoder(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t ret;

    iface = aom_codec_av1_cx();
    if (!iface) return -1;

    ret = aom_codec_enc_config_default(iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) return -1;

    // Use fuzz data to vary configuration slightly if size allows
    // Ensure we have enough data before accessing indices
    if (size > 0) {
        cfg.g_w = (data[0] % 1024) + 16;
    }
    if (size > 1) {
        cfg.g_h = (data[1] % 1024) + 16;
    }
    cfg.g_bit_depth = 8;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    
    if (size > 2) {
        cfg.rc_target_bitrate = (data[2] % 5000) + 100;
    }

    ret = aom_codec_enc_init(ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) return -1;

    return 0;
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
    aom_codec_destroy(ctx);
}

int LLVMFuzzerTestOneInput_146(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    if (initialize_encoder(&ctx, Data, Size) != 0) {
        return 0;
    }

    // Fuzz various control functions using data from input
    // Ensure bounds checking for each access
    int tier_mask = 0;
    int color_primaries = 0;
    int coeff_cost_upd_freq = 0;
    int min_cr = 0;
    int chroma_subsampling_x = 0;
    int rtc_external_rc = 0;

    if (Size > 0) tier_mask = Data[0];
    if (Size > 1) color_primaries = Data[1];
    if (Size > 2) coeff_cost_upd_freq = Data[2];
    if (Size > 3) min_cr = Data[3];
    if (Size > 4) chroma_subsampling_x = Data[4];
    if (Size > 5) rtc_external_rc = Data[5];

    // AV1E_SET_TIER_MASK
    aom_codec_control(&ctx, AV1E_SET_TIER_MASK, tier_mask);

    // AV1E_SET_COLOR_PRIMARIES
    aom_codec_control(&ctx, AV1E_SET_COLOR_PRIMARIES, color_primaries);

    // AV1E_SET_COEFF_COST_UPD_FREQ
    aom_codec_control(&ctx, AV1E_SET_COEFF_COST_UPD_FREQ, coeff_cost_upd_freq);

    // AV1E_SET_MIN_CR
    aom_codec_control(&ctx, AV1E_SET_MIN_CR, min_cr);

    // AV1E_SET_CHROMA_SUBSAMPLING_X
    aom_codec_control(&ctx, AV1E_SET_CHROMA_SUBSAMPLING_X, chroma_subsampling_x);

    // AV1E_SET_RTC_EXTERNAL_RC
    aom_codec_control(&ctx, AV1E_SET_RTC_EXTERNAL_RC, rtc_external_rc);

    cleanup_encoder(&ctx);

    return 0;
}