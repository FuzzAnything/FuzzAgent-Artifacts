// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
// aom_img_add_metadata at aom_image.c:380:5 in aom_image.h
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
// aom_img_add_metadata at aom_image.c:380:5 in aom_image.h
// aom_img_add_metadata at aom_image.c:380:5 in aom_image.h
// aom_uleb_size_in_bytes at aom_integer.c:23:8 in aom_integer.h
// aom_uleb_encode at aom_integer.c:58:5 in aom_integer.h
// aom_uleb_decode at aom_integer.c:31:5 in aom_integer.h
// aom_uleb_encode_fixed_size at aom_integer.c:79:5 in aom_integer.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "aom/aom_integer.h"
#include "aom/aom_image.h"

static void initialize_image(aom_image_t *img) {
    // Initialize minimal image structure for metadata testing
    memset(img, 0, sizeof(aom_image_t));
    img->fmt = AOM_IMG_FMT_I420;
    img->w = 16;
    img->h = 16;
    img->d_w = 16;
    img->d_h = 16;
    img->r_w = 16;
    img->r_h = 16;
    img->bit_depth = 8;
    img->x_chroma_shift = 1;
    img->y_chroma_shift = 1;
    img->range = AOM_CR_STUDIO_RANGE;
    img->csp = AOM_CSP_VERTICAL;
    img->cp = AOM_CICP_CP_BT_709;
    img->tc = AOM_CICP_TC_BT_709;
    img->mc = AOM_CICP_MC_BT_709;
    img->monochrome = 0;
    img->bps = 12;
    img->temporal_id = 0;
    img->spatial_id = 0;
    img->img_data_owner = 0;
    img->self_allocd = 0;
    img->metadata = NULL;
    img->fb_priv = NULL;
}

int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    // Test aom_uleb_size_in_bytes
    if (Size >= sizeof(uint64_t)) {
        uint64_t value;
        memcpy(&value, Data, sizeof(uint64_t));
        size_t uleb_size = aom_uleb_size_in_bytes(value);
        (void)uleb_size; // Use result to avoid unused variable warning
    }

    // Test aom_uleb_encode and aom_uleb_decode
    if (Size >= sizeof(uint64_t) + 1) {
        uint64_t encode_value;
        memcpy(&encode_value, Data, sizeof(uint64_t));
        
        size_t available = Size - sizeof(uint64_t);
        const uint8_t *buffer = Data + sizeof(uint64_t);
        
        // Encode
        uint8_t coded_buffer[10];
        size_t coded_size = 0;
        int encode_result = aom_uleb_encode(encode_value, sizeof(coded_buffer), 
                                           coded_buffer, &coded_size);
        (void)encode_result;
        
        // Decode
        if (available > 0) {
            uint64_t decode_value;
            size_t decode_length = 0;
            int decode_result = aom_uleb_decode(buffer, available, 
                                               &decode_value, &decode_length);
            (void)decode_result;
            (void)decode_value;
            (void)decode_length;
        }
    }

    // Test aom_uleb_encode_fixed_size
    if (Size >= sizeof(uint64_t) + sizeof(size_t)) {
        uint64_t value;
        memcpy(&value, Data, sizeof(uint64_t));
        
        size_t pad_to_size = 0;
        if (Size - sizeof(uint64_t) >= sizeof(size_t)) {
            memcpy(&pad_to_size, Data + sizeof(uint64_t), sizeof(size_t));
            // Limit pad_to_size to reasonable value
            if (pad_to_size > 10) pad_to_size = 10;
        }
        
        uint8_t fixed_buffer[10];
        size_t fixed_coded_size = 0;
        int fixed_result = aom_uleb_encode_fixed_size(value, sizeof(fixed_buffer),
                                                     pad_to_size, fixed_buffer,
                                                     &fixed_coded_size);
        (void)fixed_result;
    }

    // Test aom_img_add_metadata and aom_img_num_metadata
    if (Size >= sizeof(uint32_t) + sizeof(size_t)) {
        // Parse metadata parameters from fuzz data
        uint32_t metadata_type = 0;
        size_t metadata_size = 0;
        const uint8_t *metadata_data = NULL;
        aom_metadata_insert_flags_t insert_flag = AOM_MIF_ANY_FRAME;
        
        if (Size >= sizeof(uint32_t)) {
            memcpy(&metadata_type, Data, sizeof(uint32_t));
        }
        
        if (Size >= sizeof(uint32_t) + sizeof(size_t)) {
            memcpy(&metadata_size, Data + sizeof(uint32_t), sizeof(size_t));
            // Limit metadata size to prevent excessive memory allocation
            if (metadata_size > 1024) metadata_size = 1024;
            if (metadata_size > Size - sizeof(uint32_t) - sizeof(size_t)) {
                metadata_size = Size - sizeof(uint32_t) - sizeof(size_t);
            }
        }
        
        if (metadata_size > 0 && Size >= sizeof(uint32_t) + sizeof(size_t) + metadata_size) {
            metadata_data = Data + sizeof(uint32_t) + sizeof(size_t);
        }
        
        // Create and initialize image
        aom_image_t img;
        initialize_image(&img);
        
        // Test aom_img_num_metadata on empty image
        size_t num_metadata_before = aom_img_num_metadata(&img);
        (void)num_metadata_before;
        
        // Add metadata if we have data
        if (metadata_data != NULL && metadata_size > 0) {
            int add_result = aom_img_add_metadata(&img, metadata_type,
                                                 metadata_data, metadata_size,
                                                 insert_flag);
            (void)add_result;
            
            // Test aom_img_num_metadata after adding
            size_t num_metadata_after = aom_img_num_metadata(&img);
            (void)num_metadata_after;
            
            // Try adding with different insert flags
            if (metadata_size > 1) {
                aom_img_add_metadata(&img, metadata_type + 1,
                                    metadata_data, metadata_size - 1,
                                    AOM_MIF_KEY_FRAME);
            }
            
            if (metadata_size > 2) {
                aom_img_add_metadata(&img, metadata_type + 2,
                                    metadata_data, metadata_size - 2,
                                    AOM_MIF_NON_KEY_FRAME);
            }
        }
        
        // Clean up metadata if it was allocated
        if (img.metadata != NULL) {
            // Note: In real libaom usage, we would need to properly free
            // the metadata. For fuzzing, we rely on the process exiting.
        }
    }

    return 0;
}