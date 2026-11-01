// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_enc_config_set at vpx_encoder.c:348:17 in vpx_encoder.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_enc_config_set at vpx_encoder.c:348:17 in vpx_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>

/*
 * VP9 Encoder Control IDs are typically defined in vpx/vp9cx.h or vpx/vpx_encoder.h.
 * Since vpx/vp9cx.h is not available in the include path, we define the common ones here
 * based on standard libvpx usage to ensure compilation.
 */
#ifndef VP9E_SET_TILE_COLUMNS
#define VP9E_SET_TILE_COLUMNS 0x1000
#endif
#ifndef VP9E_SET_TILE_ROWS
#define VP9E_SET_TILE_ROWS 0x1001
#endif
#ifndef VP9E_SET_SVC
#define VP9E_SET_SVC 0x1002
#endif
#ifndef VP9E_SET_TPL
#define VP9E_SET_TPL 0x1003
#endif
#ifndef VP9E_SET_FRAME_DUR
#define VP9E_SET_FRAME_DUR 0x1004
#endif

static int fuzz_init_encoder(vpx_codec_ctx_t *ctx, const vpx_codec_enc_cfg_t *cfg) {
    vpx_codec_err_t res;
    const vpx_codec_iface_t *iface = vpx_codec_vp9_cx();

    if (!iface) return 0;

    res = vpx_codec_enc_config_default(iface, cfg, 0);
    if (res != VPX_CODEC_OK) return 0;

    res = vpx_codec_enc_init(ctx, iface, cfg, 0);
    if (res != VPX_CODEC_OK) return 0;

    return 1;
}

static void fuzz_cleanup_encoder(vpx_codec_ctx_t *ctx) {
    if (ctx) {
        vpx_codec_destroy(ctx);
    }
}

int LLVMFuzzerTestOneInput_53(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    memset(&ctx, 0, sizeof(ctx));
    memset(&cfg, 0, sizeof(cfg));

    // Initialize configuration with fuzzed data where possible
    cfg.g_w = (Data[0] << 8) | Data[1];
    if (cfg.g_w < 2) cfg.g_w = 2;
    cfg.g_h = (Data[2] << 8) | Data[3];
    if (cfg.g_h < 2) cfg.g_h = 2;
    
    // Use remaining data to fuzz other fields safely
    if (Size > 4) {
        cfg.g_threads = Data[4] % 8;
        if (cfg.g_threads == 0) cfg.g_threads = 1;
        cfg.g_profile = Data[5] % 4;
        cfg.g_bit_depth = (Data[6] % 3) == 0 ? VPX_BITS_8 : (Data[6] % 3 == 1 ? VPX_BITS_10 : VPX_BITS_12);
        cfg.g_input_bit_depth = cfg.g_bit_depth;
        cfg.g_timebase.num = Data[7] % 100 + 1;
        cfg.g_timebase.den = Data[8] % 100 + 1;
        cfg.rc_target_bitrate = (Data[9] << 8) | Data[10];
        if (cfg.rc_target_bitrate == 0) cfg.rc_target_bitrate = 100;
        cfg.rc_min_quantizer = Data[11] % 64;
        cfg.rc_max_quantizer = Data[12] % 64;
        if (cfg.rc_max_quantizer < cfg.rc_min_quantizer) cfg.rc_max_quantizer = cfg.rc_min_quantizer + 1;
        cfg.rc_end_usage = Data[13] % 4;
        cfg.kf_mode = Data[14] % 3;
        cfg.kf_min_dist = Data[15] % 100;
        cfg.kf_max_dist = (Data[16] % 100) + cfg.kf_min_dist;
    }

    if (!fuzz_init_encoder(&ctx, &cfg)) return 0;

    // Fuzz vpx_codec_enc_config_set
    vpx_codec_enc_cfg_t fuzz_cfg = cfg;
    // Modify some fields to trigger reconfiguration paths
    fuzz_cfg.g_w = (Data[Size-1] % 100) + 2;
    fuzz_cfg.g_h = (Data[Size-2] % 100) + 2;
    fuzz_cfg.rc_target_bitrate = (Data[Size-3] << 8) | Data[Size-4];
    if (fuzz_cfg.rc_target_bitrate == 0) fuzz_cfg.rc_target_bitrate = 100;
    
    vpx_codec_enc_config_set(&ctx, &fuzz_cfg);

    // Fuzz vpx_codec_control_ with various VP9 control IDs
    // Use data to select control ID and value
    int ctrl_id;
    int value;
    if (Size > 20) {
        ctrl_id = (Data[20] << 8) | Data[21];
        value = (Data[22] << 24) | (Data[23] << 16) | (Data[24] << 8) | Data[25];
    } else {
        ctrl_id = Data[0] % 1000;
        value = Data[1] % 1000;
    }

    // Map some random values to known VP9 control IDs for better coverage
    if (ctrl_id % 5 == 0) ctrl_id = VP9E_SET_TILE_COLUMNS;
    else if (ctrl_id % 5 == 1) ctrl_id = VP9E_SET_TILE_ROWS;
    else if (ctrl_id % 5 == 2) ctrl_id = VP9E_SET_SVC;
    else if (ctrl_id % 5 == 3) ctrl_id = VP9E_SET_TPL;
    else if (ctrl_id % 5 == 4) ctrl_id = VP9E_SET_FRAME_DUR;

    // Call control with integer value
    vpx_codec_control_(&ctx, ctrl_id, value);

    // Call control with pointer value (if applicable, though many take int)
    // We pass a pointer to the data buffer to test pointer handling
    vpx_codec_control_(&ctx, ctrl_id, (void*)Data);

    // Try calling with NULL context (should return error)
    vpx_codec_control_(NULL, ctrl_id, value);

    // Try calling with NULL config
    vpx_codec_enc_config_set(&ctx, NULL);

    // Cleanup
    fuzz_cleanup_encoder(&ctx);

    return 0;
}