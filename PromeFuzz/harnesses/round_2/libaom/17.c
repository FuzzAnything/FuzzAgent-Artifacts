// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_add_metadata at aom_image.c:380:5 in aom_image.h
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
// aom_img_add_metadata at aom_image.c:380:5 in aom_image.h
// aom_img_add_metadata at aom_image.c:380:5 in aom_image.h
// aom_img_add_metadata at aom_image.c:380:5 in aom_image.h
// aom_uleb_size_in_bytes at aom_integer.c:23:8 in aom_integer.h
// aom_uleb_encode at aom_integer.c:58:5 in aom_integer.h
// aom_uleb_decode at aom_integer.c:31:5 in aom_integer.h
// aom_uleb_decode at aom_integer.c:31:5 in aom_integer.h
// aom_uleb_encode_fixed_size at aom_integer.c:79:5 in aom_integer.h
// aom_uleb_encode_fixed_size at aom_integer.c:79:5 in aom_integer.h
// aom_uleb_encode_fixed_size at aom_integer.c:79:5 in aom_integer.h
// aom_uleb_encode_fixed_size at aom_integer.c:79:5 in aom_integer.h
// aom_uleb_encode_fixed_size at aom_integer.c:79:5 in aom_integer.h
// aom_uleb_encode at aom_integer.c:58:5 in aom_integer.h
// aom_uleb_encode at aom_integer.c:58:5 in aom_integer.h
// aom_uleb_encode at aom_integer.c:58:5 in aom_integer.h
// aom_uleb_decode at aom_integer.c:31:5 in aom_integer.h
// aom_uleb_decode at aom_integer.c:31:5 in aom_integer.h
// aom_uleb_decode at aom_integer.c:31:5 in aom_integer.h
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom/aom_image.h"
#include "aom/aom_integer.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void initialize_image(aom_image_t *img) {
    if (!img) return;
    
    // Initialize with default values
    img->fmt = AOM_IMG_FMT_I420;
    img->cp = AOM_CICP_CP_BT_709;
    img->tc = AOM_CICP_TC_BT_709;
    img->mc = AOM_CICP_MC_BT_709;
    img->monochrome = 0;
    img->csp = AOM_CSP_VERTICAL;
    img->range = AOM_CR_STUDIO_RANGE;
    
    img->w = 16;
    img->h = 16;
    img->bit_depth = 8;
    
    img->d_w = 16;
    img->d_h = 16;
    
    img->r_w = 16;
    img->r_h = 16;
    
    img->x_chroma_shift = 1;
    img->y_chroma_shift = 1;
    
    img->planes[0] = NULL;
    img->planes[1] = NULL;
    img->planes[2] = NULL;
    img->stride[0] = 16;
    img->stride[1] = 8;
    img->stride[2] = 8;
    img->sz = 0;
    
    img->bps = 8;
    
    img->temporal_id = 0;
    img->spatial_id = 0;
    
    img->user_priv = NULL;
    img->img_data = NULL;
    img->img_data_owner = 0;
    img->self_allocd = 0;
    
    // Set metadata to NULL - the library will handle allocation
    img->metadata = NULL;
    
    img->fb_priv = NULL;
}

static void cleanup_image(aom_image_t *img) {
    if (!img) return;
    
    // Don't try to free metadata array as we don't know its structure
    // The library functions should handle this internally
    
    if (img->img_data && img->img_data_owner) {
        free(img->img_data);
    }
}

int LLVMFuzzerTestOneInput_17(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Test aom_img_num_metadata and aom_img_add_metadata
    {
        aom_image_t img;
        initialize_image(&img);
        
        // Test with NULL
        size_t num_meta = aom_img_num_metadata(NULL);
        (void)num_meta;
        
        // Test with valid image (should be 0 since metadata is NULL)
        num_meta = aom_img_num_metadata(&img);
        
        // Add metadata if we have enough data
        if (Size >= 5) {
            uint32_t type = *(const uint32_t*)Data;
            size_t data_size = Size - 4;
            if (data_size > 0) {
                const uint8_t* metadata_data = Data + 4;
                
                // Test with various insert flags
                aom_metadata_insert_flags_t flags[] = {
                    AOM_MIF_NON_KEY_FRAME,
                    AOM_MIF_KEY_FRAME,
                    AOM_MIF_ANY_FRAME,
                    AOM_MIF_NON_KEY_FRAME_LAYER_SPECIFIC,
                    AOM_MIF_KEY_FRAME_LAYER_SPECIFIC,
                    AOM_MIF_ANY_FRAME_LAYER_SPECIFIC
                };
                
                for (size_t i = 0; i < sizeof(flags)/sizeof(flags[0]); i++) {
                    int ret = aom_img_add_metadata(&img, type, metadata_data, 
                                                  data_size > 1024 ? 1024 : data_size, 
                                                  flags[i]);
                    (void)ret;
                    
                    // Test num_metadata after adding
                    num_meta = aom_img_num_metadata(&img);
                }
                
                // Test error cases
                aom_img_add_metadata(NULL, type, metadata_data, data_size, AOM_MIF_ANY_FRAME);
                aom_img_add_metadata(&img, type, NULL, data_size, AOM_MIF_ANY_FRAME);
                aom_img_add_metadata(&img, type, metadata_data, 0, AOM_MIF_ANY_FRAME);
            }
        }
        
        cleanup_image(&img);
    }
    
    // Test LEB128 functions
    {
        // Test aom_uleb_size_in_bytes
        if (Size >= 8) {
            uint64_t value = *(const uint64_t*)Data;
            size_t size_bytes = aom_uleb_size_in_bytes(value);
            (void)size_bytes;
        }
        
        // Test aom_uleb_encode and aom_uleb_decode
        if (Size >= 8) {
            uint64_t encode_value = *(const uint64_t*)Data;
            uint8_t encode_buffer[10];
            size_t encoded_size = 0;
            
            // Test encode
            int encode_ret = aom_uleb_encode(encode_value, sizeof(encode_buffer), 
                                            encode_buffer, &encoded_size);
            (void)encode_ret;
            
            // Test decode on the encoded buffer
            if (encoded_size > 0) {
                uint64_t decoded_value = 0;
                size_t decoded_length = 0;
                int decode_ret = aom_uleb_decode(encode_buffer, encoded_size, 
                                                &decoded_value, &decoded_length);
                (void)decode_ret;
            }
            
            // Test decode on fuzzer input
            if (Size > 0) {
                uint64_t decode_value = 0;
                size_t decode_length = 0;
                aom_uleb_decode(Data, Size, &decode_value, &decode_length);
            }
            
            // Test aom_uleb_encode_fixed_size
            size_t pad_to_size = (Size % 10) + 1; // 1-10
            size_t coded_size = 0;
            uint8_t fixed_buffer[10];
            
            aom_uleb_encode_fixed_size(encode_value, sizeof(fixed_buffer), 
                                      pad_to_size, fixed_buffer, &coded_size);
            
            // Test error cases for encode_fixed_size
            aom_uleb_encode_fixed_size(encode_value, 0, pad_to_size, 
                                      fixed_buffer, &coded_size);
            aom_uleb_encode_fixed_size(encode_value, sizeof(fixed_buffer), 
                                      pad_to_size, NULL, &coded_size);
            aom_uleb_encode_fixed_size(encode_value, sizeof(fixed_buffer), 
                                      pad_to_size, fixed_buffer, NULL);
            
            // Test with value that might be too large for pad_to_size
            if (pad_to_size < 10) {
                // Create a value that likely needs more bytes than pad_to_size
                uint64_t large_value = UINT64_MAX >> (8 * (10 - pad_to_size - 1));
                aom_uleb_encode_fixed_size(large_value, sizeof(fixed_buffer), 
                                          pad_to_size, fixed_buffer, &coded_size);
            }
        }
        
        // Test error cases for aom_uleb_encode
        if (Size >= 1) {
            uint8_t small_buffer[1];
            size_t coded_size = 0;
            aom_uleb_encode(1, 0, small_buffer, &coded_size);
            aom_uleb_encode(1, sizeof(small_buffer), NULL, &coded_size);
            aom_uleb_encode(1, sizeof(small_buffer), small_buffer, NULL);
        }
        
        // Test error cases for aom_uleb_decode
        if (Size >= 1) {
            uint64_t value = 0;
            size_t length = 0;
            aom_uleb_decode(NULL, Size, &value, &length);
            aom_uleb_decode(Data, Size, NULL, &length);
            aom_uleb_decode(Data, Size, &value, NULL);
        }
    }
    
    return 0;
}