// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aom_image.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_decoder.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_integer.h>

static int fuzz_encoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t res;
    aom_image_t *img = NULL;
    int val;
    const aom_codec_iface_t *iface;

    if (size < 10) return 0;

    // Explicitly get the interface to ensure it is valid before using it
    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    res = aom_codec_enc_config_default(iface, &cfg, 0);
    if (res != AOM_CODEC_OK) return 0;

    // Use fuzzing data to configure encoder settings safely
    cfg.g_w = 64 + (data[0] % 32);
    cfg.g_h = 64 + (data[1] % 32);
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100 + (data[2] % 1000);
    cfg.g_error_resilient = (data[3] & 1) ? 1 : 0;

    res = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (res != AOM_CODEC_OK) return 0;

    // Create a dummy image
    img = aom_img_alloc(NULL, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 16);
    if (!img) {
        aom_codec_destroy(&ctx);
        return 0;
    }

    // Fill image with fuzzing data
    memset(img->planes[0], 0, img->d_w * img->d_h);
    memset(img->planes[1], 128, (img->d_w / 2) * (img->d_h / 2));
    memset(img->planes[2], 128, (img->d_w / 2) * (img->d_h / 2));

    // Fuzz various control parameters using the provided data
    val = data[4] % 2;
    res = aom_codec_control(&ctx, AV1E_SET_INTRA_DCT_ONLY, val);
    (void)res;

    val = data[5] % 2;
    res = aom_codec_control(&ctx, AV1E_SET_ENABLE_TX_SIZE_SEARCH, val);
    (void)res;

    val = data[6] % 2;
    res = aom_codec_control(&ctx, AV1E_SET_ENABLE_RECT_TX, val);
    (void)res;

    val = data[7] % 2;
    res = aom_codec_control(&ctx, AV1E_SET_COLOR_RANGE, val);
    (void)res;

    val = data[8] % 4; // Matrix coefficients usually have a few options
    res = aom_codec_control(&ctx, AV1E_SET_MATRIX_COEFFICIENTS, val);
    (void)res;

    val = data[9] % 2;
    res = aom_codec_control(&ctx, AV1E_SET_ENABLE_INTERINTRA_COMP, val);
    (void)res;

    // Encode a frame
    res = aom_codec_encode(&ctx, img, 0, 1, 0);
    (void)res;

    aom_img_free(img);
    aom_codec_destroy(&ctx);
    return 0;
}

int LLVMFuzzerTestOneInput_295(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;
    return fuzz_encoder(Data, Size);
}