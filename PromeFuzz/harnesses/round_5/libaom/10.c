// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_enc_config_set at aom_encoder.c:298:17 in aom_encoder.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "aom/aom_codec.h"
#include "aom/aom_encoder.h"
#include "aom/aomcx.h"

static aom_codec_err_t fuzz_aom_codec_encode(aom_codec_ctx_t *ctx, const aom_image_t *img,
                                             aom_codec_pts_t pts, unsigned long duration,
                                             aom_enc_frame_flags_t flags) {
    return aom_codec_encode(ctx, img, pts, duration, flags);
}

static aom_codec_err_t fuzz_aom_codec_enc_config_default(aom_codec_iface_t *iface,
                                                         aom_codec_enc_cfg_t *cfg,
                                                         unsigned int usage) {
    return aom_codec_enc_config_default(iface, cfg, usage);
}

static aom_codec_err_t fuzz_aom_codec_enc_config_set(aom_codec_ctx_t *ctx,
                                                     const aom_codec_enc_cfg_t *cfg) {
    return aom_codec_enc_config_set(ctx, cfg);
}

static aom_codec_err_t fuzz_aom_codec_set_option(aom_codec_ctx_t *ctx, const char *name,
                                                 const char *value) {
    return aom_codec_set_option(ctx, name, value);
}

static aom_codec_err_t fuzz_aom_codec_enc_init_ver(aom_codec_ctx_t *ctx,
                                                   aom_codec_iface_t *iface,
                                                   const aom_codec_enc_cfg_t *cfg,
                                                   aom_codec_flags_t flags, int ver) {
    return aom_codec_enc_init_ver(ctx, iface, cfg, flags, ver);
}

static aom_codec_err_t fuzz_aom_codec_control(aom_codec_ctx_t *ctx, int ctrl_id, ...) {
    va_list args;
    va_start(args, ctrl_id);
    
    aom_codec_err_t result = AOM_CODEC_ERROR;
    if (ctx != NULL && ctrl_id != 0) {
        switch (ctrl_id) {
            case AOME_SET_CPUUSED:
                result = aom_codec_control(ctx, ctrl_id, va_arg(args, int));
                break;
            case AOME_SET_ENABLEAUTOALTREF:
                result = aom_codec_control(ctx, ctrl_id, va_arg(args, int));
                break;
            case AOME_SET_ARNR_MAXFRAMES:
                result = aom_codec_control(ctx, ctrl_id, va_arg(args, unsigned int));
                break;
            case AOME_SET_ARNR_STRENGTH:
                result = aom_codec_control(ctx, ctrl_id, va_arg(args, unsigned int));
                break;
            case AOME_SET_CQ_LEVEL:
                result = aom_codec_control(ctx, ctrl_id, va_arg(args, unsigned int));
                break;
            default:
                result = AOM_CODEC_INVALID_PARAM;
                break;
        }
    } else {
        result = AOM_CODEC_INVALID_PARAM;
    }
    
    va_end(args);
    return result;
}

static aom_image_t *create_test_image(const uint8_t *data, size_t size) {
    static aom_image_t img;
    
    memset(&img, 0, sizeof(img));
    
    img.fmt = AOM_IMG_FMT_I420;
    img.cp = AOM_CICP_CP_BT_709;
    img.tc = AOM_CICP_TC_BT_709;
    img.mc = AOM_CICP_MC_BT_709;
    img.monochrome = 0;
    img.csp = AOM_CSP_VERTICAL;
    img.range = AOM_CR_STUDIO_RANGE;
    
    img.w = 64;
    img.h = 64;
    img.bit_depth = 8;
    img.d_w = 64;
    img.d_h = 64;
    img.r_w = 64;
    img.r_h = 64;
    img.x_chroma_shift = 1;
    img.y_chroma_shift = 1;
    
    static uint8_t y_plane[64 * 64];
    static uint8_t u_plane[32 * 32];
    static uint8_t v_plane[32 * 32];
    
    size_t y_size = sizeof(y_plane);
    size_t u_size = sizeof(u_plane);
    size_t v_size = sizeof(v_plane);
    
    if (size > 0) {
        size_t copy_size = size < y_size ? size : y_size;
        memcpy(y_plane, data, copy_size);
        
        if (size > y_size) {
            copy_size = (size - y_size) < u_size ? (size - y_size) : u_size;
            memcpy(u_plane, data + y_size, copy_size);
        }
        
        if (size > y_size + u_size) {
            copy_size = (size - y_size - u_size) < v_size ? (size - y_size - u_size) : v_size;
            memcpy(v_plane, data + y_size + u_size, copy_size);
        }
    }
    
    img.planes[0] = y_plane;
    img.planes[1] = u_plane;
    img.planes[2] = v_plane;
    img.stride[0] = 64;
    img.stride[1] = 32;
    img.stride[2] = 32;
    img.sz = y_size + u_size + v_size;
    img.bps = 12;
    img.temporal_id = 0;
    img.spatial_id = 0;
    img.user_priv = NULL;
    
    return &img;
}

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }
    
    static aom_codec_ctx_t codec;
    static aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    
    if (iface == NULL) {
        return 0;
    }
    
    unsigned int usage = Data[0] % 3;
    aom_codec_err_t res = fuzz_aom_codec_enc_config_default(iface, &cfg, usage);
    if (res != AOM_CODEC_OK) {
        return 0;
    }
    
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 256;
    cfg.g_pass = AOM_RC_ONE_PASS;
    cfg.g_lag_in_frames = 0;
    
    aom_codec_flags_t flags = 0;
    if (Data[1] & 0x01) {
        flags |= AOM_CODEC_USE_HIGHBITDEPTH;
    }
    
    res = fuzz_aom_codec_enc_init_ver(&codec, iface, &cfg, flags, AOM_ENCODER_ABI_VERSION);
    if (res != AOM_CODEC_OK) {
        return 0;
    }
    
    if (Size > 10) {
        static const char *option_names[] = {"cpu-used", "end-usage", "cq-level", "arnr-strength"};
        static const char *option_values[] = {"5", "cq", "32", "3"};
        
        size_t name_idx = Data[2] % (sizeof(option_names) / sizeof(option_names[0]));
        size_t value_idx = Data[3] % (sizeof(option_values) / sizeof(option_values[0]));
        
        fuzz_aom_codec_set_option(&codec, option_names[name_idx], option_values[value_idx]);
    }
    
    if (Size > 20) {
        static int control_ids[] = {
            AOME_SET_CPUUSED,
            AOME_SET_ENABLEAUTOALTREF,
            AOME_SET_ARNR_MAXFRAMES,
            AOME_SET_ARNR_STRENGTH,
            AOME_SET_CQ_LEVEL
        };
        
        size_t ctrl_idx = Data[4] % (sizeof(control_ids) / sizeof(control_ids[0]));
        int ctrl_value = (int)Data[5];
        
        fuzz_aom_codec_control(&codec, control_ids[ctrl_idx], ctrl_value);
    }
    
    if (Size > 30) {
        aom_image_t *img = create_test_image(Data + 10, Size - 10);
        
        aom_codec_pts_t pts = 0;
        unsigned long duration = 1;
        aom_enc_frame_flags_t encode_flags = 0;
        
        if (Data[6] & 0x01) {
            encode_flags |= AOM_EFLAG_FORCE_KF;
        }
        
        fuzz_aom_codec_encode(&codec, img, pts, duration, encode_flags);
        
        pts += duration;
        fuzz_aom_codec_encode(&codec, NULL, pts, duration, 0);
    }
    
    if (Size > 100) {
        cfg.rc_target_bitrate = (Data[7] * 100) + 100;
        cfg.rc_min_quantizer = Data[8] % 64;
        cfg.rc_max_quantizer = (Data[9] % 64) + cfg.rc_min_quantizer;
        if (cfg.rc_max_quantizer > 63) {
            cfg.rc_max_quantizer = 63;
        }
        
        fuzz_aom_codec_enc_config_set(&codec, &cfg);
        
        if (Size > 200) {
            aom_image_t *img2 = create_test_image(Data + 100, Size - 100);
            aom_codec_pts_t pts2 = 100;
            fuzz_aom_codec_encode(&codec, img2, pts2, 1, 0);
        }
    }
    
    aom_codec_destroy(&codec);
    
    return 0;
}