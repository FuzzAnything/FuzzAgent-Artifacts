// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_error at vpx_codec.c:54:13 in vpx_codec.h
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_error at vpx_codec.c:54:13 in vpx_codec.h
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_control_ at vpx_codec.c:89:17 in vpx_codec.h
// vpx_codec_error at vpx_codec.c:54:13 in vpx_codec.h
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_codec.h"
#include "vpx/vpx_decoder.h"
#include "vpx/vp8dx.h"

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    vpx_codec_ctx_t ctx;
    vpx_codec_iface_t *iface = vpx_codec_vp8_dx();
    vpx_codec_dec_cfg_t cfg = {0};
    cfg.threads = (Data[0] % 4) + 1;
    cfg.w = 640;
    cfg.h = 480;

    vpx_codec_err_t init_err = vpx_codec_dec_init_ver(&ctx, iface, &cfg, 0, VPX_DECODER_ABI_VERSION);
    if (init_err != VPX_CODEC_OK) {
        const char *error_msg = vpx_codec_error(&ctx);
        const char *error_detail = vpx_codec_error_detail(&ctx);
        return 0;
    }

    if (Size > 1) {
        unsigned int data_sz = (Size > 1000000) ? 1000000 : (unsigned int)Size;
        vpx_codec_err_t decode_err = vpx_codec_decode(&ctx, Data, data_sz, NULL, 0);
        
        if (decode_err != VPX_CODEC_OK) {
            const char *error_msg = vpx_codec_error(&ctx);
            const char *error_detail = vpx_codec_error_detail(&ctx);
        } else {
            vpx_codec_iter_t iter = NULL;
            vpx_image_t *img;
            while ((img = vpx_codec_get_frame(&ctx, &iter)) != NULL) {
            }
        }
    }

    if (Size > 2) {
        int ctrl_id = Data[1] % 10;
        int value = Data[2];
        
        switch (ctrl_id) {
            case 0:
                vpx_codec_control_(&ctx, VP8D_GET_LAST_REF_UPDATES, &value);
                break;
            case 1:
                vpx_codec_control_(&ctx, VP8D_GET_FRAME_CORRUPTED, &value);
                break;
            case 2:
                vpx_codec_control_(&ctx, VP8D_GET_LAST_REF_USED, &value);
                break;
            case 3:
                vpx_codec_control_(&ctx, VP9D_GET_FRAME_SIZE, &value);
                break;
            case 4:
                vpx_codec_control_(&ctx, VP9D_GET_DISPLAY_SIZE, &value);
                break;
            case 5:
                vpx_codec_control_(&ctx, VP9D_GET_BIT_DEPTH, &value);
                break;
            default:
                break;
        }
    }

    const char *final_error = vpx_codec_error(&ctx);
    const char *final_detail = vpx_codec_error_detail(&ctx);

    vpx_codec_destroy(&ctx);

    return 0;
}