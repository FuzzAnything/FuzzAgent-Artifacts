// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_img_wrap at aom_image.c:216:14 in aom_image.h
// aom_img_plane_height at aom_image.c:320:5 in aom_image.h
// aom_img_plane_height at aom_image.c:320:5 in aom_image.h
// aom_img_plane_height at aom_image.c:320:5 in aom_image.h
// aom_img_plane_height at aom_image.c:320:5 in aom_image.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
// aom_img_plane_height at aom_image.c:320:5 in aom_image.h
// aom_img_plane_height at aom_image.c:320:5 in aom_image.h
// aom_img_plane_height at aom_image.c:320:5 in aom_image.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom_codec.h>
#include <aom/aomdx.h>

static aom_codec_ctx_t *init_decoder(const uint8_t *data, size_t size) {
    aom_codec_ctx_t *ctx = NULL;
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    aom_codec_dec_cfg_t cfg;

    if (iface == NULL) return NULL;

    /* Allocate ctx on heap to prevent stack-buffer-overflow during memset in libaom */
    ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
    if (ctx == NULL) return NULL;

    /* Initialize config with defaults */
    memset(&cfg, 0, sizeof(cfg));

    if (aom_codec_dec_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        free(ctx);
        return NULL;
    }

    /* Decode the input data */
    aom_codec_decode(ctx, data, size, NULL);

    return ctx;
}

int LLVMFuzzerTestOneInput_176(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    /* --- Test aom_img_wrap and aom_img_plane_height --- */
    unsigned int d_w = 16, d_h = 16;
    unsigned int stride_align = 16;
    aom_img_fmt_t fmt = AOM_IMG_FMT_I420;
    
    /* Calculate approximate size needed for I420 16x16 with alignment */
    /* Y: 16*16, U: 8*8, V: 8*8. With stride align 16, stride is 16. */
    size_t alloc_size = (16 * 16) + (8 * 16) + (8 * 16); 
    if (Size > alloc_size) {
        alloc_size = Size;
    }

    unsigned char *img_data = malloc(alloc_size);
    if (!img_data) return 0;

    /* Fill with fuzz data */
    memset(img_data, 0, alloc_size);
    if (Size > 0) {
        memcpy(img_data, Data, Size < alloc_size ? Size : alloc_size);
    }

    aom_image_t img;
    aom_image_t *wrapped_img = aom_img_wrap(&img, fmt, d_w, d_h, stride_align, img_data);

    if (wrapped_img != NULL) {
        /* Test aom_img_plane_height for all planes (0, 1, 2) */
        int h0 = aom_img_plane_height(wrapped_img, 0);
        int h1 = aom_img_plane_height(wrapped_img, 1);
        int h2 = aom_img_plane_height(wrapped_img, 2);
        
        /* Also test with invalid plane index to ensure robustness */
        int h_invalid = aom_img_plane_height(wrapped_img, 5);
    }

    free(img_data);

    /* --- Test aom_codec_get_frame and aom_codec_control --- */
    if (Size < 10) return 0; /* Skip codec init if data is too small */

    aom_codec_ctx_t *ctx = init_decoder(Data, Size);
    if (ctx == NULL) return 0;

    aom_codec_iter_t iter = NULL;
    aom_image_t *frame = NULL;

    /* Iterate frames */
    while ((frame = aom_codec_get_frame(ctx, &iter)) != NULL) {
        /* Test aom_img_plane_height on decoded frame */
        if (frame->planes[0] != NULL) {
            int h0 = aom_img_plane_height(frame, 0);
            int h1 = aom_img_plane_height(frame, 1);
            int h2 = aom_img_plane_height(frame, 2);
        }

        /* Test aom_codec_control with AOMD_GET_STILL_PICTURE */
        aom_still_picture_info info;
        memset(&info, 0, sizeof(info));
        aom_codec_control(ctx, AOMD_GET_STILL_PICTURE, &info);

        /* Test aom_codec_control with AOMD_GET_FWD_KF_PRESENT */
        int fwd_kf_present = 0;
        aom_codec_control(ctx, AOMD_GET_FWD_KF_PRESENT, &fwd_kf_present);

        /* Test aom_codec_control with AV1_COPY_NEW_FRAME_IMAGE */
        /* Pass the current frame as the destination buffer */
        aom_codec_control(ctx, AV1_COPY_NEW_FRAME_IMAGE, frame);
    }

    aom_codec_destroy(ctx);
    free(ctx);

    return 0;
}