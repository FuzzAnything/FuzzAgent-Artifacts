// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_add_metadata at aom_image.c:380:5 in aom_image.h
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
// aom_uleb_encode_fixed_size at aom_integer.c:79:5 in aom_integer.h
// aom_uleb_decode at aom_integer.c:31:5 in aom_integer.h
// aom_img_add_metadata at aom_image.c:380:5 in aom_image.h
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
// aom_uleb_size_in_bytes at aom_integer.c:23:8 in aom_integer.h
// aom_uleb_encode at aom_integer.c:58:5 in aom_integer.h
// aom_uleb_size_in_bytes at aom_integer.c:23:8 in aom_integer.h
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

static aom_image_t* create_dummy_image(void) {
    static aom_image_t img;
    memset(&img, 0, sizeof(img));
    
    img.fmt = AOM_IMG_FMT_I420;
    img.cp = AOM_CICP_CP_BT_709;
    img.tc = AOM_CICP_TC_BT_709;
    img.mc = AOM_CICP_MC_BT_709;
    img.monochrome = 0;
    img.csp = AOM_CSP_VERTICAL;
    img.range = AOM_CR_STUDIO_RANGE;
    
    img.w = 64;
    img.h = 64;
    img.d_w = 64;
    img.d_h = 64;
    img.r_w = 64;
    img.r_h = 64;
    img.bit_depth = 8;
    
    img.x_chroma_shift = 1;
    img.y_chroma_shift = 1;
    
    img.bps = 12;
    img.temporal_id = 0;
    img.spatial_id = 0;
    
    img.metadata = NULL;
    img.img_data = NULL;
    img.img_data_owner = 0;
    img.self_allocd = 0;
    img.fb_priv = NULL;
    
    return &img;
}

int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    uint64_t value = 0;
    size_t coded_size = 0;
    size_t available = 0;
    size_t pad_to_size = 0;
    uint8_t *buffer = NULL;
    size_t length = 0;
    int result = 0;
    
    uint8_t selector = Data[0] % 7;
    const uint8_t *fuzz_data = Data + 1;
    size_t fuzz_size = Size - 1;
    
    switch (selector) {
        case 0: {
            if (fuzz_size >= sizeof(uint64_t)) {
                memcpy(&value, fuzz_data, sizeof(uint64_t));
            } else if (fuzz_size > 0) {
                memcpy(&value, fuzz_data, fuzz_size);
            }
            
            size_t needed_size = aom_uleb_size_in_bytes(value);
            if (needed_size > 0 && needed_size <= 10) {
                uint8_t coded_value[10];
                available = sizeof(coded_value);
                result = aom_uleb_encode(value, available, coded_value, &coded_size);
            }
            break;
        }
        
        case 1: {
            if (fuzz_size >= sizeof(uint64_t)) {
                memcpy(&value, fuzz_data, sizeof(uint64_t));
            } else if (fuzz_size > 0) {
                memcpy(&value, fuzz_data, fuzz_size);
            }
            
            size_t size = aom_uleb_size_in_bytes(value);
            break;
        }
        
        case 2: {
            aom_image_t *img = create_dummy_image();
            uint32_t type = 0;
            const uint8_t *data = NULL;
            size_t sz = 0;
            aom_metadata_insert_flags_t insert_flag = AOM_MIF_ANY_FRAME;
            
            if (fuzz_size >= sizeof(uint32_t)) {
                memcpy(&type, fuzz_data, sizeof(uint32_t));
                data = fuzz_data + sizeof(uint32_t);
                sz = fuzz_size - sizeof(uint32_t);
                
                if (sz > 0) {
                    insert_flag = (data[0] % 6);
                    if (insert_flag > 5) insert_flag = AOM_MIF_ANY_FRAME;
                    
                    result = aom_img_add_metadata(img, type, data, sz, insert_flag);
                }
            }
            break;
        }
        
        case 3: {
            aom_image_t *img = create_dummy_image();
            size_t num_meta = aom_img_num_metadata(img);
            break;
        }
        
        case 4: {
            if (fuzz_size >= sizeof(uint64_t) + sizeof(size_t)) {
                memcpy(&value, fuzz_data, sizeof(uint64_t));
                memcpy(&pad_to_size, fuzz_data + sizeof(uint64_t), sizeof(size_t));
                
                if (pad_to_size > 0 && pad_to_size <= 10) {
                    uint8_t coded_value[10];
                    available = sizeof(coded_value);
                    result = aom_uleb_encode_fixed_size(value, available, pad_to_size, 
                                                       coded_value, &coded_size);
                }
            }
            break;
        }
        
        case 5: {
            if (fuzz_size > 0) {
                buffer = (uint8_t*)fuzz_data;
                available = fuzz_size;
                result = aom_uleb_decode(buffer, available, &value, &length);
            }
            break;
        }
        
        case 6: {
            aom_image_t *img = create_dummy_image();
            uint32_t type = 0;
            const uint8_t *data = NULL;
            size_t sz = 0;
            aom_metadata_insert_flags_t insert_flag = AOM_MIF_ANY_FRAME;
            
            if (fuzz_size >= sizeof(uint32_t)) {
                memcpy(&type, fuzz_data, sizeof(uint32_t));
                data = fuzz_data + sizeof(uint32_t);
                sz = fuzz_size - sizeof(uint32_t);
                
                if (sz > 0) {
                    insert_flag = (data[0] % 6);
                    if (insert_flag > 5) insert_flag = AOM_MIF_ANY_FRAME;
                    
                    result = aom_img_add_metadata(img, type, data, sz, insert_flag);
                    
                    if (result == 0) {
                        size_t num_meta = aom_img_num_metadata(img);
                    }
                }
            }
            break;
        }
    }
    
    return 0;
}