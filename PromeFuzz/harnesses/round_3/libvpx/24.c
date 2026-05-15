// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_vp9_dx at vp9_dx_iface.c:712:1 in vp8dx.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_register_put_frame_cb at vpx_decoder.c:133:17 in vpx_decoder.h
// vpx_codec_register_put_slice_cb at vpx_decoder.c:153:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_get_stream_info at vpx_decoder.c:85:17 in vpx_decoder.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_decoder.h"
#include "vpx/vp8dx.h"

static void dummy_put_frame_cb(void *user_priv, const vpx_image_t *img) {
    (void)user_priv;
    (void)img;
}

static void dummy_put_slice_cb(void *user_priv, const vpx_image_t *img,
                               const vpx_image_rect_t *valid,
                               const vpx_image_rect_t *update) {
    (void)user_priv;
    (void)img;
    (void)valid;
    (void)update;
}

int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    vpx_codec_ctx_t ctx;
    vpx_codec_iface_t *iface;
    
    uint8_t decoder_choice = Data[0] % 2;
    if (decoder_choice == 0) {
        iface = vpx_codec_vp8_dx();
    } else {
        iface = vpx_codec_vp9_dx();
    }
    
    if (!iface) return 0;
    
    vpx_codec_dec_cfg_t cfg;
    cfg.threads = (Data[1] % 8) + 1;
    cfg.w = (Data[2] % 1024) + 1;
    cfg.h = (Data[3] % 1024) + 1;
    
    vpx_codec_flags_t flags = 0;
    if (Data[4] & 0x01) flags |= VPX_CODEC_USE_INPUT_FRAGMENTS;
    if (Data[4] & 0x02) flags |= VPX_CODEC_USE_POSTPROC;
    if (Data[4] & 0x04) flags |= VPX_CODEC_USE_ERROR_CONCEALMENT;
    
    vpx_codec_err_t init_err = vpx_codec_dec_init_ver(&ctx, iface, &cfg, flags, VPX_DECODER_ABI_VERSION);
    if (init_err != VPX_CODEC_OK) {
        return 0;
    }
    
    if (Data[5] & 0x01) {
        vpx_codec_register_put_frame_cb(&ctx, dummy_put_frame_cb, NULL);
    }
    
    if (Data[5] & 0x02) {
        vpx_codec_register_put_slice_cb(&ctx, dummy_put_slice_cb, NULL);
    }
    
    size_t decode_size = Size > 6 ? Size - 6 : 0;
    const uint8_t *decode_data = Size > 6 ? Data + 6 : NULL;
    
    vpx_codec_err_t decode_err = vpx_codec_decode(&ctx, decode_data, decode_size, NULL, 0);
    (void)decode_err;
    
    vpx_codec_stream_info_t si;
    memset(&si, 0, sizeof(si));
    si.sz = sizeof(si);
    vpx_codec_err_t stream_info_err = vpx_codec_get_stream_info(&ctx, &si);
    (void)stream_info_err;
    
    vpx_codec_iter_t iter = NULL;
    const vpx_image_t *img;
    while ((img = vpx_codec_get_frame(&ctx, &iter)) != NULL) {
        (void)img;
    }
    
    vpx_codec_decode(&ctx, NULL, 0, NULL, 0);
    
    iter = NULL;
    while ((img = vpx_codec_get_frame(&ctx, &iter)) != NULL) {
        (void)img;
    }
    
    vpx_codec_destroy(&ctx);
    
    return 0;
}