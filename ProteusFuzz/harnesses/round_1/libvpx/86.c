// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// VP8E_SET_CPUUSED__value at vp8cx.h:984:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// VP9E_SET_COLOR_SPACE__value at vp8cx.h:1050:1 in vp8cx.h
// VP9E_SET_QUANTIZER_ONE_PASS__value at vp8cx.h:1107:1 in vp8cx.h
// VP9E_SET_TILE_ROWS__value at vp8cx.h:1026:1 in vp8cx.h
// VP9E_SET_ALT_REF_AQ__value at vp8cx.h:1070:1 in vp8cx.h
// VP9E_SET_COLOR_RANGE__value at vp8cx.h:1058:1 in vp8cx.h
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

int LLVMFuzzerTestOneInput_86(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Fuzz VP8 Encoder
    {
        vpx_codec_ctx_t ctx;
        vpx_codec_iface_t *iface;
        vpx_codec_enc_cfg_t cfg;
        int ret;

        iface = vpx_codec_vp8_cx();
        if (!iface) return 0;

        ret = vpx_codec_enc_config_default(iface, &cfg, 0);
        if (ret != VPX_CODEC_OK) return 0;

        cfg.g_w = 16;
        cfg.g_h = 16;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 30;
        cfg.rc_target_bitrate = 100;

        ret = vpx_codec_enc_init(&ctx, iface, &cfg, 0);
        if (ret != VPX_CODEC_OK) {
            vpx_codec_destroy(&ctx);
            return 0;
        }

        // Fuzz VP8E_SET_CPUUSED
        if (Size > 0) {
            int cpu_used = Data[0] % 9; // Range 0-8
            vpx_codec_control(&ctx, VP8E_SET_CPUUSED, cpu_used);
        }

        vpx_codec_destroy(&ctx);
    }

    // Fuzz VP9 Encoder
    {
        vpx_codec_ctx_t ctx;
        vpx_codec_iface_t *iface;
        vpx_codec_enc_cfg_t cfg;
        int ret;

        iface = vpx_codec_vp9_cx();
        if (!iface) return 0;

        ret = vpx_codec_enc_config_default(iface, &cfg, 0);
        if (ret != VPX_CODEC_OK) return 0;

        cfg.g_w = 16;
        cfg.g_h = 16;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 30;
        cfg.rc_target_bitrate = 100;

        ret = vpx_codec_enc_init(&ctx, iface, &cfg, 0);
        if (ret != VPX_CODEC_OK) {
            vpx_codec_destroy(&ctx);
            return 0;
        }

        // Fuzz VP9E_SET_COLOR_SPACE
        if (Size > 0) {
            int color_space = Data[0] % 10;
            vpx_codec_control(&ctx, VP9E_SET_COLOR_SPACE, color_space);
        }

        // Fuzz VP9E_SET_QUANTIZER_ONE_PASS
        if (Size > 1) {
            int q = (Data[1] * 256 + (Size > 2 ? Data[2] : 0)) % 100;
            vpx_codec_control(&ctx, VP9E_SET_QUANTIZER_ONE_PASS, q);
        }

        // Fuzz VP9E_SET_TILE_ROWS
        if (Size > 2) {
            int tile_rows = Data[2] % 16;
            vpx_codec_control(&ctx, VP9E_SET_TILE_ROWS, tile_rows);
        }

        // Fuzz VP9E_SET_ALT_REF_AQ
        if (Size > 3) {
            int alt_ref_aq = Data[3] % 2;
            vpx_codec_control(&ctx, VP9E_SET_ALT_REF_AQ, alt_ref_aq);
        }

        // Fuzz VP9E_SET_COLOR_RANGE
        if (Size > 4) {
            int color_range = Data[4] % 2;
            vpx_codec_control(&ctx, VP9E_SET_COLOR_RANGE, color_range);
        }

        vpx_codec_destroy(&ctx);
    }

    return 0;
}