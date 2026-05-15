// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_img_flip at aom_image.c:285:6 in aom_image.h
// aom_img_remove_metadata at aom_image.c:412:6 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_alloc_with_border at aom_image.c:225:14 in aom_image.h
// aom_img_flip at aom_image.c:285:6 in aom_image.h
// aom_img_remove_metadata at aom_image.c:412:6 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom/aom_codec.h"
#include "aom/aom_decoder.h"
#include "aom/aom_image.h"

static aom_img_fmt_t get_random_fmt(const uint8_t *data, size_t *offset, size_t size) {
    if (*offset >= size) return AOM_IMG_FMT_I420;
    aom_img_fmt_t formats[] = {
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
    uint8_t idx = data[*offset] % (sizeof(formats) / sizeof(formats[0]));
    (*offset)++;
    return formats[idx];
}

static unsigned int get_limited_value(const uint8_t *data, size_t *offset, size_t size, 
                                      unsigned int max_val, unsigned int min_val) {
    if (*offset >= size) return min_val;
    unsigned int val = data[*offset] % (max_val + 1);
    if (val < min_val) val = min_val;
    (*offset)++;
    return val;
}

int LLVMFuzzerTestOneInput_4(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    size_t offset = 0;
    
    // Test aom_img_alloc and aom_img_free
    aom_img_fmt_t fmt1 = get_random_fmt(Data, &offset, Size);
    unsigned int d_w1 = get_limited_value(Data, &offset, Size, 256, 1);
    unsigned int d_h1 = get_limited_value(Data, &offset, Size, 256, 1);
    unsigned int align1 = get_limited_value(Data, &offset, Size, 64, 1);
    
    aom_image_t *img1 = aom_img_alloc(NULL, fmt1, d_w1, d_h1, align1);
    if (img1) {
        // Test aom_img_flip
        aom_img_flip(img1);
        
        // Test aom_img_remove_metadata (safe even if no metadata)
        aom_img_remove_metadata(img1);
        
        aom_img_free(img1);
    }
    
    // Test aom_img_alloc_with_border
    if (offset < Size) {
        aom_img_fmt_t fmt2 = get_random_fmt(Data, &offset, Size);
        unsigned int d_w2 = get_limited_value(Data, &offset, Size, 256, 1);
        unsigned int d_h2 = get_limited_value(Data, &offset, Size, 256, 1);
        unsigned int align2 = get_limited_value(Data, &offset, Size, 64, 1);
        unsigned int size_align = get_limited_value(Data, &offset, Size, 64, 1);
        unsigned int border = get_limited_value(Data, &offset, Size, 32, 0);
        
        aom_image_t *img2 = aom_img_alloc_with_border(NULL, fmt2, d_w2, d_h2, 
                                                     align2, size_align, border);
        if (img2) {
            aom_img_flip(img2);
            aom_img_remove_metadata(img2);
            aom_img_free(img2);
        }
    }
    
    // Test aom_codec_get_frame - we need a codec context
    // Create a minimal decoder context for fuzzing
    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    
    // Try to initialize decoder with dummy data
    aom_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    
    // Initialize iterator to NULL as required
    aom_codec_iter_t iter = NULL;
    
    // Call aom_codec_get_frame multiple times to iterate
    // Since we haven't decoded anything, it should return NULL
    for (int i = 0; i < 3; i++) {
        aom_image_t *frame = aom_codec_get_frame(&ctx, &iter);
        (void)frame; // Ignore result for fuzzing
    }
    
    return 0;
}