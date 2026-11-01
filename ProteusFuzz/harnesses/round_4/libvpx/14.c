// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_vp9_dx at vp9_dx_iface.c:712:1 in vp8dx.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "vpx_decoder.h"
#include "vpx_codec.h"
#include "vp8cx.h"
#include "vp8dx.h"

static int init_decoder(vpx_codec_ctx_t *ctx, const vpx_codec_iface_t *iface) {
    vpx_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.w = 1920;
    cfg.h = 1080;
    
    return vpx_codec_dec_init(ctx, iface, &cfg, 0);
}

int LLVMFuzzerTestOneInput_14(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    vpx_codec_ctx_t ctx;
    vpx_codec_iter_t iter = NULL;
    const vpx_codec_iface_t *iface = vpx_codec_vp9_dx();

    if (iface == NULL) {
        return 0;
    }

    if (init_decoder(&ctx, iface) != VPX_CODEC_OK) {
        return 0;
    }

    vpx_codec_err_t ret = vpx_codec_decode(&ctx, Data, Size, NULL, 0);

    if (ret == VPX_CODEC_OK) {
        vpx_image_t *img;
        while ((img = vpx_codec_get_frame(&ctx, &iter)) != NULL) {
            (void)img;
        }
    }

    vpx_codec_destroy(&ctx);

    return 0;
}