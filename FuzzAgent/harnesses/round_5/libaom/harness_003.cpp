#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <algorithm>
#include <vector>
#include "fuzzer/FuzzedDataProvider.h"
#include "aom/aom_image.h"

namespace harness_003 {
/*
 * Fuzzing harness for libaom AV1 image metadata manipulation APIs
 * Targets metadata functions: aom_img_add_metadata, aom_img_get_metadata, 
 * aom_img_num_metadata, aom_img_remove_metadata, aom_img_metadata_alloc, 
 * aom_img_metadata_free, aom_img_alloc, aom_img_free
 * Follows coverage guidance for comprehensive metadata testing
 */



int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  // Minimum size needed for meaningful metadata testing
  // Need: image params (format, width, height, align) + metadata params (type, size, flag) + some payload
  if (size < 32) {
    return 0;
  }

  FuzzedDataProvider fdp(data, size);

  // Consume image parameters from fuzzer input
  // Use limited range for width/height to keep it lightweight
  uint32_t width = fdp.ConsumeIntegralInRange<uint32_t>(16, 128);
  uint32_t height = fdp.ConsumeIntegralInRange<uint32_t>(16, 128);
  uint32_t align = fdp.ConsumeIntegralInRange<uint32_t>(1, 32);
  
  // Image format - pick from common formats
  aom_img_fmt_t fmt = static_cast<aom_img_fmt_t>(fdp.ConsumeIntegralInRange<uint32_t>(
      AOM_IMG_FMT_I420, AOM_IMG_FMT_I444));

  // 1. Call aom_img_alloc to create an image object
  aom_image_t image;
  aom_image_t *img_ptr = aom_img_alloc(&image, fmt, width, height, align);
  if (!img_ptr) {
    return 0;  // Allocation failed
  }

  // Consume metadata parameters
  uint32_t metadata_type = fdp.ConsumeIntegral<uint32_t>();
  uint32_t insert_flag = fdp.ConsumeIntegralInRange<uint32_t>(
      AOM_MIF_NON_KEY_FRAME, AOM_MIF_ANY_FRAME_LAYER_SPECIFIC);
  
  // Metadata payload size - limited for fuzzing efficiency
  size_t metadata_size = fdp.ConsumeIntegralInRange<size_t>(1, 256);
  
  // Check if we have enough data for metadata payload
  if (fdp.remaining_bytes() < metadata_size) {
    aom_img_free(&image);
    return 0;
  }

  // Get metadata payload data
  std::vector<uint8_t> metadata_data = fdp.ConsumeBytes<uint8_t>(metadata_size);
  const uint8_t *metadata_payload = metadata_data.data();

  // 2. Call aom_img_metadata_alloc to allocate metadata structure
  aom_metadata_t *metadata = aom_img_metadata_alloc(
      metadata_type, metadata_payload, metadata_size, 
      static_cast<aom_metadata_insert_flags_t>(insert_flag));
  
  if (!metadata) {
    // Allocation failed, but we can still test other APIs
    aom_img_free(&image);
    return 0;
  }

  // 3. Call aom_img_add_metadata to add metadata to the image
  int add_result = aom_img_add_metadata(&image, metadata_type, metadata_payload, 
                                        metadata_size, 
                                        static_cast<aom_metadata_insert_flags_t>(insert_flag));
  
  // Test if addition was successful
  if (add_result == 0) {
    // 4. Call aom_img_num_metadata to verify metadata count
    size_t num_metadata = aom_img_num_metadata(&image);
    
    // 5. Call aom_img_get_metadata to retrieve and verify metadata
    if (num_metadata > 0) {
      const aom_metadata_t *retrieved_metadata = aom_img_get_metadata(&image, 0);
      // Validate retrieved metadata if possible
      if (retrieved_metadata) {
        // Could compare type, size, etc.
      }
    }
    
    // Test with out-of-bounds index
    const aom_metadata_t *invalid_metadata = aom_img_get_metadata(&image, 999);
    
    // 6. Call aom_img_remove_metadata to remove metadata
    aom_img_remove_metadata(&image);
    
    // Verify metadata is removed
    size_t after_remove_count = aom_img_num_metadata(&image);
  } else {
    // Test error handling paths
    // Try adding with invalid parameters
    aom_img_add_metadata(&image, metadata_type, nullptr, metadata_size, 
                        static_cast<aom_metadata_insert_flags_t>(insert_flag));
    aom_img_add_metadata(&image, metadata_type, metadata_payload, 0, 
                        static_cast<aom_metadata_insert_flags_t>(insert_flag));
  }
  
  // 7. Call aom_img_metadata_free to free metadata structure
  aom_img_metadata_free(metadata);
  
  // Test edge cases with additional metadata if we have enough input
  if (fdp.remaining_bytes() > 32) {
    uint32_t second_type = fdp.ConsumeIntegral<uint32_t>();
    uint32_t second_flag = fdp.ConsumeIntegralInRange<uint32_t>(
        AOM_MIF_NON_KEY_FRAME, AOM_MIF_ANY_FRAME_LAYER_SPECIFIC);
    size_t second_size = fdp.ConsumeIntegralInRange<size_t>(1, 128);
    
    if (fdp.remaining_bytes() >= second_size) {
      std::vector<uint8_t> second_data = fdp.ConsumeBytes<uint8_t>(second_size);
      
      // Create second metadata
      aom_metadata_t *second_metadata = aom_img_metadata_alloc(
          second_type, second_data.data(), second_size, 
          static_cast<aom_metadata_insert_flags_t>(second_flag));
      
      if (second_metadata) {
        // Add and test second metadata
        aom_img_add_metadata(&image, second_type, second_data.data(), 
                            second_size, 
                            static_cast<aom_metadata_insert_flags_t>(second_flag));
        
        size_t new_count = aom_img_num_metadata(&image);
        
        // Get the second metadata
        if (new_count > 1) {
          const aom_metadata_t *second_retrieved = aom_img_get_metadata(&image, 1);
        }
        
        // Remove all metadata
        aom_img_remove_metadata(&image);
        
        // Free second metadata
        aom_img_metadata_free(second_metadata);
      }
    }
  }

  // 8. Call aom_img_free to clean up image
  aom_img_free(&image);

  return 0;

  return 0;
}

} // namespace harness_003
