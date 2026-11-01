// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_plane_width at aom_image.c:313:5 in aom_image.h
// aom_img_plane_width at aom_image.c:313:5 in aom_image.h
// aom_img_plane_height at aom_image.c:320:5 in aom_image.h
// aom_img_plane_height at aom_image.c:320:5 in aom_image.h
// aom_img_set_rect at aom_image.c:234:5 in aom_image.h
// aom_img_flip at aom_image.c:285:6 in aom_image.h
// aom_img_flip at aom_image.c:285:6 in aom_image.h
// aom_img_remove_metadata at aom_image.c:412:6 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aom_codec.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_external_partition.h"
#include "aom/aomcx.h"
#include "aom/aom_image.h"
#include "aom/aomdx.h"
#include "aom/aom_integer.h"
#include "aom/aom.h"

static int fuzz_image_operations(const uint8_t *Data, size_t Size) {
    if (Size < 8) return 0;

    // Parse input to determine image properties
    uint32_t width = (Data[0] << 8) | Data[1];
    uint32_t height = (Data[2] << 8) | Data[3];
    uint8_t fmt_idx = Data[4] % 10; // Limit to first 10 formats
    uint8_t bit_depth = (Data[5] % 5) + 8; // 8, 10, 12, 14, 16
    uint8_t plane_idx = Data[6] % 3;
    uint8_t x = Data[7];

    // Bounds checking to prevent allocation failures or overflows
    if (width == 0 || height == 0) return 0;
    if (width > 4096 || height > 4096) return 0;

    // Map format index to enum
    aom_img_fmt_t fmts[] = {
        AOM_IMG_FMT_NONE, AOM_IMG_FMT_YV12, AOM_IMG_FMT_I420, AOM_IMG_FMT_AOMYV12,
        AOM_IMG_FMT_AOMI420, AOM_IMG_FMT_I422, AOM_IMG_FMT_I444, AOM_IMG_FMT_NV12,
        AOM_IMG_FMT_I42016, AOM_IMG_FMT_YV1216
    };
    
    aom_img_fmt_t fmt = fmts[fmt_idx];
    if (fmt == AOM_IMG_FMT_NONE) return 0;

    // Initialize image
    aom_image_t img;
    memset(&img, 0, sizeof(img));
    img.w = width;
    img.h = height;
    img.bit_depth = bit_depth;
    img.d_w = width;
    img.d_h = height;
    img.r_w = width;
    img.r_h = height;
    img.fmt = fmt;
    img.monochrome = 0;
    img.temporal_id = 0;
    img.spatial_id = 0;
    img.user_priv = NULL;
    img.fb_priv = NULL;
    img.img_data_owner = 0;
    img.self_allocd = 0;
    img.metadata = NULL;

    // Set chroma shifts based on format (simplified logic)
    if (fmt == AOM_IMG_FMT_I444 || fmt == AOM_IMG_FMT_I44416) {
        img.x_chroma_shift = 0;
        img.y_chroma_shift = 0;
    } else if (fmt == AOM_IMG_FMT_I422 || fmt == AOM_IMG_FMT_I42216) {
        img.x_chroma_shift = 1;
        img.y_chroma_shift = 0;
    } else {
        img.x_chroma_shift = 1;
        img.y_chroma_shift = 1;
    }

    // Allocate planes
    size_t bytes_per_sample = (bit_depth > 8) ? 2 : 1;
    size_t stride_y = width * bytes_per_sample;
    size_t stride_uv = stride_y;
    if (fmt == AOM_IMG_FMT_NV12 || fmt == AOM_IMG_FMT_NV12) {
        stride_uv = stride_y; // NV12 interleaved
    } else {
        stride_uv = (width >> img.x_chroma_shift) * bytes_per_sample;
    }

    img.planes[0] = malloc(height * stride_y);
    if (!img.planes[0]) return 0;
    img.stride[0] = stride_y;

    if (fmt != AOM_IMG_FMT_NONE && fmt != AOM_IMG_FMT_NV12) {
        img.planes[1] = malloc((height >> img.y_chroma_shift) * stride_uv);
        img.planes[2] = malloc((height >> img.y_chroma_shift) * stride_uv);
        if (!img.planes[1] || !img.planes[2]) {
            free(img.planes[1]);
            free(img.planes[2]);
            free(img.planes[0]);
            return 0;
        }
        img.stride[1] = stride_uv;
        img.stride[2] = stride_uv;
    } else if (fmt == AOM_IMG_FMT_NV12) {
        img.planes[1] = malloc((height >> img.y_chroma_shift) * stride_uv);
        img.planes[2] = NULL;
        if (!img.planes[1]) {
            free(img.planes[1]);
            free(img.planes[0]);
            return 0;
        }
        img.stride[1] = stride_uv;
        img.stride[2] = 0;
    }

    img.sz = (height * stride_y) + (img.planes[1] ? (height >> img.y_chroma_shift) * stride_uv : 0) + (img.planes[2] ? (height >> img.y_chroma_shift) * stride_uv : 0);
    img.bps = bit_depth;

    // 1. Test aom_img_plane_width
    int w0 = aom_img_plane_width(&img, 0);
    int w1 = aom_img_plane_width(&img, plane_idx);
    (void)w0;
    (void)w1;

    // 2. Test aom_img_plane_height
    int h0 = aom_img_plane_height(&img, 0);
    int h1 = aom_img_plane_height(&img, plane_idx);
    (void)h0;
    (void)h1;

    // 3. Test aom_img_set_rect
    unsigned int rect_x = x % (width > 0 ? width : 1);
    unsigned int rect_y = (Data[8] % 255) % (height > 0 ? height : 1);
    unsigned int rect_w = (Data[9] % 255) % (width - rect_x + 1);
    unsigned int rect_h = (Data[10] % 255) % (height - rect_y + 1);
    unsigned int border = Data[11] % 4;

    int ret_rect = aom_img_set_rect(&img, rect_x, rect_y, rect_w, rect_h, border);
    (void)ret_rect;

    // 4. Test aom_img_flip
    aom_img_flip(&img);
    aom_img_flip(&img); // Flip back

    // 5. Test aom_img_remove_metadata (safe even if NULL)
    aom_img_remove_metadata(&img);

    // Cleanup
    aom_img_free(&img);

    return 0;
}

int LLVMFuzzerTestOneInput_223(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Run the main fuzzing logic
    fuzz_image_operations(Data, Size);

    return 0;
}