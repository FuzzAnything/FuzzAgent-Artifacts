// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_alloc_with_border at aom_image.c:225:14 in aom_image.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_img_flip at aom_image.c:285:6 in aom_image.h
// aom_img_remove_metadata at aom_image.c:412:6 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_flip at aom_image.c:285:6 in aom_image.h
// aom_img_remove_metadata at aom_image.c:412:6 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom/aom_decoder.h"
#include "aom/aom_image.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

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

static aom_image_t* create_test_image(const uint8_t *data, size_t size, int use_border) {
    if (size < 5) return NULL;
    
    uint8_t fmt_idx = data[0] % (sizeof(formats) / sizeof(formats[0]));
    unsigned int width = (data[1] + 1) & 0x7F;  // Limit to reasonable size
    unsigned int height = (data[2] + 1) & 0x7F;
    unsigned int align = (data[3] % 32) + 1;    // Alignment 1-32
    unsigned int border = data[4] % 16;         // Border 0-15
    unsigned int size_align = (data[4] % 8) + 1; // Size alignment 1-8
    
    aom_image_t *img = NULL;
    
    if (use_border) {
        img = aom_img_alloc_with_border(NULL, formats[fmt_idx], width, height, 
                                       align, size_align, border);
    } else {
        img = aom_img_alloc(NULL, formats[fmt_idx], width, height, align);
    }
    
    return img;
}

int LLVMFuzzerTestOneInput_13(const uint8_t *data, size_t size) {
    if (size == 0) return 0;
    
    // Test aom_img_alloc
    aom_image_t *img1 = create_test_image(data, size, 0);
    if (img1) {
        // Test aom_img_flip
        aom_img_flip(img1);
        
        // Test aom_img_remove_metadata (safe even without metadata)
        aom_img_remove_metadata(img1);
        
        aom_img_free(img1);
    }
    
    // Test aom_img_alloc_with_border
    if (size >= 6) {
        aom_image_t *img2 = create_test_image(data + 1, size - 1, 1);
        if (img2) {
            aom_img_flip(img2);
            aom_img_remove_metadata(img2);
            aom_img_free(img2);
        }
    }
    
    // Test aom_codec_get_frame with minimal context
    // Note: This requires a valid codec context, so we create a minimal one
    // and test the function even though it will likely return NULL
    aom_codec_ctx_t ctx = {0};
    aom_codec_iter_t iter = NULL;
    
    // The function should handle NULL or invalid context gracefully
    aom_image_t *frame = aom_codec_get_frame(&ctx, &iter);
    (void)frame; // Ignore result
    
    // Test with iterator set to NULL again
    iter = NULL;
    frame = aom_codec_get_frame(&ctx, &iter);
    (void)frame;
    
    // Test aom_img_free with NULL (should be safe)
    aom_img_free(NULL);
    
    return 0;
}