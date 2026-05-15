// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_img_set_rect at vpx_image.c:176:5 in vpx_image.h
// vpx_img_flip at vpx_image.c:229:6 in vpx_image.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_img_wrap at vpx_image.c:168:14 in vpx_image.h
// vpx_img_set_rect at vpx_image.c:176:5 in vpx_image.h
// vpx_img_flip at vpx_image.c:229:6 in vpx_image.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "vpx/vpx_image.h"
#include "vpx/vpx_encoder.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static vpx_codec_ctx_t* create_test_codec_ctx(void) {
    static vpx_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));
    ctx.name = "test";
    return &ctx;
}

static vpx_image_t* create_test_image(vpx_img_fmt_t fmt, unsigned int w, unsigned int h) {
    vpx_image_t* img = vpx_img_alloc(NULL, fmt, w, h, 1);
    if (img) {
        img->cs = VPX_CS_BT_601;
        img->range = VPX_CR_STUDIO_RANGE;
    }
    return img;
}

static unsigned char* create_test_buffer(size_t size) {
    return (unsigned char*)malloc(size);
}

int LLVMFuzzerTestOneInput_10(const uint8_t *Data, size_t Size) {
    if (Size < 16) return 0;

    // Use first bytes to determine parameters
    unsigned int d_w = (Data[0] << 8) | Data[1];
    unsigned int d_h = (Data[2] << 8) | Data[3];
    unsigned int x = Data[4];
    unsigned int y = Data[5];
    unsigned int w = Data[6];
    unsigned int h = Data[7];
    vpx_img_fmt_t fmt = (Data[8] % 8) + 1; // Valid range 1-8
    unsigned int stride_align = (Data[9] % 32) + 1;
    unsigned int align = (Data[10] % 32) + 1;
    vpx_codec_pts_t pts = (Data[11] << 24) | (Data[12] << 16) | (Data[13] << 8) | Data[14];
    vpx_enc_deadline_t deadline = Data[15] % 3; // 0,1,2 for different deadlines

    // Limit dimensions to reasonable values
    d_w = d_w % 1024;
    d_h = d_h % 1024;
    w = w % 256;
    h = h % 256;
    x = x % 128;
    y = y % 128;

    // Test vpx_img_alloc
    vpx_image_t* img1 = create_test_image(fmt, d_w, d_h);
    if (img1) {
        // Test vpx_img_set_rect
        vpx_img_set_rect(img1, x, y, w, h);
        
        // Test vpx_img_flip
        vpx_img_flip(img1);
        
        // Test vpx_img_free
        vpx_img_free(img1);
    }

    // Test vpx_img_wrap
    size_t buffer_size = d_w * d_h * 3; // Conservative estimate
    unsigned char* buffer = create_test_buffer(buffer_size);
    if (buffer) {
        vpx_image_t* img2 = vpx_img_wrap(NULL, fmt, d_w, d_h, stride_align, buffer);
        if (img2) {
            vpx_img_set_rect(img2, x, y, w, h);
            vpx_img_flip(img2);
            vpx_img_free(img2);
        }
        free(buffer);
    }

    // Test vpx_codec_encode with NULL image (flush)
    vpx_codec_ctx_t* ctx = create_test_codec_ctx();
    vpx_codec_encode(ctx, NULL, pts, 1, 0, deadline);
    
    // Test vpx_codec_encode with actual image
    vpx_image_t* img3 = create_test_image(fmt, d_w, d_h);
    if (img3 && ctx) {
        vpx_codec_encode(ctx, img3, pts, 1, 0, deadline);
        vpx_img_free(img3);
    }

    return 0;
}