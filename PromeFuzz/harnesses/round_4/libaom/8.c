// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_uleb_encode_fixed_size at aom_integer.c:79:5 in aom_integer.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_uleb_size_in_bytes at aom_integer.c:23:8 in aom_integer.h
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
// aom_img_add_metadata at aom_image.c:380:5 in aom_image.h
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
// aom_uleb_decode at aom_integer.c:31:5 in aom_integer.h
// aom_uleb_decode at aom_integer.c:31:5 in aom_integer.h
// aom_uleb_decode at aom_integer.c:31:5 in aom_integer.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_add_metadata at aom_image.c:380:5 in aom_image.h
// aom_img_add_metadata at aom_image.c:380:5 in aom_image.h
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
// aom_uleb_encode at aom_integer.c:58:5 in aom_integer.h
// aom_uleb_encode at aom_integer.c:58:5 in aom_integer.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aom_integer.h"
#include "aom/aom_image.h"

static aom_image_t *create_dummy_image(void) {
    aom_image_t *img = aom_img_alloc(NULL, AOM_IMG_FMT_I420, 16, 16, 0);
    if (!img) return NULL;
    
    img->cp = AOM_CICP_CP_BT_709;
    img->tc = AOM_CICP_TC_BT_709;
    img->mc = AOM_CICP_MC_BT_709;
    img->monochrome = 0;
    img->csp = AOM_CSP_VERTICAL;
    img->range = AOM_CR_STUDIO_RANGE;
    img->bit_depth = 8;
    img->d_w = 16;
    img->d_h = 16;
    img->r_w = 16;
    img->r_h = 16;
    img->x_chroma_shift = 1;
    img->y_chroma_shift = 1;
    img->temporal_id = 0;
    img->spatial_id = 0;
    
    return img;
}

static void free_dummy_image(aom_image_t *img) {
    aom_img_free(img);
}

int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    uint8_t selector = Data[0];
    const uint8_t *fuzz_data = Data + 1;
    size_t fuzz_size = Size - 1;
    
    if (selector % 6 == 0) {
        aom_image_t *img = create_dummy_image();
        if (img) {
            uint32_t type = 0;
            if (fuzz_size >= sizeof(uint32_t)) {
                memcpy(&type, fuzz_data, sizeof(uint32_t));
            }
            
            const uint8_t *metadata_data = fuzz_data + sizeof(uint32_t);
            size_t metadata_sz = fuzz_size > sizeof(uint32_t) ? 
                                 fuzz_size - sizeof(uint32_t) : 0;
            
            aom_metadata_insert_flags_t flags[] = {
                AOM_MIF_NON_KEY_FRAME,
                AOM_MIF_KEY_FRAME,
                AOM_MIF_ANY_FRAME,
                AOM_MIF_NON_KEY_FRAME_LAYER_SPECIFIC,
                AOM_MIF_KEY_FRAME_LAYER_SPECIFIC,
                AOM_MIF_ANY_FRAME_LAYER_SPECIFIC
            };
            
            for (size_t i = 0; i < sizeof(flags)/sizeof(flags[0]); i++) {
                aom_img_add_metadata(img, type, metadata_data, metadata_sz, flags[i]);
            }
            
            if (metadata_sz > 0) {
                aom_img_add_metadata(img, type, NULL, metadata_sz, AOM_MIF_ANY_FRAME);
            }
            
            size_t num_meta = aom_img_num_metadata(img);
            (void)num_meta;
            
            free_dummy_image(img);
        }
    }
    else if (selector % 6 == 1) {
        uint64_t value = 0;
        if (fuzz_size >= sizeof(uint64_t)) {
            memcpy(&value, fuzz_data, sizeof(uint64_t));
        }
        
        size_t available = 20;
        uint8_t *coded_value = (uint8_t *)malloc(available);
        if (coded_value) {
            size_t coded_size = 0;
            aom_uleb_encode(value, available, coded_value, &coded_size);
            
            if (available > 0) {
                aom_uleb_encode(value, available / 2, coded_value, &coded_size);
            }
            
            free(coded_value);
        }
    }
    else if (selector % 6 == 2) {
        uint64_t value = 0;
        if (fuzz_size >= sizeof(uint64_t)) {
            memcpy(&value, fuzz_data, sizeof(uint64_t));
        }
        
        size_t available = 20;
        size_t pad_to_size = (selector % 10) + 1;
        uint8_t *coded_value = (uint8_t *)malloc(available);
        if (coded_value) {
            size_t coded_size = 0;
            aom_uleb_encode_fixed_size(value, available, pad_to_size, 
                                      coded_value, &coded_size);
            
            free(coded_value);
        }
    }
    else if (selector % 6 == 3) {
        uint64_t value = 0;
        if (fuzz_size >= sizeof(uint64_t)) {
            memcpy(&value, fuzz_data, sizeof(uint64_t));
        }
        
        size_t size = aom_uleb_size_in_bytes(value);
        (void)size;
    }
    else if (selector % 6 == 4) {
        aom_image_t *img = create_dummy_image();
        if (img) {
            size_t num1 = aom_img_num_metadata(img);
            (void)num1;
            
            if (fuzz_size > 0) {
                aom_img_add_metadata(img, 1, fuzz_data, 
                                    fuzz_size > 10 ? 10 : fuzz_size, 
                                    AOM_MIF_ANY_FRAME);
                size_t num2 = aom_img_num_metadata(img);
                (void)num2;
            }
            
            free_dummy_image(img);
        }
        
        size_t num_null = aom_img_num_metadata(NULL);
        (void)num_null;
    }
    else if (selector % 6 == 5) {
        if (fuzz_size > 0) {
            uint64_t decoded_value = 0;
            size_t decoded_length = 0;
            
            for (size_t avail = 1; avail <= fuzz_size && avail <= 10; avail++) {
                aom_uleb_decode(fuzz_data, avail, &decoded_value, &decoded_length);
            }
            
            if (fuzz_size > 0) {
                aom_uleb_decode(fuzz_data, fuzz_size, NULL, &decoded_length);
                aom_uleb_decode(fuzz_data, fuzz_size, &decoded_value, NULL);
            }
        }
    }
    
    return 0;
}