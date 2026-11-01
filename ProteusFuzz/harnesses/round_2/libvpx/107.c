// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp9_dx at vp9_dx_iface.c:712:1 in vp8dx.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>
#include <vpx/vpx_decoder.h>
#include <vpx/vpx_encoder.h>

static int init_decoder(vpx_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    vpx_codec_iface_t *iface = vpx_codec_vp9_dx();
    if (!iface) return -1;

    vpx_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    // Use correct member names for vpx_codec_dec_cfg_t
    cfg.w = 128;
    cfg.h = 128;
    cfg.threads = 1;

    if (vpx_codec_dec_init(ctx, iface, &cfg, 0) != VPX_CODEC_OK) {
        return -1;
    }
    return 0;
}

static int init_encoder(vpx_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    vpx_codec_iface_t *iface = vpx_codec_vp9_cx();
    if (!iface) return -1;

    vpx_codec_enc_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    if (vpx_codec_enc_config_default(iface, &cfg, 0) != VPX_CODEC_OK) {
        return -1;
    }

    cfg.g_w = 128;
    cfg.g_h = 128;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 500;
    cfg.rc_min_quantizer = 0;
    cfg.rc_max_quantizer = 63;
    cfg.g_threads = 1;

    if (vpx_codec_enc_init(ctx, iface, &cfg, 0) != VPX_CODEC_OK) {
        return -1;
    }
    return 0;
}

int LLVMFuzzerTestOneInput_107(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Use first byte to decide between encoder and decoder context
    int is_encoder = (Data[0] % 2);
    vpx_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    int init_result = 0;
    if (is_encoder) {
        init_result = init_encoder(&ctx, Data, Size);
    } else {
        init_result = init_decoder(&ctx, Data, Size);
    }

    if (init_result != 0) {
        return 0;
    }

    // Extract control ID and value from fuzz input
    // We'll try a few control IDs relevant to the task description
    int ctrl_ids[] = {
        VP9E_SET_TUNE_CONTENT,
        VP9E_SET_DISABLE_LOOPFILTER,
        VP9E_GET_LOOPFILTER_LEVEL,
    };
    int num_ids = sizeof(ctrl_ids) / sizeof(ctrl_ids[0]);

    // Iterate through available control IDs
    for (int i = 0; i < num_ids; i++) {
        int ctrl_id = ctrl_ids[i];
        
        // Use remaining data to form a value if needed
        int value = 0;
        if (Size > 1) {
            value = (Data[1] % 256);
        }

        // For GET operations, we might need a pointer to store result
        int *result_ptr = NULL;
        int is_get = (ctrl_id == VP9E_GET_LOOPFILTER_LEVEL);
        
        if (is_get) {
            result_ptr = &value;
            // Use vpx_codec_control_ directly to avoid macro expansion issues with variable IDs
            vpx_codec_err_t err = vpx_codec_control_(&ctx, ctrl_id, result_ptr);
            (void)err; // Ignore error for fuzzing, just ensure no crash
        } else {
            // For SET operations, we pass the value directly
            vpx_codec_err_t err = vpx_codec_control_(&ctx, ctrl_id, value);
            (void)err;
        }
    }

    // Also try to pass raw data as value for some controls if they accept it
    // This is speculative but helps explore state space
    if (Size > 4) {
        int raw_val = 0;
        memcpy(&raw_val, Data + 1, sizeof(int));
        vpx_codec_err_t err = vpx_codec_control_(&ctx, VP9E_SET_TUNE_CONTENT, raw_val);
        (void)err;
    }

    // Cleanup
    if (is_encoder) {
        vpx_codec_destroy(&ctx);
    } else {
        vpx_codec_destroy(&ctx);
    }

    return 0;
}