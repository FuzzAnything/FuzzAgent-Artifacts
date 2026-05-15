// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_uleb_size_in_bytes at aom_integer.c:23:8 in aom_integer.h
// aom_uleb_encode at aom_integer.c:58:5 in aom_integer.h
// aom_uleb_encode_fixed_size at aom_integer.c:79:5 in aom_integer.h
// aom_uleb_decode at aom_integer.c:31:5 in aom_integer.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_img_add_metadata at aom_image.c:380:5 in aom_image.h
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aom_integer.h"
#include "aom/aom_image.h"

int LLVMFuzzerTestOneInput_6(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Seed for deterministic random-like behavior based on input
    uint64_t seed = 0;
    for (size_t i = 0; i < (Size < 8 ? Size : 8); ++i) {
        seed = (seed << 8) | Data[i];
    }

    // Test aom_uleb_size_in_bytes
    uint64_t value = seed;
    size_t size_bytes = aom_uleb_size_in_bytes(value);
    (void)size_bytes; // Suppress unused variable warning

    // Test aom_uleb_encode
    uint8_t encode_buffer[16];
    size_t coded_size = 0;
    int encode_result = aom_uleb_encode(value, sizeof(encode_buffer), encode_buffer, &coded_size);
    (void)encode_result; // Suppress unused variable warning

    // Test aom_uleb_encode_fixed_size
    size_t pad_to_size = (seed % 10) + 1; // 1-10 bytes
    size_t fixed_coded_size = 0;
    uint8_t fixed_encode_buffer[16];
    int fixed_encode_result = aom_uleb_encode_fixed_size(
        value, sizeof(fixed_encode_buffer), pad_to_size, fixed_encode_buffer, &fixed_coded_size);
    (void)fixed_encode_result; // Suppress unused variable warning

    // Test aom_uleb_decode
    uint64_t decoded_value = 0;
    size_t decoded_length = 0;
    int decode_result = -1;
    if (Size > 0) {
        // Use the input data as buffer for decoding
        decode_result = aom_uleb_decode(Data, Size, &decoded_value, &decoded_length);
    }
    (void)decode_result; // Suppress unused variable warning

    // Test aom_img_add_metadata and aom_img_num_metadata
    // Create image using proper allocation function
    aom_image_t *img = aom_img_alloc(NULL, AOM_IMG_FMT_I420, 64, 64, 0);
    if (!img) return 0;
    
    // Initialize additional fields
    img->cp = AOM_CICP_CP_BT_709;
    img->tc = AOM_CICP_TC_BT_709;
    img->mc = AOM_CICP_MC_BT_709;
    img->csp = AOM_CSP_VERTICAL;
    img->range = AOM_CR_STUDIO_RANGE;
    img->temporal_id = 0;
    img->spatial_id = 0;
    
    // Test metadata addition with various flags
    uint32_t metadata_type = seed & 0xFF;
    size_t metadata_sz = Size > 1024 ? 1024 : Size;
    const uint8_t* metadata_data = Size > 0 ? Data : NULL;
    
    // Try different insert flags
    aom_metadata_insert_flags_t flags[] = {
        AOM_MIF_NON_KEY_FRAME,
        AOM_MIF_KEY_FRAME,
        AOM_MIF_ANY_FRAME,
        AOM_MIF_NON_KEY_FRAME_LAYER_SPECIFIC,
        AOM_MIF_KEY_FRAME_LAYER_SPECIFIC,
        AOM_MIF_ANY_FRAME_LAYER_SPECIFIC
    };
    
    for (int i = 0; i < 6; i++) {
        if (metadata_sz > 0 && metadata_data != NULL) {
            int add_result = aom_img_add_metadata(img, metadata_type + i, 
                                                 metadata_data, metadata_sz, flags[i]);
            (void)add_result; // Ignore result for fuzzing
        }
    }
    
    // Test getting metadata count
    size_t metadata_count = aom_img_num_metadata(img);
    (void)metadata_count; // Suppress unused variable warning
    
    // Clean up using proper deallocation function
    aom_img_free(img);
    
    return 0;
}