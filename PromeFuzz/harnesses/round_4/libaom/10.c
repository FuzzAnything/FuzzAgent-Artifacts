// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_wrap at aom_image.c:216:14 in aom_image.h
// aom_img_flip at aom_image.c:285:6 in aom_image.h
// aom_img_set_rect at aom_image.c:234:5 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_img_flip at aom_image.c:285:6 in aom_image.h
// aom_img_set_rect at aom_image.c:234:5 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_img_flip at aom_image.c:285:6 in aom_image.h
// aom_img_set_rect at aom_image.c:234:5 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_alloc_with_border at aom_image.c:225:14 in aom_image.h
// aom_img_flip at aom_image.c:285:6 in aom_image.h
// aom_img_set_rect at aom_image.c:234:5 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom/aom_image.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint32_t read_uint32(const uint8_t **data, size_t *size) {
    if (*size < 4) {
        *size = 0;
        return 0;
    }
    uint32_t val = 0;
    for (int i = 0; i < 4; i++) {
        val = (val << 8) | (*data)[i];
    }
    *data += 4;
    *size -= 4;
    return val;
}

static uint8_t read_byte(const uint8_t **data, size_t *size) {
    if (*size == 0) {
        return 0;
    }
    uint8_t val = (*data)[0];
    (*data)++;
    (*size)--;
    return val;
}

static const aom_img_fmt_t test_formats[] = {
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
static const size_t num_formats = sizeof(test_formats) / sizeof(test_formats[0]);

int LLVMFuzzerTestOneInput_10(const uint8_t *data, size_t size) {
    if (size < 1) {
        return 0;
    }

    const uint8_t *original_data = data;
    size_t original_size = size;

    uint8_t mode = read_byte(&data, &size);
    uint32_t fmt_idx = read_byte(&data, &size) % num_formats;
    aom_img_fmt_t fmt = test_formats[fmt_idx];

    uint32_t d_w = read_uint32(&data, &size) % 0x08000000;
    uint32_t d_h = read_uint32(&data, &size) % 0x08000000;
    if (d_w == 0) d_w = 1;
    if (d_h == 0) d_h = 1;

    uint32_t align = read_uint32(&data, &size) % 65536;
    if (align == 0) align = 1;

    uint32_t size_align = read_uint32(&data, &size) % 65536;
    if (size_align == 0) size_align = 1;

    uint32_t border = read_uint32(&data, &size) % 65536;
    uint32_t x = read_uint32(&data, &size) % d_w;
    uint32_t y = read_uint32(&data, &size) % d_h;
    uint32_t w = read_uint32(&data, &size) % (d_w - x);
    uint32_t h = read_uint32(&data, &size) % (d_h - y);
    if (w == 0) w = 1;
    if (h == 0) h = 1;

    switch (mode % 4) {
        case 0: {
            aom_image_t *img = aom_img_alloc(NULL, fmt, d_w, d_h, align);
            if (img) {
                aom_img_flip(img);
                aom_img_set_rect(img, x, y, w, h, border);
                aom_img_free(img);
            }
            break;
        }
        case 1: {
            aom_image_t *img = aom_img_alloc_with_border(NULL, fmt, d_w, d_h, align, size_align, border);
            if (img) {
                aom_img_flip(img);
                aom_img_set_rect(img, x, y, w, h, border);
                aom_img_free(img);
            }
            break;
        }
        case 2: {
            size_t remaining = original_size - (data - original_data);
            if (remaining > 0) {
                size_t buffer_size = d_w * d_h * 4;
                if (buffer_size > 1024 * 1024 * 16) {
                    buffer_size = 1024 * 1024 * 16;
                }
                unsigned char *img_data = (unsigned char *)malloc(buffer_size);
                if (img_data) {
                    size_t copy_size = remaining < buffer_size ? remaining : buffer_size;
                    memcpy(img_data, data, copy_size);
                    aom_image_t *img = aom_img_wrap(NULL, fmt, d_w, d_h, align, img_data);
                    if (img) {
                        aom_img_flip(img);
                        aom_img_set_rect(img, x, y, w, h, border);
                        aom_img_free(img);
                        // For aom_img_wrap, we need to free the img_data ourselves
                        // since it's externally allocated memory
                        free(img_data);
                    } else {
                        free(img_data);
                    }
                }
            }
            break;
        }
        case 3: {
            aom_image_t img_stack;
            aom_image_t *img = aom_img_alloc(&img_stack, fmt, d_w, d_h, align);
            if (img) {
                aom_img_flip(img);
                aom_img_set_rect(img, x, y, w, h, border);
                aom_img_free(img);
            }
            break;
        }
    }

    return 0;
}