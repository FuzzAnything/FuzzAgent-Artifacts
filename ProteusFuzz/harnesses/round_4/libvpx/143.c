// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp9_dx at vp9_dx_iface.c:712:1 in vp8dx.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_decoder.h>
#include <vpx/vp8cx.h>
#include <vpx/vp8dx.h>

static int fuzz_vp8(const uint8_t *data, size_t size) {
    if (size < 10) return 0;

    vpx_codec_ctx_t decoder;
    vpx_codec_dec_cfg_t cfg = {0};
    cfg.w = 640;
    cfg.h = 480;
    cfg.threads = 1;

    if (vpx_codec_dec_init(&decoder, vpx_codec_vp8_dx(), &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    vpx_codec_iter_t iter = NULL;
    vpx_image_t *img;

    vpx_codec_decode(&decoder, data, size, NULL, 0);
    
    while ((img = vpx_codec_get_frame(&decoder, &iter)) != NULL) {
        (void)img;
    }

    vpx_codec_destroy(&decoder);
    return 0;
}

static int fuzz_vp9(const uint8_t *data, size_t size) {
    if (size < 10) return 0;

    vpx_codec_ctx_t decoder;
    vpx_codec_dec_cfg_t cfg = {0};
    cfg.w = 640;
    cfg.h = 480;
    cfg.threads = 1;

    if (vpx_codec_dec_init(&decoder, vpx_codec_vp9_dx(), &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    vpx_codec_iter_t iter = NULL;
    vpx_image_t *img;

    vpx_codec_decode(&decoder, data, size, NULL, 0);
    
    while ((img = vpx_codec_get_frame(&decoder, &iter)) != NULL) {
        (void)img;
    }

    vpx_codec_destroy(&decoder);
    return 0;
}

int LLVMFuzzerTestOneInput_143(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    fuzz_vp8(Data, Size);
    fuzz_vp9(Data, Size);

    return 0;
}