// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_encoder.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>
#include <vpx/vpx_image.h>

// Helper to safely free encoder resources
static void cleanup_encoder(vpx_codec_ctx_t *ctx) {
    if (ctx) {
        vpx_codec_destroy(ctx);
        free(ctx);
    }
}

// Helper to initialize encoder for testing SVC controls
static vpx_codec_ctx_t *init_encoder(const uint8_t *data, size_t size) {
    vpx_codec_iface_t *codec = vpx_codec_vp9_cx();
    if (!codec) return NULL;

    vpx_codec_ctx_t *ctx = (vpx_codec_ctx_t *)malloc(sizeof(vpx_codec_ctx_t));
    if (!ctx) return NULL;

    vpx_codec_enc_cfg_t cfg;
    if (vpx_codec_enc_config_default(codec, &cfg, 0) != VPX_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    // Configure for basic encoding
    cfg.g_w = 160;
    cfg.g_h = 120;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;
    cfg.kf_mode = VPX_KF_AUTO;
    cfg.kf_max_dist = 120;
    cfg.kf_min_dist = 0;
    cfg.g_lag_in_frames = 0;
    cfg.g_error_resilient = 1;
    cfg.g_threads = 1;
    cfg.g_pass = VPX_RC_ONE_PASS;
    cfg.g_profile = 0;

    if (vpx_codec_enc_init(ctx, codec, &cfg, 0) != VPX_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    return ctx;
}

int LLVMFuzzerTestOneInput_86(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    vpx_codec_ctx_t *ctx = init_encoder(Data, Size);
    if (!ctx) return 0;

    // 1. Fuzz VP9E_SET_SVC_REF_FRAME_CONFIG
    if (Size >= sizeof(vpx_svc_ref_frame_config_t)) {
        vpx_svc_ref_frame_config_t cfg;
        memcpy(&cfg, Data, sizeof(cfg));
        
        // Sanitize values
        for (int i = 0; i < VPX_SS_MAX_LAYERS; i++) {
            if (cfg.lst_fb_idx[i] < 0 || cfg.lst_fb_idx[i] > 127) cfg.lst_fb_idx[i] = 0;
            if (cfg.gld_fb_idx[i] < 0 || cfg.gld_fb_idx[i] > 127) cfg.gld_fb_idx[i] = 0;
            if (cfg.alt_fb_idx[i] < 0 || cfg.alt_fb_idx[i] > 127) cfg.alt_fb_idx[i] = 0;
            cfg.update_buffer_slot[i] = (cfg.update_buffer_slot[i] != 0) ? 1 : 0;
            cfg.update_last[i] = (cfg.update_last[i] != 0) ? 1 : 0;
            cfg.update_golden[i] = (cfg.update_golden[i] != 0) ? 1 : 0;
            cfg.update_alt_ref[i] = (cfg.update_alt_ref[i] != 0) ? 1 : 0;
            cfg.reference_last[i] = (cfg.reference_last[i] != 0) ? 1 : 0;
            cfg.reference_golden[i] = (cfg.reference_golden[i] != 0) ? 1 : 0;
            cfg.reference_alt_ref[i] = (cfg.reference_alt_ref[i] != 0) ? 1 : 0;
            if (cfg.duration[i] < 0) cfg.duration[i] = 0;
            if (cfg.duration[i] > 1000000) cfg.duration[i] = 1000000;
        }
        vpx_codec_control_(ctx, VP9E_SET_SVC_REF_FRAME_CONFIG, &cfg);
    }

    // 2. Fuzz VP9E_SET_SVC_FRAME_DROP_LAYER
    if (Size >= sizeof(vpx_svc_frame_drop_t)) {
        vpx_svc_frame_drop_t cfg;
        memcpy(&cfg, Data, sizeof(cfg));
        
        for (int i = 0; i < VPX_SS_MAX_LAYERS; i++) {
            if (cfg.framedrop_thresh[i] < 0) cfg.framedrop_thresh[i] = 0;
            if (cfg.framedrop_thresh[i] > 1000) cfg.framedrop_thresh[i] = 1000;
        }
        cfg.framedrop_mode = (cfg.framedrop_mode > 1) ? 0 : cfg.framedrop_mode;
        if (cfg.max_consec_drop < 0) cfg.max_consec_drop = 0;
        if (cfg.max_consec_drop > 100) cfg.max_consec_drop = 100;
        
        vpx_codec_control_(ctx, VP9E_SET_SVC_FRAME_DROP_LAYER, &cfg);
    }

    // 3. Fuzz VP9E_SET_SVC_LAYER_ID
    if (Size >= sizeof(vpx_svc_layer_id_t)) {
        vpx_svc_layer_id_t cfg;
        memcpy(&cfg, Data, sizeof(cfg));
        
        if (cfg.spatial_layer_id < 0) cfg.spatial_layer_id = 0;
        if (cfg.spatial_layer_id >= VPX_SS_MAX_LAYERS) cfg.spatial_layer_id = VPX_SS_MAX_LAYERS - 1;
        
        if (cfg.temporal_layer_id < 0) cfg.temporal_layer_id = 0;
        if (cfg.temporal_layer_id > 3) cfg.temporal_layer_id = 3;

        for (int i = 0; i < VPX_SS_MAX_LAYERS; i++) {
            if (cfg.temporal_layer_id_per_spatial[i] < 0) cfg.temporal_layer_id_per_spatial[i] = 0;
            if (cfg.temporal_layer_id_per_spatial[i] > 3) cfg.temporal_layer_id_per_spatial[i] = 3;
        }

        vpx_codec_control_(ctx, VP9E_SET_SVC_LAYER_ID, &cfg);
    }

    // 4. Fuzz VP9E_SET_SVC_SPATIAL_LAYER_SYNC
    if (Size >= sizeof(vpx_svc_spatial_layer_sync_t)) {
        vpx_svc_spatial_layer_sync_t cfg;
        memcpy(&cfg, Data, sizeof(cfg));
        
        for (int i = 0; i < VPX_SS_MAX_LAYERS; i++) {
            cfg.spatial_layer_sync[i] = (cfg.spatial_layer_sync[i] != 0) ? 1 : 0;
        }
        cfg.base_layer_intra_only = (cfg.base_layer_intra_only != 0) ? 1 : 0;

        vpx_codec_control_(ctx, VP9E_SET_SVC_SPATIAL_LAYER_SYNC, &cfg);
    }

    // 5. Fuzz VP9E_SET_SVC (General SVC enable/disable or config)
    if (Size >= sizeof(int)) {
        int svc_flag = *(const int *)Data;
        svc_flag = (svc_flag != 0) ? 1 : 0;
        vpx_codec_control_(ctx, VP9E_SET_SVC, &svc_flag);
    }

    // 6. VP9E_SET_SVC_PARAMETERS
    // Skipping due to missing struct definition in the prompt and potential undefined behavior.

    // Cleanup
    cleanup_encoder(ctx);

    return 0;
}