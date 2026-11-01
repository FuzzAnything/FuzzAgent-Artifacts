// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_flip at aom_image.c:285:6 in aom_image.h
// aom_img_flip at aom_image.c:285:6 in aom_image.h
// aom_img_remove_metadata at aom_image.c:412:6 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_alloc_with_border at aom_image.c:225:14 in aom_image.h
// aom_img_set_rect at aom_image.c:234:5 in aom_image.h
// aom_img_flip at aom_image.c:285:6 in aom_image.h
// aom_img_flip at aom_image.c:285:6 in aom_image.h
// aom_img_remove_metadata at aom_image.c:412:6 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_wrap at aom_image.c:216:14 in aom_image.h
// aom_img_set_rect at aom_image.c:234:5 in aom_image.h
// aom_img_flip at aom_image.c:285:6 in aom_image.h
// aom_img_flip at aom_image.c:285:6 in aom_image.h
// aom_img_remove_metadata at aom_image.c:412:6 in aom_image.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_img_set_rect at aom_image.c:234:5 in aom_image.h
// aom_img_flip at aom_image.c:285:6 in aom_image.h
// aom_img_flip at aom_image.c:285:6 in aom_image.h
// aom_img_remove_metadata at aom_image.c:412:6 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_img_set_rect at aom_image.c:234:5 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "aomdx.h"
#include "aom_encoder.h"
#include "aom_frame_buffer.h"
#include "aom_image.h"
#include "aom_decoder.h"
#include "aom.h"
#include "aom_integer.h"
#include "aom_codec.h"
#include "aomcx.h"
#include "aom_external_partition.h"

static aom_img_fmt_t get_fmt_from_data(const uint8_t *data, size_t size) {
    if (size > 0) {
        uint8_t val = data[0];
        // Map 0-10 to valid formats, others default to I420
        if (val <= 10) {
            return (aom_img_fmt_t)val;
        }
    }
    return AOM_IMG_FMT_I420;
}

static unsigned int get_safe_dim(uint32_t val) {
    // Limit dimensions to 2^27 (0x08000000) as per API constraints
    if (val == 0) return 1;
    if (val > 0x08000000) return 0x08000000;
    return val;
}

static unsigned int get_safe_align(uint32_t val) {
    // Limit alignment to 65536 as per API constraints
    if (val == 0) return 1;
    if (val > 65536) return 65536;
    return val;
}

static void init_image_metadata(aom_image_t *img) {
    if (!img) return;
    img->cp = AOM_CICP_CP_BT_709;
    img->tc = AOM_CICP_TC_BT_709;
    img->mc = AOM_CICP_MC_BT_709;
    img->range = AOM_CR_STUDIO_RANGE;
    img->csp = AOM_CSP_COLOCATED;
    img->monochrome = 0;
    img->temporal_id = 0;
    img->spatial_id = 0;
    img->user_priv = NULL;
    img->fb_priv = NULL;
}

int LLVMFuzzerTestOneInput_288(const uint8_t *Data, size_t Size) {
    if (Size < 16) {
        return 0;
    }

    // Parse input data for parameters
    uint32_t width = *(const uint32_t *)(Data);
    uint32_t height = *(const uint32_t *)(Data + 4);
    uint32_t align = *(const uint32_t *)(Data + 8);
    uint32_t border = *(const uint32_t *)(Data + 12);

    aom_img_fmt_t fmt = get_fmt_from_data(Data + 16, Size > 16 ? Size - 16 : 0);
    
    // Ensure dimensions are within valid ranges
    width = get_safe_dim(width);
    height = get_safe_dim(height);
    align = get_safe_align(align);
    border = get_safe_align(border);

    // Test aom_img_alloc
    aom_image_t img_alloc;
    aom_image_t *img_ptr = aom_img_alloc(&img_alloc, fmt, width, height, align);
    
    if (img_ptr) {
        init_image_metadata(img_ptr);
        
        // Test aom_img_set_rect
        uint32_t rect_x = (Size > 20) ? *(const uint32_t *)(Data + 20) : 0;
        uint32_t rect_y = (Size > 24) ? *(const uint32_t *)(Data + 24) : 0;
        uint32_t rect_w = (Size > 28) ? *(const uint32_t *)(Data + 28) : width;
        uint32_t rect_h = (Size > 32) ? *(const uint32_t *)(Data + 32) : height;
        
        // Clamp rect to image bounds to avoid immediate failure, but allow fuzzing edge cases
        if (rect_x > width) rect_x = 0;
        if (rect_y > height) rect_y = 0;
        if (rect_w > width - rect_x) rect_w = width - rect_x;
        if (rect_h > height - rect_y) rect_h = height - rect_y;
        
        aom_img_set_rect(img_ptr, rect_x, rect_y, rect_w, rect_h, border);

        // Test aom_img_flip (call twice to restore state)
        aom_img_flip(img_ptr);
        aom_img_flip(img_ptr);

        // Test aom_img_remove_metadata
        aom_img_remove_metadata(img_ptr);

        // Free allocated image
        aom_img_free(img_ptr);
    }

    // Test aom_img_alloc_with_border
    aom_image_t img_border;
    aom_image_t *img_border_ptr = aom_img_alloc_with_border(
        &img_border, fmt, width, height, align, align, border
    );

    if (img_border_ptr) {
        init_image_metadata(img_border_ptr);
        
        // Test aom_img_set_rect on border image
        aom_img_set_rect(img_border_ptr, 0, 0, width, height, border);
        
        // Test aom_img_flip
        aom_img_flip(img_border_ptr);
        aom_img_flip(img_border_ptr);

        aom_img_remove_metadata(img_border_ptr);
        aom_img_free(img_border_ptr);
    }

    // Test aom_img_wrap
    // Calculate required size for wrapping
    // We use a dummy buffer size calculation based on format
    // Note: Actual calculation is complex, using a safe over-estimate for fuzzing
    size_t max_stride = (width + align - 1) & ~(align - 1);
    size_t max_height = (height + align - 1) & ~(align - 1);
    size_t buffer_size = max_stride * max_height * 4; // 4 bytes max for high bit depth
    
    // Ensure buffer_size is not too large to avoid allocation failure
    if (buffer_size > 1024 * 1024 * 64) { // 64MB limit for fuzzing
        buffer_size = 1024 * 1024 * 64;
    }
    
    unsigned char *img_data = (unsigned char *)malloc(buffer_size);
    if (img_data) {
        memset(img_data, 0, buffer_size);
        
        aom_image_t img_wrap;
        aom_image_t *img_wrap_ptr = aom_img_wrap(&img_wrap, fmt, width, height, align, img_data);
        
        if (img_wrap_ptr) {
            init_image_metadata(img_wrap_ptr);
            
            // Test aom_img_set_rect
            aom_img_set_rect(img_wrap_ptr, 0, 0, width, height, border);
            
            // Test aom_img_flip
            aom_img_flip(img_wrap_ptr);
            aom_img_flip(img_wrap_ptr);
            
            aom_img_remove_metadata(img_wrap_ptr);
            
            // Note: Do not call aom_img_free on wrapped image if we allocated the struct manually
            // but we passed &img_wrap, so we should not free the struct itself, only the data
            // The API docs say: "If the img parameter is non-null, the value of the img parameter will be returned."
            // aom_img_free will free the struct if self_allocd is set, but here we passed a stack struct.
            // However, aom_img_free checks img_data_owner. Since we provided img_data, it won't free img_data.
            // It might try to free the struct if it thinks it owns it.
            // To be safe with stack allocation, we just don't call aom_img_free on the stack struct.
            // The memory img_data is freed below.
        }
        
        free(img_data);
    }

    // Test with NULL img pointer for heap allocation of descriptor
    aom_image_t *img_heap = aom_img_alloc(NULL, fmt, width, height, align);
    if (img_heap) {
        init_image_metadata(img_heap);
        aom_img_set_rect(img_heap, 0, 0, width, height, border);
        aom_img_flip(img_heap);
        aom_img_flip(img_heap);
        aom_img_remove_metadata(img_heap);
        aom_img_free(img_heap);
    }

    return 0;
}