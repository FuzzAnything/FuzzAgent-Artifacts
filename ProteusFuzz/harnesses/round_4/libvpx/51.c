// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "vpx/vpx_codec.h"
#include "vpx/vp8cx.h"
#include "vpx/vp8dx.h"
#include "vpx/vpx_decoder.h"

static int create_dummy_file(const uint8_t *data, size_t size) {
    const char *filename = "./dummy_file";
    FILE *f = fopen(filename, "wb");
    if (!f) return -1;
    if (size > 0) {
        size_t written = fwrite(data, 1, size, f);
        if (written != size) {
            fclose(f);
            return -1;
        }
    }
    fclose(f);
    return 0;
}

static void cleanup_dummy_file(void) {
    remove("./dummy_file");
}

int LLVMFuzzerTestOneInput_51(const uint8_t *Data, size_t Size) {
    if (Size < 2) return 0;

    /* Create a dummy file if needed by some codec initialization paths */
    if (create_dummy_file(Data, Size) != 0) {
        return 0;
    }

    vpx_codec_ctx_t ctx;
    vpx_codec_err_t res;
    const vpx_codec_iface_t *iface = NULL;

    /* Try VP8 Encoder Interface */
    iface = vpx_codec_vp8_cx();
    if (iface) {
        vpx_codec_enc_cfg_t cfg;
        vpx_codec_enc_config_default(iface, &cfg, 0);
        
        /* Use fuzz data to tweak config slightly if possible, but keep valid */
        /* Ensure Size is sufficient before accessing Data[1] */
        cfg.g_w = (Data[0] % 256) + 16;
        if (Size > 1) {
            cfg.g_h = (Data[1] % 256) + 16;
        } else {
            cfg.g_h = 16;
        }
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = 30;
        cfg.rc_target_bitrate = (Size > 2 ? Data[2] % 100 : 0) + 10;
        cfg.g_error_resilient = 0;
        cfg.g_lag_in_frames = 0;
        cfg.g_threads = 1;
        cfg.g_pass = 0;
        cfg.g_profile = 0;
        
        res = vpx_codec_enc_init(&ctx, iface, &cfg, 0);
        if (res == VPX_CODEC_OK) {
            int ctrl_id;
            int val;
            
            /* Use first byte to select a control ID */
            switch (Data[0] % 5) {
                case 0:
                    ctrl_id = VP8E_SET_TUNING;
                    val = (Size > 1 ? Data[1] % 10 : 0);
                    res = vpx_codec_control_(&ctx, ctrl_id, val);
                    break;
                case 1:
                    ctrl_id = VP8E_SET_STATIC_THRESHOLD;
                    val = (Size > 1 ? Data[1] % 256 : 0);
                    res = vpx_codec_control_(&ctx, ctrl_id, val);
                    break;
                case 2:
                    ctrl_id = VP8E_SET_NOISE_SENSITIVITY;
                    val = (Size > 1 ? Data[1] % 32 : 0);
                    res = vpx_codec_control_(&ctx, ctrl_id, val);
                    break;
                case 3:
                    ctrl_id = VP8E_SET_TOKEN_PARTITIONS;
                    val = (Size > 1 ? Data[1] % 4 : 0);
                    res = vpx_codec_control_(&ctx, ctrl_id, val);
                    break;
                case 4:
                    ctrl_id = VP8E_SET_TUNING;
                    val = (Size > 1 ? Data[1] % 10 : 0);
                    res = vpx_codec_control_(&ctx, ctrl_id, val);
                    break;
                default:
                    ctrl_id = VP8E_SET_TUNING;
                    val = 0;
                    res = vpx_codec_control_(&ctx, ctrl_id, val);
            }

            /* Cleanup encoder context */
            vpx_codec_destroy(&ctx);
        }
    }

    /* Try VP8 Decoder Interface */
    iface = vpx_codec_vp8_dx();
    if (iface) {
        vpx_codec_dec_cfg_t cfg;
        memset(&cfg, 0, sizeof(cfg));
        cfg.threads = 1;
        
        res = vpx_codec_dec_init(&ctx, iface, &cfg, 0);
        if (res == VPX_CODEC_OK) {
            int ctrl_id;
            
            switch (Size > 2 ? Data[2] % 3 : 0) {
                case 0:
                    ctrl_id = 99999;
                    res = vpx_codec_control_(&ctx, ctrl_id);
                    break;
                case 1:
                    ctrl_id = 0;
                    res = vpx_codec_control_(&ctx, ctrl_id);
                    break;
                case 2:
                    ctrl_id = 10000; 
                    res = vpx_codec_control_(&ctx, ctrl_id);
                    break;
            }

            vpx_codec_destroy(&ctx);
        }
    }

    /* Cleanup */
    cleanup_dummy_file();

    return 0;
}