// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

static vpx_codec_ctx_t *create_encoder_context(const uint8_t *data, size_t size) {
    vpx_codec_ctx_t *ctx = (vpx_codec_ctx_t *)malloc(sizeof(vpx_codec_ctx_t));
    if (!ctx) return NULL;
    
    memset(ctx, 0, sizeof(vpx_codec_ctx_t));
    
    vpx_codec_enc_cfg_t cfg;
    if (vpx_codec_enc_config_default(&vpx_codec_vp8_cx, &cfg, 0) != VPX_CODEC_OK) {
        free(ctx);
        return NULL;
    }
    
    cfg.g_w = (data[0] % 512) + 16;
    cfg.g_h = (data[1] % 512) + 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = (data[2] * 256 + data[3]) + 1;
    cfg.g_lag_in_frames = (data[4] % 20) + 1;
    
    vpx_codec_iface_t *iface = &vpx_codec_vp8_cx;
    if (iface == NULL) {
        free(ctx);
        return NULL;
    }
    
    ctx->iface = iface;
    ctx->name = "VP8 Encoder";
    
    if (vpx_codec_enc_init(ctx, iface, &cfg, 0) != VPX_CODEC_OK) {
        free(ctx);
        return NULL;
    }
    
    return ctx;
}

static void cleanup_encoder_context(vpx_codec_ctx_t *ctx) {
    if (ctx) {
        vpx_codec_destroy(ctx);
        free(ctx);
    }
}

int LLVMFuzzerTestOneInput_68(const uint8_t *Data, size_t Size) {
    if (Size < 5) {
        return 0;
    }

    vpx_codec_ctx_t *ctx = create_encoder_context(Data, Size);
    if (!ctx) {
        return 0;
    }

    int ctrl_id = (int)(Data[0] & 0xFF);
    
    int test_ctrl_ids[] = {
        ctrl_id,
        (int)(Data[1] & 0xFF),
        (int)(Data[2] & 0xFF),
        (int)(Data[3] & 0xFF),
        (int)(Data[4] & 0xFF),
        0
    };
    
    for (int i = 0; i < 5; i++) {
        int id = test_ctrl_ids[i];
        
        vpx_codec_control_(ctx, id);
        
        int int_arg = (int)(Data[(i + 5) % Size] * 256 + Data[(i + 6) % Size]);
        vpx_codec_control_(ctx, id, int_arg);
        
        static uint8_t dummy_buf[256];
        memset(dummy_buf, 0, sizeof(dummy_buf));
        if (Size > 10) {
            memcpy(dummy_buf, Data + 10, Size > 256 ? 256 : Size - 10);
        }
        vpx_codec_control_(ctx, id, dummy_buf);
        
        int arg2 = (int)(Data[(i + 7) % Size]);
        vpx_codec_control_(ctx, id, int_arg, arg2);
    }
    
    cleanup_encoder_context(ctx);
    return 0;
}