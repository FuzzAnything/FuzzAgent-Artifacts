// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp9_cx at vp9_cx_iface.c:2277:1 in vp8cx.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_decoder.h"
#include "vpx/vpx_encoder.h"
#include "vpx/vpx_image.h"
#include "vpx/vp8cx.h"
#include "vpx/vp8dx.h"

static int fuzz_encoder(const uint8_t *Data, size_t Size) {
    if (Size < 6) return 0;

    vpx_codec_ctx_t ctx;
    vpx_codec_iface_t *codec = vpx_codec_vp9_cx();
    if (!codec) return 0;

    vpx_codec_enc_cfg_t cfg;
    if (vpx_codec_enc_config_default(codec, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Initialize configuration from fuzz data with bounds checking
    cfg.g_w = Data[0] ? Data[0] : 16;
    cfg.g_h = Data[1] ? Data[1] : 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 1;
    cfg.rc_target_bitrate = Data[2] ? Data[2] : 100;
    cfg.g_error_resilient = 1;

    if (vpx_codec_enc_init(&ctx, codec, &cfg, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Encode a single frame
    vpx_image_t img;
    vpx_img_alloc(&img, VPX_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
    
    if (Size > 5) {
        memset(img.planes[0], Data[3] % 256, img.stride[0] * cfg.g_h);
        memset(img.planes[1], Data[4] % 256, img.stride[1] * (cfg.g_h / 2));
        memset(img.planes[2], Data[5] % 256, img.stride[2] * (cfg.g_h / 2));
    }

    vpx_codec_frame_flags_t flags = 0;
    vpx_codec_encode(&ctx, &img, 0, 1, flags, 0);

    vpx_codec_destroy(&ctx);
    vpx_img_free(&img);
    return 0;
}

static int fuzz_decoder(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    vpx_codec_ctx_t ctx;
    vpx_codec_iface_t *codec = vpx_codec_vp8_dx();
    if (!codec) return 0;

    if (vpx_codec_dec_init(&ctx, codec, NULL, 0) != VPX_CODEC_OK) {
        return 0;
    }

    // Decode the fuzzed data
    vpx_image_t frame;
    vpx_codec_err_t res = vpx_codec_decode(&ctx, Data, Size, &frame, 0);

    if (res == VPX_CODEC_OK) {
        vpx_img_free(&frame);
    }

    vpx_codec_destroy(&ctx);
    return 0;
}

int LLVMFuzzerTestOneInput_120(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Randomly choose between encoder and decoder fuzzing
    // Ensure we have enough data for encoder before choosing it
    if (Data[0] % 2 == 0 && Size >= 6) {
        fuzz_encoder(Data, Size);
    } else {
        fuzz_decoder(Data, Size);
    }

    return 0;
}