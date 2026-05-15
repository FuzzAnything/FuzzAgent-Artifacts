// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_uleb_encode_fixed_size at aom_integer.c:79:5 in aom_integer.h
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
// aom_img_add_metadata at aom_image.c:380:5 in aom_image.h
// aom_img_add_metadata at aom_image.c:380:5 in aom_image.h
// aom_img_add_metadata at aom_image.c:380:5 in aom_image.h
// aom_img_add_metadata at aom_image.c:380:5 in aom_image.h
// aom_uleb_size_in_bytes at aom_integer.c:23:8 in aom_integer.h
// aom_uleb_encode at aom_integer.c:58:5 in aom_integer.h
// aom_uleb_decode at aom_integer.c:31:5 in aom_integer.h
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

static void init_dummy_image(aom_image_t *img) {
    if (!img) return;
    
    memset(img, 0, sizeof(aom_image_t));
    img->fmt = AOM_IMG_FMT_I420;
    img->cp = AOM_CICP_CP_BT_709;
    img->tc = AOM_CICP_TC_BT_709;
    img->mc = AOM_CICP_MC_BT_709;
    img->monochrome = 0;
    img->csp = AOM_CSP_VERTICAL;
    img->range = AOM_CR_STUDIO_RANGE;
    img->w = 64;
    img->h = 64;
    img->bit_depth = 8;
    img->d_w = 64;
    img->d_h = 64;
    img->r_w = 64;
    img->r_h = 64;
    img->x_chroma_shift = 1;
    img->y_chroma_shift = 1;
    img->sz = 0;
    img->bps = 12;
    img->temporal_id = 0;
    img->spatial_id = 0;
    img->user_priv = NULL;
    img->img_data = NULL;
    img->img_data_owner = 0;
    img->self_allocd = 0;
    img->metadata = NULL;
    img->fb_priv = NULL;
    
    for (int i = 0; i < 3; i++) {
        img->planes[i] = NULL;
        img->stride[i] = 0;
    }
}

int LLVMFuzzerTestOneInput_23(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Use first byte to determine which functions to test
    uint8_t selector = Data[0];
    const uint8_t *fuzz_data = Data + 1;
    size_t fuzz_size = Size - 1;
    
    // Test aom_uleb_size_in_bytes
    if (fuzz_size >= sizeof(uint64_t)) {
        uint64_t value = 0;
        memcpy(&value, fuzz_data, sizeof(uint64_t));
        size_t encoded_size = aom_uleb_size_in_bytes(value);
        (void)encoded_size; // Use result to avoid unused variable warning
    }
    
    // Test aom_uleb_encode and aom_uleb_decode
    if (fuzz_size >= sizeof(uint64_t) + 1) {
        uint64_t encode_value = 0;
        memcpy(&encode_value, fuzz_data, sizeof(uint64_t));
        
        size_t available = fuzz_size - sizeof(uint64_t);
        if (available > 20) available = 20; // Reasonable buffer size
        
        uint8_t *buffer = (uint8_t*)malloc(available);
        if (buffer) {
            size_t coded_size = 0;
            
            // Test aom_uleb_encode
            int encode_result = aom_uleb_encode(encode_value, available, buffer, &coded_size);
            (void)encode_result;
            
            // Test aom_uleb_decode
            if (coded_size > 0 && coded_size <= available) {
                uint64_t decode_value = 0;
                size_t decode_length = 0;
                int decode_result = aom_uleb_decode(buffer, available, &decode_value, &decode_length);
                (void)decode_result;
            }
            
            free(buffer);
        }
    }
    
    // Test aom_uleb_encode_fixed_size
    if (fuzz_size >= sizeof(uint64_t) + sizeof(size_t) * 2) {
        uint64_t value = 0;
        memcpy(&value, fuzz_data, sizeof(uint64_t));
        
        size_t available = 0;
        size_t pad_to_size = 0;
        memcpy(&available, fuzz_data + sizeof(uint64_t), sizeof(size_t));
        memcpy(&pad_to_size, fuzz_data + sizeof(uint64_t) + sizeof(size_t), sizeof(size_t));
        
        // Ensure reasonable values
        if (pad_to_size > 20) pad_to_size = 20;
        if (available > 20) available = 20;
        if (available < pad_to_size) available = pad_to_size;
        
        uint8_t *coded_value = (uint8_t*)malloc(available);
        if (coded_value) {
            size_t coded_size = 0;
            int result = aom_uleb_encode_fixed_size(value, available, pad_to_size, coded_value, &coded_size);
            (void)result;
            free(coded_value);
        }
    }
    
    // Test aom_img_num_metadata
    {
        aom_image_t img;
        init_dummy_image(&img);
        
        // Test with NULL
        size_t num1 = aom_img_num_metadata(NULL);
        (void)num1;
        
        // Test with valid image (no metadata)
        size_t num2 = aom_img_num_metadata(&img);
        (void)num2;
        
        // Test with image containing metadata array (using pointer only)
        // We can't create a full aom_metadata_array_t since it's incomplete
        // Just test with NULL pointer for metadata array
        img.metadata = NULL;
        
        size_t num3 = aom_img_num_metadata(&img);
        (void)num3;
    }
    
    // Test aom_img_add_metadata
    if (fuzz_size >= sizeof(uint32_t) + sizeof(size_t)) {
        aom_image_t img;
        init_dummy_image(&img);
        
        uint32_t type = 0;
        memcpy(&type, fuzz_data, sizeof(uint32_t));
        
        size_t data_size = 0;
        memcpy(&data_size, fuzz_data + sizeof(uint32_t), sizeof(size_t));
        
        // Limit data size to prevent excessive memory allocation
        if (data_size > 1024) data_size = 1024;
        
        const uint8_t *metadata_data = NULL;
        if (fuzz_size >= sizeof(uint32_t) + sizeof(size_t) + data_size) {
            metadata_data = fuzz_data + sizeof(uint32_t) + sizeof(size_t);
        } else {
            // Use dummy data if not enough fuzz data
            metadata_data = fuzz_data;
            data_size = fuzz_size > 0 ? fuzz_size : 1;
        }
        
        // Test with different insert flags
        aom_metadata_insert_flags_t flags[] = {
            AOM_MIF_NON_KEY_FRAME,
            AOM_MIF_KEY_FRAME,
            AOM_MIF_ANY_FRAME,
            AOM_MIF_NON_KEY_FRAME_LAYER_SPECIFIC,
            AOM_MIF_KEY_FRAME_LAYER_SPECIFIC,
            AOM_MIF_ANY_FRAME_LAYER_SPECIFIC
        };
        
        for (size_t i = 0; i < sizeof(flags)/sizeof(flags[0]); i++) {
            // Reset image for each test
            init_dummy_image(&img);
            
            int result = aom_img_add_metadata(&img, type, metadata_data, data_size, flags[i]);
            (void)result;
        }
        
        // Test with NULL parameters (should fail)
        int null_result = aom_img_add_metadata(NULL, type, metadata_data, data_size, AOM_MIF_ANY_FRAME);
        (void)null_result;
        
        null_result = aom_img_add_metadata(&img, type, NULL, data_size, AOM_MIF_ANY_FRAME);
        (void)null_result;
        
        null_result = aom_img_add_metadata(&img, type, metadata_data, 0, AOM_MIF_ANY_FRAME);
        (void)null_result;
    }
    
    return 0;
}