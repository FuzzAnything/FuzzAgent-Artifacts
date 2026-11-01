// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
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
#include <aom/aom_encoder.h>
#include <aom/aom_image.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_integer.h>

// Define the control IDs if they are not defined in the headers
// These correspond to the AV1 encoder specific control commands
#ifndef AV1E_SET_RTC_EXTERNAL_RC
#define AV1E_SET_RTC_EXTERNAL_RC 0x1000
#endif
#ifndef AV1E_SET_TIER_MASK
#define AV1E_SET_TIER_MASK 0x1001
#endif
#ifndef AV1E_SET_COLOR_PRIMARIES
#define AV1E_SET_COLOR_PRIMARIES 0x1002
#endif
#ifndef AV1E_SET_CHROMA_SUBSAMPLING_X
#define AV1E_SET_CHROMA_SUBSAMPLING_X 0x1003
#endif
#ifndef AV1E_SET_MV_COST_UPD_FREQ
#define AV1E_SET_MV_COST_UPD_FREQ 0x1004
#endif
#ifndef AV1E_SET_ENABLE_QM
#define AV1E_SET_ENABLE_QM 0x1005
#endif

int LLVMFuzzerTestOneInput_130(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    ctx.iface = iface;
    ctx.name = "av1";
    ctx.err = AOM_CODEC_OK;
    ctx.err_detail = NULL;
    ctx.init_flags = 0;
    ctx.config.raw = NULL;
    ctx.priv = NULL;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use input data to configure encoder settings
    cfg.g_w = (Data[0] % 256) + 16;
    cfg.g_h = (Data[1] % 256) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.g_lag_in_frames = 0;

    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    int result = 0;

    // Fuzz AV1E_SET_RTC_EXTERNAL_RC
    if (Size >= 4) {
        int32_t rtc_val = (int32_t)(Data[2] * 256 + Data[3]);
        result += aom_codec_control(&ctx, AV1E_SET_RTC_EXTERNAL_RC, &rtc_val);
    }

    // Fuzz AV1E_SET_TIER_MASK
    if (Size >= 5) {
        uint8_t tier_mask = Data[4];
        result += aom_codec_control(&ctx, AV1E_SET_TIER_MASK, &tier_mask);
    }

    // Fuzz AV1E_SET_COLOR_PRIMARIES
    if (Size >= 6) {
        int32_t color_primaries = Data[5];
        result += aom_codec_control(&ctx, AV1E_SET_COLOR_PRIMARIES, &color_primaries);
    }

    // Fuzz AV1E_SET_CHROMA_SUBSAMPLING_X
    if (Size >= 7) {
        int32_t chroma_x = Data[6];
        result += aom_codec_control(&ctx, AV1E_SET_CHROMA_SUBSAMPLING_X, &chroma_x);
    }

    // Fuzz AV1E_SET_MV_COST_UPD_FREQ
    if (Size >= 8) {
        int32_t mv_cost_freq = Data[7];
        result += aom_codec_control(&ctx, AV1E_SET_MV_COST_UPD_FREQ, &mv_cost_freq);
    }

    // Fuzz AV1E_SET_ENABLE_QM
    if (Size >= 9) {
        int32_t enable_qm = Data[8] ? 1 : 0;
        result += aom_codec_control(&ctx, AV1E_SET_ENABLE_QM, &enable_qm);
    }

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}