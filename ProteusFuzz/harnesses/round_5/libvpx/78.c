// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>

/* 
 * Fuzz driver for libvpx encoder initialization and control APIs.
 * Targets: vpx_codec_enc_init_ver, vpx_codec_control_
 * Note: VP8E_SET_*, VP9E_SET_* are control IDs (enums), not functions.
 */

static int fuzz_encode(const uint8_t *data, size_t size) {
    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_err_t res;
    vpx_codec_iface_t *iface;
    int ctrl_id;
    int arg_val;
    int ret = 0;

    /* Initialize configuration with safe defaults */
    /* Try VP8 first for default config */
    if (vpx_codec_enc_config_default(vpx_codec_vp8_cx(), &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    /* Fuzz configuration parameters based on input data */
    /* Ensure we have enough data before accessing indices */
    if (size >= 13) {
        cfg.g_w = (data[0] % 128) + 16;
        cfg.g_h = (data[1] % 128) + 16;
        cfg.g_threads = (data[2] % 8) + 1;
        cfg.rc_target_bitrate = (data[3] % 1000) + 100;
        cfg.g_bit_depth = (data[4] % 3) + 8; /* 8, 10, or 12 */
        cfg.g_input_bit_depth = cfg.g_bit_depth;
        cfg.rc_end_usage = (data[5] % 4); /* VBR, CBR, CQ, Q */
        cfg.kf_mode = (data[6] % 3);
        cfg.ts_number_layers = (data[7] % 4) + 1;
        cfg.g_timebase.num = (data[8] % 100) + 1;
        cfg.g_timebase.den = (data[9] % 1000) + 1;
        ctrl_id = data[10] % 100;
        arg_val = (int)(data[11] * 256 + data[12]) % 1000 - 500;
    } else {
        /* Default values if data is too short */
        ctrl_id = 0;
        arg_val = 0;
    }

    /* Try VP8 and VP9 interfaces */
    const vpx_codec_iface_t *interfaces[] = {
        vpx_codec_vp8_cx(),
        vpx_codec_vp9_cx(),
        NULL
    };

    for (int i = 0; interfaces[i] != NULL; i++) {
        iface = interfaces[i];

        /* Initialize encoder context */
        memset(&ctx, 0, sizeof(ctx));
        res = vpx_codec_enc_init_ver(&ctx, iface, &cfg, 0, VPX_ENCODER_ABI_VERSION);
        if (res != VPX_CODEC_OK) {
            continue;
        }

        /* Fuzz control commands */
        /* Map fuzzed ctrl_id to known valid control IDs for better coverage */
        int known_ids[] = {
            VP8E_SET_CPUUSED,
            VP8E_SET_TUNING,
            VP8E_SET_TEMPORAL_LAYER_ID,
            VP9E_SET_TUNE_CONTENT,
            -1
        };

        /* Try known IDs */
        for (int k = 0; known_ids[k] != -1; k++) {
            res = vpx_codec_control_(&ctx, known_ids[k], arg_val);
            if (res == VPX_CODEC_OK) {
                ret = 1;
            }
        }

        /* Try raw fuzzed ID if it's within a reasonable range */
        if (ctrl_id >= 0 && ctrl_id < 1000) {
            res = vpx_codec_control_(&ctx, ctrl_id, arg_val);
            /* We don't care about the return value here, just execution */
        }

        /* Cleanup */
        vpx_codec_destroy(&ctx);
    }

    return ret;
}

int LLVMFuzzerTestOneInput_78(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    /* Basic sanity check */
    if (Size < 2) return 0;

    return fuzz_encode(Data, Size);
}