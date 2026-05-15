// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_add_metadata at aom_image.c:380:5 in aom_image.h
// aom_img_add_metadata at aom_image.c:380:5 in aom_image.h
// aom_img_add_metadata at aom_image.c:380:5 in aom_image.h
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
// aom_uleb_size_in_bytes at aom_integer.c:23:8 in aom_integer.h
// aom_uleb_encode at aom_integer.c:58:5 in aom_integer.h
// aom_uleb_decode at aom_integer.c:31:5 in aom_integer.h
// aom_uleb_encode_fixed_size at aom_integer.c:79:5 in aom_integer.h
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
// aom_img_add_metadata at aom_image.c:380:5 in aom_image.h
// aom_img_add_metadata at aom_image.c:380:5 in aom_image.h
// aom_img_add_metadata at aom_image.c:380:5 in aom_image.h
// aom_img_add_metadata at aom_image.c:380:5 in aom_image.h
// aom_img_add_metadata at aom_image.c:380:5 in aom_image.h
// aom_img_add_metadata at aom_image.c:380:5 in aom_image.h
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
    memset(img, 0, sizeof(aom_image_t));
    img->fmt = AOM_IMG_FMT_I420;
    img->cp = AOM_CICP_CP_BT_709;
    img->tc = AOM_CICP_TC_BT_709;
    img->mc = AOM_CICP_MC_BT_709;
    img->range = AOM_CR_STUDIO_RANGE;
    img->csp = AOM_CSP_VERTICAL;
    img->w = img->d_w = img->r_w = 64;
    img->h = img->d_h = img->r_h = 64;
    img->bit_depth = 8;
    img->x_chroma_shift = 1;
    img->y_chroma_shift = 1;
    img->bps = 12;
    img->temporal_id = 0;
    img->spatial_id = 0;
    img->img_data_owner = 0;
    img->self_allocd = 0;
    img->metadata = NULL;
    img->fb_priv = NULL;
}

int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Test aom_uleb_size_in_bytes
    uint64_t value = 0;
    if (Size >= sizeof(uint64_t)) {
        memcpy(&value, Data, sizeof(uint64_t));
    } else {
        memcpy(&value, Data, Size);
    }
    size_t uleb_size = aom_uleb_size_in_bytes(value);

    // Test aom_uleb_encode and aom_uleb_decode
    size_t available = Size > 20 ? 20 : Size;
    uint8_t *coded_value = malloc(available);
    size_t coded_size = 0;
    if (coded_value) {
        // Encode
        int encode_result = aom_uleb_encode(value, available, coded_value, &coded_size);
        
        // Decode if encode succeeded
        if (encode_result == 0 && coded_size > 0) {
            uint64_t decoded_value = 0;
            size_t decoded_length = 0;
            aom_uleb_decode(coded_value, coded_size, &decoded_value, &decoded_length);
        }
        
        // Test aom_uleb_encode_fixed_size with various pad sizes
        for (size_t pad_to_size = 1; pad_to_size <= 10 && pad_to_size <= available; pad_to_size++) {
            size_t fixed_coded_size = 0;
            aom_uleb_encode_fixed_size(value, available, pad_to_size, coded_value, &fixed_coded_size);
        }
        
        free(coded_value);
    }

    // Test image metadata functions
    aom_image_t img;
    init_dummy_image(&img);
    
    // Test aom_img_num_metadata with NULL and valid image
    size_t num_metadata = aom_img_num_metadata(NULL);
    num_metadata = aom_img_num_metadata(&img);
    
    // Test aom_img_add_metadata with various inputs
    if (Size > 0) {
        uint32_t type = 0;
        if (Size >= sizeof(uint32_t)) {
            memcpy(&type, Data, sizeof(uint32_t));
        } else {
            memcpy(&type, Data, Size);
        }
        
        size_t data_size = Size > 100 ? 100 : Size;
        const uint8_t *metadata_data = Data;
        
        // Test with different insert flags
        aom_img_add_metadata(&img, type, metadata_data, data_size, AOM_MIF_ANY_FRAME);
        aom_img_add_metadata(&img, type, metadata_data, data_size, AOM_MIF_KEY_FRAME);
        aom_img_add_metadata(&img, type, metadata_data, data_size, AOM_MIF_NON_KEY_FRAME);
        
        // Test with NULL parameters (should fail)
        aom_img_add_metadata(NULL, type, metadata_data, data_size, AOM_MIF_ANY_FRAME);
        aom_img_add_metadata(&img, type, NULL, data_size, AOM_MIF_ANY_FRAME);
        aom_img_add_metadata(&img, type, metadata_data, 0, AOM_MIF_ANY_FRAME);
        
        // Test layer-specific flags
        aom_img_add_metadata(&img, type, metadata_data, data_size, AOM_MIF_ANY_FRAME_LAYER_SPECIFIC);
        aom_img_add_metadata(&img, type, metadata_data, data_size, AOM_MIF_KEY_FRAME_LAYER_SPECIFIC);
        aom_img_add_metadata(&img, type, metadata_data, data_size, AOM_MIF_NON_KEY_FRAME_LAYER_SPECIFIC);
    }
    
    // Test aom_img_num_metadata again after adding metadata
    num_metadata = aom_img_num_metadata(&img);

    return 0;
}