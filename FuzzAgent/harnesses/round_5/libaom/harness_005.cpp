#include <fuzzer/FuzzedDataProvider.h>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include "aom/aom_image.h"
#include "aom/aom_integer.h"

namespace harness_005 {

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
    if (size < 32) return 0; // Minimum size for initial parameters
    
    FuzzedDataProvider fdp(data, size);
    
    // Consume image format from fuzzer input
    aom_img_fmt_t fmt = static_cast<aom_img_fmt_t>(
        fdp.ConsumeIntegralInRange<uint32_t>(AOM_IMG_FMT_NONE, AOM_IMG_FMT_I44416));
    
    // Consume image dimensions with reasonable limits
    unsigned int width = fdp.ConsumeIntegralInRange<unsigned int>(1, 1024);
    unsigned int height = fdp.ConsumeIntegralInRange<unsigned int>(1, 1024);
    
    // Consume alignment parameters
    unsigned int align = fdp.ConsumeIntegralInRange<unsigned int>(1, 256);
    unsigned int stride_align = fdp.ConsumeIntegralInRange<unsigned int>(1, 256);
    unsigned int size_align = fdp.ConsumeIntegralInRange<unsigned int>(1, 256);
    unsigned int border = fdp.ConsumeIntegralInRange<unsigned int>(0, 128);
    
    // 1. Initialize image parameters and allocate image with default allocation
    aom_image_t* img1 = aom_img_alloc(nullptr, fmt, width, height, align);
    if (!img1) return 0;
    
    // 2. Allocate image with border
    aom_image_t* img2 = aom_img_alloc_with_border(nullptr, fmt, width, height, 
                                                   align, size_align, border);
    if (!img2) {
        aom_img_free(img1);
        return 0;
    }
    
    // 3. Wrap existing memory buffer as image
    // Calculate approximate buffer size needed for the image
    size_t buffer_size = width * height * 4; // Conservative estimate for RGBA
    if (fdp.remaining_bytes() < buffer_size) {
        aom_img_free(img1);
        aom_img_free(img2);
        return 0;
    }
    
    std::vector<uint8_t> buffer = fdp.ConsumeBytes<uint8_t>(buffer_size);
    aom_image_t* img3 = aom_img_wrap(nullptr, fmt, width, height, stride_align, 
                                     buffer.data());
    if (!img3) {
        aom_img_free(img1);
        aom_img_free(img2);
        return 0;
    }
    
    // 4. Use helper functions to query image properties
    int plane_width_y = aom_img_plane_width(img1, AOM_PLANE_Y);
    int plane_height_y = aom_img_plane_height(img1, AOM_PLANE_Y);
    int plane_width_u = aom_img_plane_width(img1, AOM_PLANE_U);
    int plane_height_u = aom_img_plane_height(img1, AOM_PLANE_U);
    int plane_width_v = aom_img_plane_width(img1, AOM_PLANE_V);
    int plane_height_v = aom_img_plane_height(img1, AOM_PLANE_V);
    
    // Also query properties for other images
    aom_img_plane_width(img2, AOM_PLANE_Y);
    aom_img_plane_height(img2, AOM_PLANE_Y);
    aom_img_plane_width(img3, AOM_PLANE_Y);
    aom_img_plane_height(img3, AOM_PLANE_Y);
    
    // 5. Optionally call aom_img_set_rect to modify image regions
    if (width > 2 && height > 2) {
        unsigned int rect_x = fdp.ConsumeIntegralInRange<unsigned int>(0, width - 2);
        unsigned int rect_y = fdp.ConsumeIntegralInRange<unsigned int>(0, height - 2);
        unsigned int rect_w = fdp.ConsumeIntegralInRange<unsigned int>(1, width - rect_x);
        unsigned int rect_h = fdp.ConsumeIntegralInRange<unsigned int>(1, height - rect_y);
        unsigned int rect_border = fdp.ConsumeIntegralInRange<unsigned int>(0, border);
        
        aom_img_set_rect(img1, rect_x, rect_y, rect_w, rect_h, rect_border);
        aom_img_set_rect(img2, rect_x, rect_y, rect_w, rect_h, rect_border);
    }
    
    // 6. Optionally call aom_img_flip to flip image orientation
    aom_img_flip(img1);
    aom_img_flip(img2);
    aom_img_flip(img3);
    
    // Flip back to original orientation
    aom_img_flip(img1);
    aom_img_flip(img2);
    aom_img_flip(img3);
    
    // 7. Clean up all allocated images
    aom_img_free(img1);
    aom_img_free(img2);
    aom_img_free(img3);
    
    return 0;
}

} // namespace harness_005
