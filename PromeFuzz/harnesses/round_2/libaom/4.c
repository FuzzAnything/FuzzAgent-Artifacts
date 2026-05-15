// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_dx at av1_dx_iface.c:1796:20 in aomdx.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_alloc_with_border at aom_image.c:225:14 in aom_image.h
// aom_img_alloc_with_border at aom_image.c:225:14 in aom_image.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_img_flip at aom_image.c:285:6 in aom_image.h
// aom_img_flip at aom_image.c:285:6 in aom_image.h
// aom_img_flip at aom_image.c:285:6 in aom_image.h
// aom_img_remove_metadata at aom_image.c:412:6 in aom_image.h
// aom_img_remove_metadata at aom_image.c:412:6 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom/aom_image.h"
#include "aom/aom_decoder.h"
#include "aom/aomdx.h"

static const aom_img_fmt_t formats[] = {
    AOM_IMG_FMT_YV12,
    AOM_IMG_FMT_I420,
    AOM_IMG_FMT_AOMYV12,
    AOM_IMG_FMT_AOMI420,
    AOM_IMG_FMT_I422,
    AOM_IMG_FMT_I444,
    AOM_IMG_FMT_NV12,
    AOM_IMG_FMT_I42016,
    AOM_IMG_FMT_YV1216,
    AOM_IMG_FMT_I42216,
    AOM_IMG_FMT_I44416
};

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Use first byte to determine test path
    uint8_t selector = Data[0];
    Data++;
    Size--;

    // Initialize parameters from fuzzer data with safe limits
    unsigned int d_w = 0, d_h = 0, align = 0, size_align = 0, border = 0;
    aom_img_fmt_t fmt = AOM_IMG_FMT_I420;

    if (Size >= 5) {
        d_w = (Data[0] << 8) | Data[1];
        d_h = (Data[2] << 8) | Data[3];
        fmt = formats[Data[4] % (sizeof(formats)/sizeof(formats[0]))];
        Data += 5;
        Size -= 5;
    }

    if (Size >= 3) {
        align = Data[0] % 256;
        size_align = Data[1] % 256;
        border = Data[2] % 256;
        Data += 3;
        Size -= 3;
    }

    // Limit dimensions to prevent excessive memory allocation
    d_w = d_w % 4096 + 1;
    d_h = d_h % 4096 + 1;
    align = align % 256 + 1;
    size_align = size_align % 256 + 1;
    border = border % 256;

    // Test aom_img_alloc_with_border and aom_img_alloc
    aom_image_t *img1 = NULL;
    aom_image_t *img2 = NULL;
    aom_image_t stack_img;

    if (selector & 0x01) {
        // Test with NULL img parameter
        img1 = aom_img_alloc_with_border(NULL, fmt, d_w, d_h, align, size_align, border);
    } else {
        // Test with stack-allocated img parameter
        memset(&stack_img, 0, sizeof(stack_img));
        img1 = aom_img_alloc_with_border(&stack_img, fmt, d_w, d_h, align, size_align, border);
    }

    if (selector & 0x02) {
        img2 = aom_img_alloc(NULL, fmt, d_w, d_h, align);
    }

    // Test aom_img_flip on valid images
    if (img1) {
        aom_img_flip(img1);
        // Flip back to original state
        aom_img_flip(img1);
    }
    if (img2) {
        aom_img_flip(img2);
    }

    // Test aom_img_remove_metadata
    if (img1) {
        aom_img_remove_metadata(img1);
    }
    if (img2) {
        aom_img_remove_metadata(img2);
    }

    // Test aom_codec_get_frame if we have enough data
    if (Size > 0 && (selector & 0x04)) {
        aom_codec_ctx_t ctx;
        aom_codec_iter_t iter = NULL;
        aom_image_t *frame = NULL;
        
        // Initialize decoder with AV1
        if (aom_codec_dec_init(&ctx, aom_codec_av1_dx(), NULL, 0) == AOM_CODEC_OK) {
            // Try to decode the fuzzer data
            aom_codec_decode(&ctx, Data, Size, NULL);
            
            // Iterate through available frames
            while ((frame = aom_codec_get_frame(&ctx, &iter)) != NULL) {
                // Just iterate, no need to do anything with the frame
                (void)frame;
            }
            
            aom_codec_destroy(&ctx);
        }
    }

    // Test aom_img_free (safe with NULL)
    aom_img_free(NULL);
    aom_img_free(img1);
    aom_img_free(img2);

    return 0;
}