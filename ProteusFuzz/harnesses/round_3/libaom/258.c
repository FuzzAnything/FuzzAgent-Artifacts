// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_wrap at aom_image.c:216:14 in aom_image.h
// aom_img_plane_height at aom_image.c:320:5 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_img_wrap at aom_image.c:216:14 in aom_image.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aom_codec.h"
#include "aom/aom_image.h"
#include "aom/aomdx.h"
#include "aom/aomcx.h"
#include "aom/aom_integer.h"

static int calculate_image_size(aom_img_fmt_t fmt, unsigned int d_w, unsigned int d_h, unsigned int stride_align) {
    if (d_w == 0 || d_h == 0 || stride_align == 0) return 0;
    if (d_w > 0x08000000 || d_h > 0x08000000) return 0;
    if (stride_align > 65536) return 0;

    int x_chroma_shift = 1;
    int y_chroma_shift = 1;
    int bps = 8;
    int is_planar = (fmt & AOM_IMG_FMT_PLANAR);
    int is_highbit = (fmt & AOM_IMG_FMT_HIGHBITDEPTH);

    if (fmt == AOM_IMG_FMT_I422 || fmt == AOM_IMG_FMT_I42216) y_chroma_shift = 0;
    if (fmt == AOM_IMG_FMT_I444 || fmt == AOM_IMG_FMT_I44416) { x_chroma_shift = 0; y_chroma_shift = 0; }
    if (fmt == AOM_IMG_FMT_NV12) { /* NV12 is planar but U/V interleaved, handled by logic below */ }

    if (is_highbit) bps = 16;

    int align = (1 << x_chroma_shift) - 1;
    int w = (d_w + align) & ~align;
    align = (1 << y_chroma_shift) - 1;
    int h = (d_h + align) & ~align;

    uint64_t s;
    if (is_planar) {
        s = w;
    } else {
        s = (uint64_t)bps * w / 8;
    }

    if (is_highbit) s = s * 2;

    s = (s + stride_align - 1) & ~((uint64_t)stride_align - 1);

    if (is_highbit) s = s / 2;

    uint64_t alloc_size;
    if (is_planar) {
        alloc_size = (uint64_t)h * s * bps / 8;
    } else {
        alloc_size = (uint64_t)h * s;
    }

    return (int)alloc_size;
}

int LLVMFuzzerTestOneInput_258(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    // Extract parameters from fuzz input
    uint32_t param0 = 0;
    uint32_t param1 = 0;
    uint32_t param2 = 0;
    uint32_t param3 = 0;

    if (Size >= 4) param0 = *(uint32_t*)(Data);
    if (Size >= 8) param1 = *(uint32_t*)(Data + 4);
    if (Size >= 12) param2 = *(uint32_t*)(Data + 8);
    if (Size >= 16) param3 = *(uint32_t*)(Data + 12);

    // Clamp values to reasonable ranges to avoid immediate crashes or allocation failures
    unsigned int d_w = (param0 % 4096) + 1;
    unsigned int d_h = (param1 % 4096) + 1;
    unsigned int stride_align = (param2 % 65535) + 1;
    int plane_idx = param3 % 3;
    int fmt_idx = (param3 >> 8) % 10;

    aom_img_fmt_t fmts[] = {
        AOM_IMG_FMT_YV12, AOM_IMG_FMT_I420, AOM_IMG_FMT_AOMYV12, AOM_IMG_FMT_AOMI420,
        AOM_IMG_FMT_I422, AOM_IMG_FMT_I444, AOM_IMG_FMT_NV12, AOM_IMG_FMT_I42016,
        AOM_IMG_FMT_YV1216, AOM_IMG_FMT_I42216
    };
    aom_img_fmt_t fmt = (fmt_idx < 10) ? fmts[fmt_idx] : AOM_IMG_FMT_I420;

    // 1. Prepare environment for aom_img_wrap
    size_t img_size = calculate_image_size(fmt, d_w, d_h, stride_align);
    if (img_size == 0 || img_size > 1024 * 1024 * 10) return 0; // Limit size

    unsigned char *img_data = (unsigned char *)malloc(img_size);
    if (!img_data) return 0;
    memset(img_data, 0, img_size);

    aom_image_t img;
    aom_image_t *wrapped_img = aom_img_wrap(&img, fmt, d_w, d_h, stride_align, img_data);

    if (wrapped_img) {
        // 2. Fuzz aom_img_plane_height
        // This function is safe to call with valid image and plane index
        int h = aom_img_plane_height(wrapped_img, plane_idx);
        (void)h;

        // Cleanup
        aom_img_free(wrapped_img);
    }

    // 3. Prepare environment for aom_codec_get_frame
    // Initialize decoder context
    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    aom_codec_iface_t *decoder_iface = aom_codec_av1_dx();
    if (!decoder_iface) {
        free(img_data);
        return 0;
    }

    aom_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.w = d_w;
    cfg.h = d_h;

    aom_codec_err_t err = aom_codec_dec_init(&ctx, decoder_iface, &cfg, 0);
    if (err != AOM_CODEC_OK) {
        free(img_data);
        return 0;
    }

    // 4. Fuzz aom_codec_control_typechecked_AOMD_GET_STILL_PICTURE
    // Even if signature is unknown, we can try to pass a struct pointer
    aom_still_picture_info still_info;
    memset(&still_info, 0, sizeof(still_info));
    // The actual macro/function call depends on internal implementation, 
    // but typically it's aom_codec_control(&ctx, AOMD_GET_STILL_PICTURE, &still_info);
    // Since the prompt asks to fuzz the specific typechecked function if possible,
    // and we don't have the macro, we assume the standard control path or skip if undefined.
    // We will attempt the standard control call which maps to the typechecked logic internally.
    aom_codec_control(&ctx, AOMD_GET_STILL_PICTURE, &still_info);

    // 5. Fuzz aom_codec_control_typechecked_AOMD_GET_FWD_KF_PRESENT
    // Similar to above, we attempt the standard control call.
    // Usually expects an int* or similar.
    int kf_present = 0;
    aom_codec_control(&ctx, AOMD_GET_FWD_KF_PRESENT, &kf_present);

    // 6. Fuzz aom_codec_control_typechecked_AV1_COPY_NEW_FRAME_IMAGE
    // This typically requires a valid image and context.
    // We try to pass the wrapped image we created earlier (re-wrapped for safety)
    aom_image_t img2;
    aom_image_t *wrapped_img2 = aom_img_wrap(&img2, fmt, d_w, d_h, stride_align, img_data);
    if (wrapped_img2) {
        // Attempt to call the control. The exact macro might be AV1_COPY_NEW_FRAME_IMAGE
        // We pass the image pointer.
        aom_codec_control(&ctx, AV1_COPY_NEW_FRAME_IMAGE, wrapped_img2);
        aom_img_free(wrapped_img2);
    }

    // 7. Fuzz aom_codec_get_frame
    aom_codec_iter_t iter = NULL;
    // First, decode some dummy data to potentially populate frames
    // We use the fuzz input as raw bitstream data (truncated if too large)
    size_t decode_size = (Size > 16) ? (Size - 16) : 0;
    if (decode_size > 0) {
        const uint8_t *bitstream = Data + 16;
        aom_codec_decode(&ctx, bitstream, decode_size, NULL);
    }

    // Iterate frames
    aom_image_t *frame = NULL;
    while ((frame = aom_codec_get_frame(&ctx, &iter)) != NULL) {
        // Process frame if needed, here just accessing to exercise code
        if (frame->planes[0]) {
            // Safe access
            (void)frame->planes[0][0];
        }
    }

    // Cleanup
    aom_codec_destroy(&ctx);
    free(img_data);

    return 0;
}