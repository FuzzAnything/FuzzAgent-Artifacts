// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_flip at aom_image.c:285:6 in aom_image.h
// aom_img_set_rect at aom_image.c:234:5 in aom_image.h
// aom_img_remove_metadata at aom_image.c:412:6 in aom_image.h
// aom_img_wrap at aom_image.c:216:14 in aom_image.h
// aom_img_remove_metadata at aom_image.c:412:6 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_wrap at aom_image.c:216:14 in aom_image.h
// aom_img_plane_width at aom_image.c:313:5 in aom_image.h
// aom_img_plane_height at aom_image.c:320:5 in aom_image.h
// aom_img_flip at aom_image.c:285:6 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aom_codec.h"
#include "aom/aomdx.h"
#include "aom/aom_integer.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_image.h"
#include "aom/aom.h"
#include "aom/aomcx.h"

/* Helper to calculate buffer size for aom_img_wrap */
static size_t calculate_wrap_size(aom_img_fmt_t fmt, unsigned int d_w, unsigned int d_h, unsigned int stride_align) {
    aom_image_t img;
    unsigned char *dummy = (unsigned char *)1;
    /* First call to get format properties */
    if (aom_img_wrap(&img, fmt, d_w, d_h, stride_align, dummy) == NULL) {
        return 0;
    }
    
    /* Calculate size based on the logic described in aom_img_wrap docs */
    int align = (1 << img.x_chroma_shift) - 1;
    int w = (d_w + align) & ~align;
    align = (1 << img.y_chroma_shift) - 1;
    int h = (d_h + align) & ~align;
    
    uint64_t s = (fmt & AOM_IMG_FMT_PLANAR) ? w : (uint64_t)img.bps * w / 8;
    s = (fmt & AOM_IMG_FMT_HIGHBITDEPTH) ? s * 2 : s;
    s = (s + stride_align - 1) & ~((uint64_t)stride_align - 1);
    s = (fmt & AOM_IMG_FMT_HIGHBITDEPTH) ? s / 2 : s;
    
    uint64_t alloc_size = (fmt & AOM_IMG_FMT_PLANAR) ? (uint64_t)h * s * img.bps / 8 : (uint64_t)h * s;
    
    return (size_t)alloc_size;
}

static void cleanup_image(aom_image_t *img) {
    if (img) {
        aom_img_remove_metadata(img);
        if (img->img_data_owner) {
            aom_img_free(img);
        }
    }
}

int LLVMFuzzerTestOneInput_53(const uint8_t *Data, size_t Size) {
    /* Ensure we have enough data for all required indices */
    if (Size < 13) return 0;

    /* Parse input for parameters */
    unsigned int d_w = Data[0] + 1;
    unsigned int d_h = Data[1] + 1;
    unsigned int stride_align = (Data[2] % 64) + 1;
    aom_img_fmt_t fmt = (aom_img_fmt_t)(Data[3] % 14); /* 14 is approx max enum value */
    
    /* Ensure valid dimensions to avoid immediate crashes in wrap */
    if (d_w > 0x08000000 || d_h > 0x08000000 || stride_align > 65536) {
        return 0;
    }

    /* Allocate buffer for image data */
    size_t buf_size = calculate_wrap_size(fmt, d_w, d_h, stride_align);
    if (buf_size == 0) return 0;
    
    unsigned char *img_data = (unsigned char *)malloc(buf_size);
    if (!img_data) return 0;
    
    /* Initialize buffer with fuzz data */
    memset(img_data, 0, buf_size);
    size_t copy_len = (Size - 4) < buf_size ? (Size - 4) : buf_size;
    memcpy(img_data, Data + 4, copy_len);

    aom_image_t img;
    aom_image_t *img_ptr = aom_img_wrap(&img, fmt, d_w, d_h, stride_align, img_data);
    
    if (!img_ptr) {
        free(img_data);
        return 0;
    }

    /* Fuzz aom_img_plane_width */
    int plane_idx_w = Data[4] % 3;
    int w = aom_img_plane_width(img_ptr, plane_idx_w);
    (void)w;

    /* Fuzz aom_img_plane_height */
    int plane_idx_h = Data[5] % 3;
    int h = aom_img_plane_height(img_ptr, plane_idx_h);
    (void)h;

    /* Fuzz aom_img_flip */
    if (Data[6] & 1) {
        aom_img_flip(img_ptr);
    }
    if (Data[7] & 1) {
        aom_img_flip(img_ptr);
    }

    /* Fuzz aom_img_set_rect */
    /* Safe bounds checking for x, y, rect_w, rect_h */
    unsigned int x = 0;
    unsigned int y = 0;
    unsigned int rect_w = 0;
    unsigned int rect_h = 0;
    unsigned int border = Data[12] % 10;

    if (d_w > 0) x = Data[8] % d_w;
    if (d_h > 0) y = Data[9] % d_h;
    
    /* Ensure rect_w and rect_h do not exceed remaining dimensions */
    unsigned int max_w = d_w - x;
    unsigned int max_h = d_h - y;
    
    if (max_w > 0) rect_w = (Data[10] % max_w) + 1;
    else rect_w = 1; /* Fallback if max_w is 0 */

    if (max_h > 0) rect_h = (Data[11] % max_h) + 1;
    else rect_h = 1; /* Fallback if max_h is 0 */

    int ret_rect = aom_img_set_rect(img_ptr, x, y, rect_w, rect_h, border);
    (void)ret_rect;

    /* Fuzz aom_img_remove_metadata */
    aom_img_remove_metadata(img_ptr);

    /* Cleanup */
    cleanup_image(img_ptr);
    free(img_data);

    return 0;
}