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
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aomcx.h>
#include <aom/aom_external_partition.h>

static aom_codec_ctx_t *init_encoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *codec = NULL;
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t ret;
    size_t w, h;

    if (size < 4) return NULL;

    w = (size_t)data[0];
    h = (size_t)data[1];
    if (w == 0 || h == 0 || w > 4096 || h > 4096) {
        w = 64;
        h = 64;
    }

    iface = aom_codec_av1_cx();
    if (!iface) return NULL;

    ret = aom_codec_enc_config_default(iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) return NULL;

    cfg.g_w = w;
    cfg.g_h = h;
    cfg.g_bit_depth = 8;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;

    codec = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (!codec) return NULL;

    ret = aom_codec_enc_init(codec, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        free(codec);
        return NULL;
    }

    return codec;
}

static void cleanup_encoder(aom_codec_ctx_t *codec) {
    if (codec) {
        aom_codec_destroy(codec);
        free(codec);
    }
}

int LLVMFuzzerTestOneInput_218(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t *codec;
    aom_codec_err_t ret;
    int tier, x_sampling, ref_mvs, color_primaries, dist_8x8, rtc_external_rc;

    if (Size < 6) return 0;

    tier = Data[0] & 0x1;
    x_sampling = Data[1] & 0x3;
    ref_mvs = Data[2] & 0x1;
    color_primaries = Data[3] & 0x7;
    dist_8x8 = Data[4] & 0x1;
    rtc_external_rc = Data[5] & 0x1;

    codec = init_encoder(Data, Size);
    if (!codec) return 0;

    ret = aom_codec_control(codec, AV1E_SET_TIER_MASK, tier);
    (void)ret;

    ret = aom_codec_control(codec, AV1E_SET_CHROMA_SUBSAMPLING_X, x_sampling);
    (void)ret;

    ret = aom_codec_control(codec, AV1E_SET_ENABLE_REF_FRAME_MVS, ref_mvs);
    (void)ret;

    ret = aom_codec_control(codec, AV1E_SET_COLOR_PRIMARIES, color_primaries);
    (void)ret;

    // AV1E_SET_ENABLE_DIST_8X8 is not a valid constant in current libaom.
    // Skipping this control call to ensure compilation.
    // If a specific test for this logic is needed, it should be replaced with a valid constant or removed.

    ret = aom_codec_control(codec, AV1E_SET_RTC_EXTERNAL_RC, rtc_external_rc);
    (void)ret;

    cleanup_encoder(codec);

    return 0;
}