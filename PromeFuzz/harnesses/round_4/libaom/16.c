// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_alloc_with_border at aom_image.c:225:14 in aom_image.h
// aom_img_set_rect at aom_image.c:234:5 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_wrap at aom_image.c:216:14 in aom_image.h
// aom_img_set_rect at aom_image.c:234:5 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_img_flip at aom_image.c:285:6 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_wrap at aom_image.c:216:14 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_alloc_with_border at aom_image.c:225:14 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_img_set_rect at aom_image.c:234:5 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom/aom_image.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint32_t extract_uint32(const uint8_t **data, size_t *size) {
    if (*size < 4) {
        *size = 0;
        return 0;
    }
    uint32_t val = 0;
    for (int i = 0; i < 4; ++i) {
        val = (val << 8) | (*data)[i];
    }
    *data += 4;
    *size -= 4;
    return val;
}

static aom_img_fmt_t get_format_from_input(uint32_t fmt_val) {
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
    return formats[fmt_val % (sizeof(formats) / sizeof(formats[0]))];
}

int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    const uint8_t *input = Data;
    size_t remaining = Size;

    uint32_t test_case = extract_uint32(&input, &remaining) % 7;
    uint32_t width = extract_uint32(&input, &remaining) % 4096 + 1;
    uint32_t height = extract_uint32(&input, &remaining) % 4096 + 1;
    uint32_t align = (extract_uint32(&input, &remaining) % 256) + 1;
    uint32_t border = extract_uint32(&input, &remaining) % 256;
    uint32_t size_align = (extract_uint32(&input, &remaining) % 256) + 1;
    uint32_t x = extract_uint32(&input, &remaining) % width;
    uint32_t y = extract_uint32(&input, &remaining) % height;
    uint32_t rect_w = extract_uint32(&input, &remaining) % (width - x) + 1;
    uint32_t rect_h = extract_uint32(&input, &remaining) % (height - y) + 1;
    aom_img_fmt_t fmt = get_format_from_input(extract_uint32(&input, &remaining));

    switch (test_case) {
        case 0: {
            aom_image_t *img = aom_img_alloc(NULL, fmt, width, height, align);
            if (img) {
                aom_img_flip(img);
                aom_img_free(img);
            }
            break;
        }
        case 1: {
            aom_image_t img_on_stack;
            aom_image_t *img = aom_img_alloc(&img_on_stack, fmt, width, height, align);
            if (img) {
                aom_img_free(img);
            }
            break;
        }
        case 2: {
            size_t buffer_size = width * height * 4;
            unsigned char *img_data = malloc(buffer_size);
            if (img_data) {
                aom_image_t *img = aom_img_wrap(NULL, fmt, width, height, align, img_data);
                if (img) {
                    aom_img_free(img);
                }
                free(img_data);
            }
            break;
        }
        case 3: {
            aom_image_t *img = aom_img_alloc_with_border(NULL, fmt, width, height, align, size_align, border);
            if (img) {
                aom_img_free(img);
            }
            break;
        }
        case 4: {
            aom_image_t *img = aom_img_alloc(NULL, fmt, width, height, align);
            if (img) {
                aom_img_set_rect(img, x, y, rect_w, rect_h, border);
                aom_img_free(img);
            }
            break;
        }
        case 5: {
            aom_image_t *img = aom_img_alloc_with_border(NULL, fmt, width, height, align, size_align, border);
            if (img) {
                aom_img_set_rect(img, x, y, rect_w, rect_h, border);
                aom_img_free(img);
            }
            break;
        }
        case 6: {
            size_t buffer_size = width * height * 4;
            unsigned char *img_data = malloc(buffer_size);
            if (img_data) {
                aom_image_t *img = aom_img_wrap(NULL, fmt, width, height, align, img_data);
                if (img) {
                    aom_img_set_rect(img, x, y, rect_w, rect_h, border);
                    aom_img_free(img);
                }
                free(img_data);
            }
            break;
        }
    }

    return 0;
}