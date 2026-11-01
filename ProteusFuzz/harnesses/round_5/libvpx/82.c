// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_register_put_frame_cb at vpx_decoder.c:133:17 in vpx_decoder.h
// vpx_codec_register_put_slice_cb at vpx_decoder.c:153:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "vpx/vpx_decoder.h"
#include "vpx/vpx_image.h"
#include "vpx/vpx_codec.h"
#include "vpx/vp8dx.h"
#include "vpx/vp8cx.h"

/* Dummy callback functions required for registration */
static void put_frame_cb(void *user_priv, const vpx_image_t *img) {
    (void)user_priv;
    (void)img;
}

/* Correct signature for put_slice_cb based on vpx_codec_put_slice_cb_fn_t */
/* void (*)(void *user_priv, const vpx_image_t *img, const vpx_image_rect_t *rect, const vpx_image_rect_t *rect) */
static void put_slice_cb(void *user_priv, const vpx_image_t *img, 
                         const vpx_image_rect_t *rect, const vpx_image_rect_t *rect2) {
    (void)user_priv;
    (void)img;
    (void)rect;
    (void)rect2;
}

/* Helper to write dummy data to a file if needed */
static void write_dummy_file(const uint8_t *data, size_t size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

int LLVMFuzzerTestOneInput_82(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    write_dummy_file(Data, Size);

    vpx_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    vpx_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.w = 16;
    cfg.h = 16;

    /* Initialize the decoder using the macro which handles ABI versioning */
    vpx_codec_err_t init_res = vpx_codec_dec_init(&ctx, NULL, &cfg, 0);
    if (init_res != VPX_CODEC_OK) {
        return 0;
    }

    /* Register callbacks */
    vpx_codec_register_put_frame_cb(&ctx, put_frame_cb, NULL);
    vpx_codec_register_put_slice_cb(&ctx, put_slice_cb, NULL);

    /* Fuzz the decode function with the input data */
    vpx_codec_decode(&ctx, Data, Size, NULL, 0);

    /* Iterate over decoded frames to explore state */
    vpx_codec_iter_t iter = NULL;
    vpx_image_t *img;
    while ((img = vpx_codec_get_frame(&ctx, &iter)) != NULL) {
        (void)img;
    }

    /* Flush the decoder */
    vpx_codec_decode(&ctx, NULL, 0, NULL, 0);

    /* Clean up */
    vpx_codec_destroy(&ctx);

    return 0;
}