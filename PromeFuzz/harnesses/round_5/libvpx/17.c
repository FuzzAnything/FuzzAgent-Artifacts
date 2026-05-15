// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_get_preview_frame at vpx_encoder.c:314:20 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_codec_get_preview_frame at vpx_encoder.c:314:20 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_codec.h"
#include "vpx/vpx_decoder.h"
#include "vpx/vpx_encoder.h"
#include "vpx/vp8dx.h"
#include "vpx/vp8cx.h"

static void fuzz_decoder(const uint8_t *data, size_t size) {
    vpx_codec_ctx_t ctx;
    vpx_codec_iface_t *iface = vpx_codec_vp8_dx();
    vpx_codec_dec_cfg_t cfg = {0};
    
    if (size > 0) {
        cfg.threads = data[0] % 8 + 1;
        cfg.w = (size > 1) ? (data[1] % 128 + 1) * 16 : 640;
        cfg.h = (size > 2) ? (data[2] % 128 + 1) * 16 : 480;
    } else {
        cfg.threads = 1;
        cfg.w = 640;
        cfg.h = 480;
    }
    
    vpx_codec_flags_t flags = 0;
    if (size > 3) {
        flags = data[3] & (VPX_CODEC_USE_INPUT_FRAGMENTS | VPX_CODEC_USE_POSTPROC);
    }
    
    vpx_codec_err_t err = vpx_codec_dec_init_ver(&ctx, iface, &cfg, flags, VPX_DECODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        const char *detail = vpx_codec_error_detail(&ctx);
        (void)detail;
        return;
    }
    
    size_t data_offset = (size > 4) ? 4 : size;
    const uint8_t *decode_data = data + data_offset;
    size_t decode_size = size - data_offset;
    
    if (decode_size > 0) {
        long deadlines[] = {0, 1000, 10000};
        for (int i = 0; i < 3; i++) {
            err = vpx_codec_decode(&ctx, decode_data, (unsigned int)decode_size, NULL, deadlines[i]);
            
            if (err != VPX_CODEC_OK) {
                const char *detail = vpx_codec_error_detail(&ctx);
                (void)detail;
            }
            
            vpx_codec_iter_t iter = NULL;
            vpx_image_t *img;
            while ((img = vpx_codec_get_frame(&ctx, &iter)) != NULL) {
                (void)img->fmt;
                (void)img->w;
                (void)img->h;
                (void)img->bit_depth;
            }
        }
        
        err = vpx_codec_decode(&ctx, NULL, 0, NULL, 0);
        
        vpx_codec_iter_t iter = NULL;
        vpx_image_t *img;
        while ((img = vpx_codec_get_frame(&ctx, &iter)) != NULL) {
            (void)img->fmt;
            (void)img->w;
            (void)img->h;
        }
    }
    
    const vpx_image_t *preview = vpx_codec_get_preview_frame(&ctx);
    (void)preview;
    
    vpx_codec_destroy(&ctx);
}

static void fuzz_encoder(const uint8_t *data, size_t size) {
    vpx_codec_ctx_t ctx;
    vpx_codec_iface_t *iface = vpx_codec_vp8_cx();
    
    vpx_codec_enc_cfg_t cfg;
    vpx_codec_enc_config_default(iface, &cfg, 0);
    
    if (size > 0) {
        cfg.g_w = (data[0] % 64 + 1) * 16;
        cfg.g_h = (size > 1) ? (data[1] % 64 + 1) * 16 : cfg.g_w;
        cfg.g_timebase.num = 1;
        cfg.g_timebase.den = (size > 2) ? data[2] % 60 + 1 : 30;
    }
    
    vpx_codec_flags_t flags = 0;
    
    vpx_codec_err_t err = vpx_codec_enc_init_ver(&ctx, iface, &cfg, flags, VPX_ENCODER_ABI_VERSION);
    if (err != VPX_CODEC_OK) {
        const char *detail = vpx_codec_error_detail(&ctx);
        (void)detail;
        return;
    }
    
    const vpx_image_t *preview = vpx_codec_get_preview_frame(&ctx);
    (void)preview;
    
    vpx_codec_destroy(&ctx);
}

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    uint8_t codec_choice = Data[0];
    
    if (codec_choice % 2 == 0) {
        fuzz_decoder(Data, Size);
    } else {
        fuzz_encoder(Data, Size);
    }
    
    return 0;
}