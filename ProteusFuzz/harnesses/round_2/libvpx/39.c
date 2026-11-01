// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
// vpx_codec_enc_config_set at vpx_encoder.c:348:17 in vpx_encoder.h
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>

static void fuzz_config_from_data(vpx_codec_enc_cfg_t *cfg, const uint8_t *data, size_t size) {
    size_t idx = 0;
    
    // Helper to read safe integers
    #define READ_UINT32(val) \
        (idx + 4 <= size) ? (val = *(uint32_t *)(data + idx), idx += 4, val) : 0
    
    #define READ_INT(val) \
        (idx + 4 <= size) ? (val = *(int32_t *)(data + idx), idx += 4, val) : 0

    cfg->g_usage = READ_UINT32(cfg->g_usage);
    cfg->g_threads = READ_UINT32(cfg->g_threads);
    cfg->g_profile = READ_UINT32(cfg->g_profile);
    cfg->g_w = READ_UINT32(cfg->g_w);
    cfg->g_h = READ_UINT32(cfg->g_h);
    
    // Constrain dimensions to reasonable fuzzing limits to avoid OOM
    if (cfg->g_w > 4096) cfg->g_w = 4096;
    if (cfg->g_h > 4096) cfg->g_h = 4096;
    if (cfg->g_w == 0) cfg->g_w = 16;
    if (cfg->g_h == 0) cfg->g_h = 16;

    cfg->g_bit_depth = (vpx_bit_depth_t)READ_UINT32(cfg->g_bit_depth);
    cfg->g_input_bit_depth = READ_UINT32(cfg->g_input_bit_depth);
    
    // Timebase
    if (idx + 8 <= size) {
        cfg->g_timebase.num = *(int32_t *)(data + idx); idx += 4;
        cfg->g_timebase.den = *(int32_t *)(data + idx); idx += 4;
        if (cfg->g_timebase.den == 0) cfg->g_timebase.den = 1;
    } else {
        cfg->g_timebase.num = 1;
        cfg->g_timebase.den = 1;
    }

    cfg->g_error_resilient = READ_UINT32(cfg->g_error_resilient);
    cfg->g_pass = (vpx_enc_pass)READ_UINT32(cfg->g_pass);
    cfg->g_lag_in_frames = READ_UINT32(cfg->g_lag_in_frames);

    cfg->rc_dropframe_thresh = READ_UINT32(cfg->rc_dropframe_thresh);
    cfg->rc_resize_allowed = READ_UINT32(cfg->rc_resize_allowed);
    cfg->rc_scaled_width = READ_UINT32(cfg->rc_scaled_width);
    cfg->rc_scaled_height = READ_UINT32(cfg->rc_scaled_height);
    cfg->rc_resize_up_thresh = READ_UINT32(cfg->rc_resize_up_thresh);
    cfg->rc_resize_down_thresh = READ_UINT32(cfg->rc_resize_down_thresh);
    
    // Cast to enum explicitly to ensure type safety
    unsigned int rc_end_usage_val = READ_UINT32(cfg->rc_end_usage);
    cfg->rc_end_usage = (enum vpx_rc_mode)rc_end_usage_val;
    
    cfg->rc_target_bitrate = READ_UINT32(cfg->rc_target_bitrate);
    cfg->rc_min_quantizer = READ_UINT32(cfg->rc_min_quantizer);
    cfg->rc_max_quantizer = READ_UINT32(cfg->rc_max_quantizer);
    cfg->rc_undershoot_pct = READ_UINT32(cfg->rc_undershoot_pct);
    cfg->rc_overshoot_pct = READ_UINT32(cfg->rc_overshoot_pct);
    cfg->rc_buf_sz = READ_UINT32(cfg->rc_buf_sz);
    cfg->rc_buf_initial_sz = READ_UINT32(cfg->rc_buf_initial_sz);
    cfg->rc_buf_optimal_sz = READ_UINT32(cfg->rc_buf_optimal_sz);

    // Cast to enum explicitly
    unsigned int kf_mode_val = READ_UINT32(cfg->kf_mode);
    cfg->kf_mode = (enum vpx_kf_mode)kf_mode_val;
    
    cfg->kf_min_dist = READ_UINT32(cfg->kf_min_dist);
    cfg->kf_max_dist = READ_UINT32(cfg->kf_max_dist);

    // Skip complex arrays to keep it simple but varied
    cfg->ss_number_layers = READ_UINT32(cfg->ss_number_layers);
    cfg->ts_number_layers = READ_UINT32(cfg->ts_number_layers);
}

int LLVMFuzzerTestOneInput_39(const uint8_t *Data, size_t Size) {
    if (Size < 12) return 0;

    vpx_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    
    vpx_codec_enc_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));

    // Initialize config with fuzzed data
    fuzz_config_from_data(&cfg, Data, Size);

    // Get encoder interface
    vpx_codec_iface_t *iface = vpx_codec_vp8_cx();

    if (iface == NULL) return 0;

    // 1. vpx_codec_enc_config_default
    cfg.g_usage = 0; 
    vpx_codec_err_t ret = vpx_codec_enc_config_default(iface, &cfg, 0);
    if (ret != VPX_CODEC_OK) {
        return 0;
    }

    // 2. vpx_codec_enc_init_ver
    vpx_codec_flags_t flags = (Data[1] & 0xFF);
    ret = vpx_codec_enc_init_ver(&ctx, iface, &cfg, flags, VPX_ENCODER_ABI_VERSION);
    
    if (ret != VPX_CODEC_OK) {
        const char *err_detail = vpx_codec_error_detail(&ctx);
        if (err_detail) {
            (void)err_detail;
        }
        return 0;
    }

    // 3. vpx_codec_enc_config_set
    vpx_codec_enc_cfg_t cfg_mod = cfg;
    cfg_mod.g_threads = (Data[2] & 0xFF) + 1;
    cfg_mod.rc_target_bitrate = (Data[3] << 8) | Data[4];
    
    ret = vpx_codec_enc_config_set(&ctx, &cfg_mod);
    if (ret != VPX_CODEC_OK) {
        const char *err = vpx_codec_error_detail(&ctx);
        (void)err;
    }

    // 4. vpx_codec_control_
    int ctrl_id = (int)(Data[5] & 0xFF);
    ret = vpx_codec_control_(&ctx, ctrl_id, (int)(Data[6] & 0xFF));
    
    if (ret != VPX_CODEC_OK) {
        const char *err = vpx_codec_error_detail(&ctx);
        (void)err;
    }

    // 5. vpx_codec_encode
    vpx_image_t img;
    memset(&img, 0, sizeof(img));
    img.fmt = VPX_IMG_FMT_I420;
    img.w = cfg.g_w;
    img.h = cfg.g_h;
    img.d_w = cfg.g_w;
    img.d_h = cfg.g_h;
    img.bit_depth = cfg.g_bit_depth;
    img.cs = VPX_CS_BT_601;
    img.range = VPX_CR_STUDIO_RANGE;
    
    size_t plane_size = img.w * img.h;
    unsigned char *y_plane = malloc(plane_size);
    unsigned char *u_plane = malloc(plane_size / 4);
    unsigned char *v_plane = malloc(plane_size / 4);
    
    if (y_plane && u_plane && v_plane) {
        img.planes[0] = y_plane;
        img.planes[1] = u_plane;
        img.planes[2] = v_plane;
        img.planes[3] = NULL;
        img.stride[0] = img.w;
        img.stride[1] = img.w / 2;
        img.stride[2] = img.w / 2;
        img.stride[3] = 0;
        
        size_t fill_size = (Size > plane_size) ? plane_size : Size;
        memcpy(y_plane, Data, fill_size);
        if (fill_size < plane_size) memset(y_plane + fill_size, 0, plane_size - fill_size);
        
        vpx_codec_pts_t pts = (int64_t)(Data[7] & 0xFF);
        unsigned long duration = (Data[8] & 0xFF) + 1;
        vpx_enc_frame_flags_t eflags = (Data[9] & 0xFF);
        vpx_enc_deadline_t deadline = (Data[10] << 8) | Data[11];
        
        ret = vpx_codec_encode(&ctx, &img, pts, duration, eflags, deadline);
        
        if (ret != VPX_CODEC_OK) {
            const char *err = vpx_codec_error_detail(&ctx);
            (void)err;
        }

        ret = vpx_codec_encode(&ctx, NULL, pts + 1, duration, 0, 0);
        if (ret != VPX_CODEC_OK) {
            const char *err = vpx_codec_error_detail(&ctx);
            (void)err;
        }

        free(y_plane);
        free(u_plane);
        free(v_plane);
    }

    vpx_codec_destroy(&ctx);

    return 0;
}