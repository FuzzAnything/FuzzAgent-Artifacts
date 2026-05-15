// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_img_plane_width at aom_image.c:313:5 in aom_image.h
// aom_img_plane_height at aom_image.c:320:5 in aom_image.h
// aom_img_plane_width at aom_image.c:313:5 in aom_image.h
// aom_img_plane_height at aom_image.c:320:5 in aom_image.h
// aom_img_flip at aom_image.c:285:6 in aom_image.h
// aom_img_set_rect at aom_image.c:234:5 in aom_image.h
// aom_codec_av1_dx at av1_dx_iface.c:1796:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom_image.h"
#include "aom_decoder.h"
#include "aomdx.h"
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

static aom_image_t* create_test_image(const uint8_t *data, size_t size) {
    if (size < 4) return NULL;
    
    unsigned int fmt_idx = data[0] % (sizeof(formats) / sizeof(formats[0]));
    unsigned int d_w = (data[1] % 128) + 1;
    unsigned int d_h = (data[2] % 128) + 1;
    unsigned int align = (data[3] % 32) + 1;
    
    aom_image_t *img = aom_img_alloc(NULL, formats[fmt_idx], d_w, d_h, align);
    if (!img) return NULL;
    
    // Fill image planes with some data
    size_t offset = 4;
    for (int plane = 0; plane < 3; ++plane) {
        if (img->planes[plane]) {
            int stride = img->stride[plane];
            int plane_width = aom_img_plane_width(img, plane);
            int plane_height = aom_img_plane_height(img, plane);
            
            for (int y = 0; y < plane_height; ++y) {
                for (int x = 0; x < plane_width; ++x) {
                    if (offset < size) {
                        img->planes[plane][y * stride + x] = data[offset++];
                    } else {
                        img->planes[plane][y * stride + x] = (plane + x + y) % 256;
                    }
                }
            }
        }
    }
    
    return img;
}

static void test_image_functions(aom_image_t *img, const uint8_t *data, size_t size) {
    if (!img) return;
    
    // Test aom_img_plane_width for all planes
    for (int plane = 0; plane < 3; ++plane) {
        aom_img_plane_width(img, plane);
    }
    
    // Test aom_img_plane_height for all planes
    for (int plane = 0; plane < 3; ++plane) {
        aom_img_plane_height(img, plane);
    }
    
    // Test aom_img_flip
    aom_img_flip(img);
    
    // Test aom_img_set_rect with various parameters
    if (size >= 8) {
        unsigned int x = data[0] % (img->d_w + 1);
        unsigned int y = data[1] % (img->d_h + 1);
        unsigned int w = data[2] % (img->d_w - x + 1);
        unsigned int h = data[3] % (img->d_h - y + 1);
        unsigned int border = data[4] % 16;
        
        aom_img_set_rect(img, x, y, w, h, border);
    }
}

static void test_decoder_functions(const uint8_t *data, size_t size) {
    aom_codec_iface_t *iface = aom_codec_av1_dx();
    if (!iface) return;
    
    aom_codec_ctx_t ctx;
    aom_codec_dec_cfg_t cfg = {0};
    
    // Initialize decoder with default configuration
    if (aom_codec_dec_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return;
    }
    
    // Try to decode the input data
    aom_codec_decode(&ctx, data, size, NULL);
    
    // Test aom_codec_get_frame iterator
    aom_codec_iter_t iter = NULL;
    aom_image_t *frame;
    while ((frame = aom_codec_get_frame(&ctx, &iter)) != NULL) {
        // Do nothing with the frame, just iterate
    }
    
    aom_codec_destroy(&ctx);
}

int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Test image functions
    aom_image_t *img = create_test_image(Data, Size);
    if (img) {
        test_image_functions(img, Data, Size);
        aom_img_free(img);
    }
    
    // Test decoder functions
    test_decoder_functions(Data, Size);
    
    return 0;
}