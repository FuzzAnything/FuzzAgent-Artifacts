// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// VP8E_SET_CPUUSED__value at vp8cx.h:984:1 in vp8cx.h
// VP8E_SET_TUNING__value at vp8cx.h:1006:1 in vp8cx.h
// VP8E_SET_SCALEMODE__value at vp8cx.h:982:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// VP9E_SET_TUNE_CONTENT__value at vp8cx.h:1044:1 in vp8cx.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "vpx/vpx_codec.h"
#include "vpx/vp8cx.h"
#include "vpx/vpx_encoder.h"

/* Helper to initialize configuration with fuzzed data */
static int init_config(const uint8_t *data, size_t size, vpx_codec_enc_cfg_t *cfg) {
    if (!data || size < sizeof(vpx_codec_enc_cfg_t)) {
        return 0;
    }

    /* Initialize with defaults first to ensure safety */
    /* We use NULL for iface here to get generic defaults, 
       though typically one passes a specific iface. 
       Since we don't have the iface yet, we rely on the macro or manual init if available.
       However, vpx_codec_enc_config_default requires an iface. 
       We will manually set critical fields to safe defaults instead. */
    
    memset(cfg, 0, sizeof(vpx_codec_enc_cfg_t));
    
    /* Overwrite with fuzzed data, but clamp critical values */
    memcpy(cfg, data, sizeof(vpx_codec_enc_cfg_t));

    /* Ensure width and height are non-zero and reasonable */
    if (cfg->g_w == 0) cfg->g_w = 16;
    if (cfg->g_h == 0) cfg->g_h = 16;
    if (cfg->g_w > 8192) cfg->g_w = 8192;
    if (cfg->g_h > 8192) cfg->g_h = 8192;

    /* Ensure bitrate is positive */
    if (cfg->rc_target_bitrate == 0) cfg->rc_target_bitrate = 1;

    /* Ensure timebase is valid */
    if (cfg->g_timebase.den == 0) {
        cfg->g_timebase.num = 1;
        cfg->g_timebase.den = 1;
    }

    /* Ensure threads are valid */
    if (cfg->g_threads == 0) cfg->g_threads = 1;

    return 1;
}

int LLVMFuzzerTestOneInput_105(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    const vpx_codec_iface_t *iface = NULL;
    vpx_codec_err_t ret;

    /* Initialize configuration */
    if (!init_config(Data, Size, &cfg)) {
        return 0;
    }

    /* Try VP8 Encoder */
    iface = vpx_codec_vp8_cx();
    if (iface) {
        ret = vpx_codec_enc_init_ver(&ctx, iface, &cfg, 0, VPX_ENCODER_ABI_VERSION);
        if (ret == VPX_CODEC_OK) {
            /* Fuzz VP8E_SET_CPUUSED */
            int cpu_used = Data[0] % 9;
            vpx_codec_control(&ctx, VP8E_SET_CPUUSED, cpu_used);

            /* Fuzz VP8E_SET_TUNING */
            int tuning = Data[1] % 10;
            vpx_codec_control(&ctx, VP8E_SET_TUNING, tuning);

            /* Fuzz VP8E_SET_SCALEMODE */
            vpx_scaling_mode_t scale_mode;
            scale_mode.h_scaling_mode = (VPX_SCALING_MODE)(Data[2] % 4);
            scale_mode.v_scaling_mode = (VPX_SCALING_MODE)(Data[3] % 4);
            vpx_codec_control(&ctx, VP8E_SET_SCALEMODE, &scale_mode);

            /* Cleanup */
            vpx_codec_destroy(&ctx);
        }
    }

    /* Try VP9 Encoder */
    iface = vpx_codec_vp9_cx();
    if (iface) {
        ret = vpx_codec_enc_init_ver(&ctx, iface, &cfg, 0, VPX_ENCODER_ABI_VERSION);
        if (ret == VPX_CODEC_OK) {
            /* Fuzz VP9E_SET_TUNE_CONTENT */
            /* VP9E_SET_TUNE_CONTENT expects an int enum: VP9E_CONTENT_DEFAULT, SCREEN, FILM */
            int content_type = Data[4] % 3; 
            vpx_codec_control(&ctx, VP9E_SET_TUNE_CONTENT, content_type);

            /* Cleanup */
            vpx_codec_destroy(&ctx);
        }
    }

    return 0;
}