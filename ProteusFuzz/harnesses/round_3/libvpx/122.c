// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// VP9E_SET_DELTA_Q_UV__value at vp8cx.h:1091:1 in vp8cx.h
// VP9E_SET_TUNE_CONTENT__value at vp8cx.h:1044:1 in vp8cx.h
// VP9E_SET_TARGET_LEVEL__value at vp8cx.h:1064:1 in vp8cx.h
// VP9E_GET_LEVEL__value at vp8cx.h:1068:1 in vp8cx.h
// VP9_INVERT_TILE_DECODE_ORDER__value at vp8dx.h:208:1 in vp8dx.h
// VP9E_SET_QUANTIZER_ONE_PASS__value at vp8cx.h:1107:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "vpx/vpx_codec.h"
#include "vpx/vp8cx.h"
#include "vpx/vp8dx.h"

static int fuzz_vp9_encoder(const uint8_t *data, size_t size) {
    if (size < 8) return 0;

    vpx_codec_ctx_t ctx;
    vpx_codec_iface_t *iface = vpx_codec_vp9_cx();
    if (!iface) return 0;

    vpx_codec_enc_cfg_t cfg;
    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Configure basic settings to allow initialization
    cfg.g_w = 128;
    cfg.g_h = 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.g_error_resilient = 1;

    if (vpx_codec_enc_init(&ctx, iface, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    int32_t val;
    vpx_codec_err_t ret;

    // Fuzz VP9E_SET_DELTA_Q_UV
    // Value is typically int32_t, range -128 to 127 usually
    val = (int32_t)(data[0] << 24 | data[1] << 16 | data[2] << 8 | data[3]);
    // Clamp to reasonable range for safety, though fuzzing aims to break bounds
    ret = vpx_codec_control(&ctx, VP9E_SET_DELTA_Q_UV, val);

    // Fuzz VP9E_SET_TUNE_CONTENT
    // Valid: VP9E_TUNE_CONTENT_FILM (0), VP9E_TUNE_CONTENT_SCREEN (1)
    int tune_val = data[4] % 2;
    ret = vpx_codec_control(&ctx, VP9E_SET_TUNE_CONTENT, tune_val);

    // Fuzz VP9E_SET_TARGET_LEVEL
    // Levels typically 0-7
    int level_val = data[5] % 8;
    ret = vpx_codec_control(&ctx, VP9E_SET_TARGET_LEVEL, level_val);

    // Fuzz VP9E_GET_LEVEL
    int32_t get_level;
    ret = vpx_codec_control(&ctx, VP9E_GET_LEVEL, &get_level);

    // Fuzz VP9_INVERT_TILE_DECODE_ORDER
    // Boolean flag
    int invert_val = data[6] % 2;
    ret = vpx_codec_control(&ctx, VP9_INVERT_TILE_DECODE_ORDER, invert_val);

    // Fuzz VP9E_SET_QUANTIZER_ONE_PASS
    // Quantizer range 0-255
    int quant_val = data[7];
    ret = vpx_codec_control(&ctx, VP9E_SET_QUANTIZER_ONE_PASS, quant_val);

    // Cleanup
    vpx_codec_destroy(&ctx);
    return 0;
}

int LLVMFuzzerTestOneInput_122(const uint8_t *Data, size_t Size) {
    if (Size < 8) return 0;

    // Create a dummy file if needed
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }

    fuzz_vp9_encoder(Data, Size);

    return 0;
}