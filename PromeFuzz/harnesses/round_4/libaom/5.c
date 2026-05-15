// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_uleb_encode_fixed_size at aom_integer.c:79:5 in aom_integer.h
// aom_uleb_decode at aom_integer.c:31:5 in aom_integer.h
// aom_uleb_decode at aom_integer.c:31:5 in aom_integer.h
// aom_uleb_decode at aom_integer.c:31:5 in aom_integer.h
// aom_uleb_decode at aom_integer.c:31:5 in aom_integer.h
// aom_img_add_metadata at aom_image.c:380:5 in aom_image.h
// aom_img_add_metadata at aom_image.c:380:5 in aom_image.h
// aom_img_add_metadata at aom_image.c:380:5 in aom_image.h
// aom_uleb_encode at aom_integer.c:58:5 in aom_integer.h
// aom_uleb_encode at aom_integer.c:58:5 in aom_integer.h
// aom_uleb_encode at aom_integer.c:58:5 in aom_integer.h
// aom_img_add_metadata at aom_image.c:380:5 in aom_image.h
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
// aom_uleb_size_in_bytes at aom_integer.c:23:8 in aom_integer.h
// aom_uleb_encode at aom_integer.c:58:5 in aom_integer.h
// aom_uleb_decode at aom_integer.c:31:5 in aom_integer.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aom_integer.h"
#include "aom/aom_image.h"

#ifdef __cplusplus
extern "C" {
#endif

static void dummy_cleanup(void) {
    // Clean up any global state if needed
}

int LLVMFuzzerTestOneInput_5(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Initialize a dummy aom_image_t structure for metadata functions
    static aom_image_t dummy_img;
    memset(&dummy_img, 0, sizeof(dummy_img));
    
    // Set some basic fields to make it look valid
    dummy_img.fmt = AOM_IMG_FMT_I420;
    dummy_img.w = 64;
    dummy_img.h = 64;
    dummy_img.d_w = 64;
    dummy_img.d_h = 64;
    dummy_img.bit_depth = 8;
    dummy_img.x_chroma_shift = 1;
    dummy_img.y_chroma_shift = 1;
    dummy_img.metadata = NULL;
    
    // Test aom_img_add_metadata
    if (Size >= 5) {
        // Use first 4 bytes for type, rest for data
        uint32_t type = *(const uint32_t *)Data;
        size_t data_size = Size - 4;
        const uint8_t *metadata_data = Data + 4;
        
        // Try different insert flags
        aom_metadata_insert_flags_t flags[] = {
            AOM_MIF_NON_KEY_FRAME,
            AOM_MIF_KEY_FRAME,
            AOM_MIF_ANY_FRAME,
            AOM_MIF_NON_KEY_FRAME_LAYER_SPECIFIC,
            AOM_MIF_KEY_FRAME_LAYER_SPECIFIC,
            AOM_MIF_ANY_FRAME_LAYER_SPECIFIC
        };
        
        for (size_t i = 0; i < sizeof(flags)/sizeof(flags[0]); i++) {
            int result = aom_img_add_metadata(&dummy_img, type, metadata_data, 
                                            data_size, flags[i]);
            (void)result; // Ignore return value for fuzzing
        }
    }
    
    // Test aom_img_num_metadata
    size_t num_metadata = aom_img_num_metadata(&dummy_img);
    (void)num_metadata;
    
    // Test LEB128 functions
    if (Size >= sizeof(uint64_t)) {
        uint64_t value = *(const uint64_t *)Data;
        
        // Test aom_uleb_size_in_bytes
        size_t leb_size = aom_uleb_size_in_bytes(value);
        
        // Test aom_uleb_encode
        if (leb_size > 0 && leb_size <= 256) {
            uint8_t *encoded_buffer = (uint8_t *)malloc(leb_size);
            size_t coded_size = 0;
            
            if (encoded_buffer) {
                int encode_result = aom_uleb_encode(value, leb_size, 
                                                   encoded_buffer, &coded_size);
                (void)encode_result;
                
                // Test aom_uleb_decode with the encoded data
                if (coded_size > 0) {
                    uint64_t decoded_value = 0;
                    size_t decoded_length = 0;
                    int decode_result = aom_uleb_decode(encoded_buffer, coded_size,
                                                       &decoded_value, &decoded_length);
                    (void)decode_result;
                    (void)decoded_value;
                    (void)decoded_length;
                }
                
                free(encoded_buffer);
            }
        }
        
        // Test aom_uleb_encode_fixed_size with various fixed sizes
        for (size_t fixed_size = 1; fixed_size <= 10 && fixed_size <= Size; fixed_size++) {
            if (fixed_size <= 256) {
                uint8_t *fixed_buffer = (uint8_t *)malloc(fixed_size);
                size_t fixed_coded_size = 0;
                
                if (fixed_buffer) {
                    int fixed_result = aom_uleb_encode_fixed_size(value, fixed_size,
                                                                 fixed_size, fixed_buffer,
                                                                 &fixed_coded_size);
                    (void)fixed_result;
                    free(fixed_buffer);
                }
            }
        }
        
        // Test aom_uleb_decode with raw fuzzer data
        if (Size > 0) {
            uint64_t decoded_value = 0;
            size_t decoded_length = 0;
            
            // Try decoding from different positions in the input
            for (size_t offset = 0; offset < Size && offset < 16; offset++) {
                size_t available = Size - offset;
                if (available > 0) {
                    int decode_result = aom_uleb_decode(Data + offset, available,
                                                       &decoded_value, &decoded_length);
                    (void)decode_result;
                }
            }
        }
    }
    
    // Test edge cases for aom_uleb_decode
    if (Size > 0) {
        uint64_t decoded_value = 0;
        size_t decoded_length = 0;
        
        // Test with zero available bytes
        aom_uleb_decode(Data, 0, &decoded_value, &decoded_length);
        
        // Test with single byte
        if (Size >= 1) {
            aom_uleb_decode(Data, 1, &decoded_value, &decoded_length);
        }
        
        // Test with maximum LEB128 size (10 bytes)
        if (Size >= 10) {
            aom_uleb_decode(Data, 10, &decoded_value, &decoded_length);
        }
    }
    
    // Test aom_img_add_metadata with edge cases
    if (Size >= 1) {
        // Test with NULL image
        aom_img_add_metadata(NULL, 0, Data, Size > 0 ? 1 : 0, AOM_MIF_ANY_FRAME);
        
        // Test with NULL data
        aom_img_add_metadata(&dummy_img, 0, NULL, Size > 0 ? 1 : 0, AOM_MIF_ANY_FRAME);
        
        // Test with zero size
        aom_img_add_metadata(&dummy_img, 0, Data, 0, AOM_MIF_ANY_FRAME);
    }
    
    // Test aom_uleb_encode with edge cases
    if (Size >= 1) {
        uint8_t dummy_buffer[1];
        size_t coded_size = 0;
        
        // Test with zero available space
        aom_uleb_encode(0, 0, dummy_buffer, &coded_size);
        
        // Test with NULL buffer
        aom_uleb_encode(0, 1, NULL, &coded_size);
        
        // Test with NULL coded_size
        aom_uleb_encode(0, 1, dummy_buffer, NULL);
    }
    
    dummy_cleanup();
    return 0;
}

#ifdef __cplusplus
}
#endif