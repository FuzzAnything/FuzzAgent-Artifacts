// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <vpx/vpx_encoder.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

static vpx_codec_enc_cfg_t *init_config(const uint8_t *data, size_t size, vpx_codec_enc_cfg_t *cfg) {
    if (!cfg) return NULL;
    memset(cfg, 0, sizeof(vpx_codec_enc_cfg_t));
    
    cfg->g_w = (size > 0) ? (data[0] % 256) + 16 : 16;
    cfg->g_h = (size > 1) ? (data[1] % 256) + 16 : 16;
    cfg->g_threads = (size > 2) ? (data[2] % 8) + 1 : 1;
    cfg->g_profile = (size > 3) ? (data[3] % 4) : 0;
    cfg->g_bit_depth = VPX_BITS_8;
    cfg->g_input_bit_depth = 8;
    
    cfg->g_timebase.num = (size > 4) ? (data[4] % 100) + 1 : 1;
    cfg->g_timebase.den = (size > 5) ? (data[5] % 100) + 1 : 1;
    
    cfg->rc_end_usage = (size > 6) ? (data[6] % 4) : VPX_VBR;
    cfg->rc_target_bitrate = (size > 7) ? (data[7] % 1000) + 10 : 10;
    
    cfg->rc_min_quantizer = (size > 8) ? (data[8] % 63) : 0;
    cfg->rc_max_quantizer = (size > 9) ? (data[9] % 63) + 10 : 10;
    
    cfg->kf_mode = (size > 10) ? (data[10] % 3) : VPX_KF_DISABLED;
    cfg->kf_min_dist = (size > 11) ? (data[11] % 100) + 1 : 1;
    cfg->kf_max_dist = (size > 12) ? (data[12] % 100) + 1 : 1;
    
    cfg->rc_buf_sz = (size > 13) ? (data[13] % 10000) + 100 : 100;
    cfg->rc_buf_initial_sz = (size > 14) ? (data[14] % 10000) + 100 : 100;
    cfg->rc_buf_optimal_sz = (size > 15) ? (data[15] % 10000) + 100 : 100;
    
    cfg->rc_undershoot_pct = (size > 16) ? (data[16] % 101) : 50;
    cfg->rc_overshoot_pct = (size > 17) ? (data[17] % 101) : 50;
    
    return cfg;
}

int LLVMFuzzerTestOneInput_48(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    vpx_codec_ctx_t ctx;
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_iface_t *iface = NULL;
    vpx_codec_err_t res;

    // Try VP8 encoder
    iface = vpx_codec_vp8_cx();
    if (iface) {
        init_config(Data, Size, &cfg);
        res = vpx_codec_enc_init_ver(&ctx, iface, &cfg, 0, VPX_ENCODER_ABI_VERSION);
        if (res == VPX_CODEC_OK) {
            // Try various control operations
            int cpu_used = (Data[0] % 9);
            vpx_codec_control_(&ctx, VP8E_SET_CPUUSED, cpu_used);
            
            int tuning = (Data[1] % 3);
            vpx_codec_control_(&ctx, VP8E_SET_TUNING, tuning);
            
            // Try to encode a dummy frame
            vpx_image_t img;
            vpx_img_alloc(&img, VPX_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
            memset(img.planes[0], 0, img.stride[0] * cfg.g_h);
            memset(img.planes[1], 128, img.stride[1] * (cfg.g_h / 2));
            memset(img.planes[2], 128, img.stride[2] * (cfg.g_h / 2));
            
            vpx_codec_encode(&ctx, &img, 0, 1, 0, 0);
            
            vpx_img_free(&img);
            vpx_codec_destroy(&ctx);
        }
    }

    return 0;
}