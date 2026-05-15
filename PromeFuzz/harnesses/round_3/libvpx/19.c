// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_get_global_headers at vpx_encoder.c:331:18 in vpx_encoder.h
// vpx_codec_enc_config_set at vpx_encoder.c:348:17 in vpx_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_codec.h"
#include "vpx/vp8cx.h"
#include "vpx/vpx_encoder.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void initialize_config(vpx_codec_enc_cfg_t *cfg, const uint8_t *data, size_t size) {
    // Use default configuration for VP9
    vpx_codec_iface_t *iface = vpx_codec_vp9_cx();
    if (iface == NULL) return;
    
    vpx_codec_err_t res = vpx_codec_enc_config_default(iface, cfg, 0);
    if (res != VPX_CODEC_OK) return;
    
    // Modify some parameters with fuzzer data to explore different states
    if (size > 0) {
        cfg->g_w = 64 + (data[0] % 192);  // Width between 64 and 255
        cfg->g_h = 64 + (data[1] % 192);  // Height between 64 and 255
        cfg->rc_target_bitrate = 100 + (data[2] % 900);  // Bitrate between 100 and 999
        cfg->g_error_resilient = data[3] & 1;
        cfg->g_pass = (data[4] % 3 == 0) ? VPX_RC_ONE_PASS : 
                     (data[4] % 3 == 1) ? VPX_RC_FIRST_PASS : VPX_RC_LAST_PASS;
        cfg->rc_end_usage = (data[5] % 4 == 0) ? VPX_VBR : 
                           (data[5] % 4 == 1) ? VPX_CBR : 
                           (data[5] % 4 == 2) ? VPX_CQ : VPX_Q;
        cfg->kf_mode = (data[6] % 3 == 0) ? VPX_KF_FIXED : 
                      (data[6] % 3 == 1) ? VPX_KF_AUTO : VPX_KF_DISABLED;
        cfg->kf_min_dist = data[7];
        cfg->kf_max_dist = 100 + (data[8] % 155);
    }
}

static void fuzz_control_functions(vpx_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
    if (size < 10) return;
    
    // Try different control IDs with fuzzer data using vpx_codec_control_ directly
    int ctrl_id = data[0] % 256;  // Various control IDs
    int int_val = *(int*)&data[1];
    
    // Use vpx_codec_control_ directly instead of the macro
    vpx_codec_control_(ctx, ctrl_id, int_val);
    
    // Try a few more control calls with different parameters
    if (size > 20) {
        // For known control IDs, we need to use the correct type
        int cpu_used = data[2] % 9;
        int tile_columns = data[3] % 6;
        int enable_auto_altref = data[4] & 1;
        int row_mt = data[5] & 1;
        
        vpx_codec_control_(ctx, VP8E_SET_CPUUSED, cpu_used);
        vpx_codec_control_(ctx, VP9E_SET_TILE_COLUMNS, tile_columns);
        vpx_codec_control_(ctx, VP8E_SET_ENABLEAUTOALTREF, enable_auto_altref);
        vpx_codec_control_(ctx, VP9E_SET_ROW_MT, row_mt);
    }
}

int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    
    // Initialize encoder configuration
    initialize_config(&cfg, Data, Size);
    
    // Initialize encoder context
    vpx_codec_err_t res = vpx_codec_enc_init_ver(&ctx, vpx_codec_vp9_cx(), &cfg, 0, 
                                                 VPX_ENCODER_ABI_VERSION);
    
    if (res != VPX_CODEC_OK) {
        // Cleanup not required on init failure
        return 0;
    }
    
    // Test vpx_codec_get_global_headers
    vpx_fixed_buf_t *global_headers = vpx_codec_get_global_headers(&ctx);
    // We don't need to use the result, just calling it exercises the code
    
    // Test vpx_codec_enc_config_set with modified configuration
    if (Size > 30) {
        // Modify some configuration parameters
        cfg.rc_min_quantizer = Data[9] % 64;
        cfg.rc_max_quantizer = 10 + (Data[10] % 54);
        cfg.g_threads = 1 + (Data[11] % 4);
        
        vpx_codec_enc_config_set(&ctx, &cfg);
    }
    
    // Test vpx_codec_control_ function
    fuzz_control_functions(&ctx, Data, Size);
    
    // Test vpx_codec_get_cx_data iterator
    vpx_codec_iter_t iter = NULL;
    const vpx_codec_cx_pkt_t *pkt;
    while ((pkt = vpx_codec_get_cx_data(&ctx, &iter)) != NULL) {
        // Just iterate through, don't need to process packets
    }
    
    // Cleanup
    vpx_codec_destroy(&ctx);
    
    return 0;
}