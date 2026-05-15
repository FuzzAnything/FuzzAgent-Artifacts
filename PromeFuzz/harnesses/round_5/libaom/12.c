// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_metadata_free at aom_image.c:345:6 in aom_image.h
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
// aom_img_metadata_free at aom_image.c:345:6 in aom_image.h
// aom_img_add_metadata at aom_image.c:380:5 in aom_image.h
// aom_img_remove_metadata at aom_image.c:412:6 in aom_image.h
// aom_img_remove_metadata at aom_image.c:412:6 in aom_image.h
// aom_img_remove_metadata at aom_image.c:412:6 in aom_image.h
// aom_img_get_metadata at aom_image.c:419:23 in aom_image.h
// aom_img_metadata_alloc at aom_image.c:327:17 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom/aom_image.h"

static aom_image_t* create_test_image(void) {
    aom_image_t* img = (aom_image_t*)malloc(sizeof(aom_image_t));
    if (!img) return NULL;
    
    memset(img, 0, sizeof(aom_image_t));
    img->fmt = AOM_IMG_FMT_I420;
    img->w = 16;
    img->h = 16;
    img->d_w = 16;
    img->d_h = 16;
    img->r_w = 16;
    img->r_h = 16;
    img->bit_depth = 8;
    img->bps = 8;
    img->x_chroma_shift = 1;
    img->y_chroma_shift = 1;
    img->range = AOM_CR_STUDIO_RANGE;
    img->csp = AOM_CSP_VERTICAL;
    img->cp = AOM_CICP_CP_BT_709;
    img->tc = AOM_CICP_TC_BT_709;
    img->mc = AOM_CICP_MC_BT_709;
    img->monochrome = 0;
    img->temporal_id = 0;
    img->spatial_id = 0;
    
    img->metadata = NULL;
    
    return img;
}

static void free_test_image(aom_image_t* img) {
    if (img) {
        aom_img_remove_metadata(img);
        free(img);
    }
}

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    aom_image_t* img = create_test_image();
    if (!img) return 0;
    
    size_t num_ops = Data[0] % 10 + 1;
    size_t data_offset = 1;
    
    for (size_t op = 0; op < num_ops && data_offset < Size; op++) {
        uint8_t op_type = Data[data_offset % Size];
        data_offset = (data_offset + 1) % Size;
        
        switch (op_type % 6) {
            case 0: {
                aom_img_remove_metadata(img);
                break;
            }
            case 1: {
                size_t index = Data[data_offset % Size] % 10;
                data_offset = (data_offset + 1) % Size;
                const aom_metadata_t* meta = aom_img_get_metadata(img, index);
                (void)meta;
                break;
            }
            case 2: {
                if (Size - data_offset < 5) break;
                
                uint32_t type = 0;
                for (int i = 0; i < 4 && data_offset < Size; i++) {
                    type = (type << 8) | Data[data_offset];
                    data_offset = (data_offset + 1) % Size;
                }
                
                size_t sz = Data[data_offset % Size] % 256;
                data_offset = (data_offset + 1) % Size;
                
                aom_metadata_insert_flags_t insert_flag = 
                    (aom_metadata_insert_flags_t)(Data[data_offset % Size] % 19);
                data_offset = (data_offset + 1) % Size;
                
                const uint8_t* data = NULL;
                if (sz > 0 && data_offset < Size) {
                    size_t available = Size - data_offset;
                    if (sz > available) {
                        sz = available;
                    }
                    if (sz > 0) {
                        data = &Data[data_offset];
                        data_offset = (data_offset + sz) % Size;
                    }
                }
                
                aom_metadata_t* meta = aom_img_metadata_alloc(type, data, sz, insert_flag);
                if (meta) {
                    aom_img_metadata_free(meta);
                }
                break;
            }
            case 3: {
                size_t num = aom_img_num_metadata(img);
                (void)num;
                break;
            }
            case 4: {
                aom_img_metadata_free(NULL);
                break;
            }
            case 5: {
                if (Size - data_offset < 5) break;
                
                uint32_t type = 0;
                for (int i = 0; i < 4 && data_offset < Size; i++) {
                    type = (type << 8) | Data[data_offset];
                    data_offset = (data_offset + 1) % Size;
                }
                
                size_t sz = Data[data_offset % Size] % 256;
                data_offset = (data_offset + 1) % Size;
                
                aom_metadata_insert_flags_t insert_flag = 
                    (aom_metadata_insert_flags_t)(Data[data_offset % Size] % 19);
                data_offset = (data_offset + 1) % Size;
                
                const uint8_t* data = NULL;
                if (sz > 0 && data_offset < Size) {
                    size_t available = Size - data_offset;
                    if (sz > available) {
                        sz = available;
                    }
                    if (sz > 0) {
                        data = &Data[data_offset];
                        data_offset = (data_offset + sz) % Size;
                    }
                }
                
                int ret = aom_img_add_metadata(img, type, data, sz, insert_flag);
                (void)ret;
                break;
            }
        }
    }
    
    aom_img_remove_metadata(img);
    free_test_image(img);
    
    return 0;
}