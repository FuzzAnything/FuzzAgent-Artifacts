// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_add_metadata at aom_image.c:380:5 in aom_image.h
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
// aom_img_remove_metadata at aom_image.c:412:6 in aom_image.h
// aom_img_metadata_alloc at aom_image.c:327:17 in aom_image.h
// aom_img_metadata_free at aom_image.c:345:6 in aom_image.h
// aom_img_metadata_alloc at aom_image.c:327:17 in aom_image.h
// aom_img_add_metadata at aom_image.c:380:5 in aom_image.h
// aom_img_metadata_free at aom_image.c:345:6 in aom_image.h
// aom_img_remove_metadata at aom_image.c:412:6 in aom_image.h
// aom_img_get_metadata at aom_image.c:419:23 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom/aom_image.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void initialize_image(aom_image_t *img) {
    if (!img) return;
    
    // Initialize basic image parameters
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
    
    img->stride[0] = 16;
    img->stride[1] = 8;
    img->stride[2] = 8;
    
    img->sz = 384; // 16*16 + 8*8 + 8*8 = 384 bytes for I420
    
    img->bps = 8;
    
    img->temporal_id = 0;
    img->spatial_id = 0;
    
    img->user_priv = NULL;
    img->img_data = NULL;
    img->img_data_owner = 0;
    img->self_allocd = 0;
    img->metadata = NULL;
    img->fb_priv = NULL;
}

int LLVMFuzzerTestOneInput_13(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    // Create and initialize an image
    aom_image_t img;
    initialize_image(&img);
    
    // Parse fuzzer input to get parameters
    size_t offset = 0;
    
    // Get number of metadata operations from first 2 bytes
    uint16_t num_ops = 0;
    if (Size >= 2) {
        num_ops = (Data[0] << 8) | Data[1];
        offset = 2;
    }
    
    // Limit number of operations to prevent excessive loops
    if (num_ops > 100) num_ops = 100;
    
    for (uint16_t i = 0; i < num_ops && offset < Size; i++) {
        // Get operation type from next byte
        uint8_t op_type = Data[offset] % 6; // 6 operations
        offset++;
        
        if (op_type == 0) { // aom_img_get_metadata
            if (offset < Size) {
                size_t index = Data[offset] % 256; // Use byte as index
                offset++;
                const aom_metadata_t *md = aom_img_get_metadata(&img, index);
                (void)md; // Use result to avoid unused variable warning
            }
        }
        else if (op_type == 1) { // aom_img_add_metadata
            if (offset + 4 < Size) {
                // Get metadata type from next 4 bytes
                uint32_t type = 0;
                if (offset + 4 <= Size) {
                    memcpy(&type, Data + offset, 4);
                    offset += 4;
                }
                
                // Get size from next byte
                uint8_t sz = 0;
                if (offset < Size) {
                    sz = Data[offset];
                    offset++;
                }
                
                // Get insert flag from next byte
                uint8_t flag_val = 0;
                if (offset < Size) {
                    flag_val = Data[offset] % 19; // 19 enum values
                    offset++;
                }
                
                aom_metadata_insert_flags_t flag = (aom_metadata_insert_flags_t)flag_val;
                
                // Use remaining data as payload if available
                const uint8_t *payload = NULL;
                if (sz > 0 && offset < Size) {
                    // Limit payload size to available data
                    size_t available = Size - offset;
                    if (sz > available) sz = available;
                    
                    payload = Data + offset;
                    offset += sz;
                    
                    int result = aom_img_add_metadata(&img, type, payload, sz, flag);
                    (void)result; // Use result
                }
            }
        }
        else if (op_type == 2) { // aom_img_num_metadata
            size_t num = aom_img_num_metadata(&img);
            (void)num; // Use result
        }
        else if (op_type == 3) { // aom_img_remove_metadata
            aom_img_remove_metadata(&img);
        }
        else if (op_type == 4) { // aom_img_metadata_free
            // Create a metadata object first, then free it
            if (offset + 4 < Size) {
                uint32_t type = 0;
                if (offset + 4 <= Size) {
                    memcpy(&type, Data + offset, 4);
                    offset += 4;
                }
                
                uint8_t sz = 0;
                if (offset < Size) {
                    sz = Data[offset];
                    offset++;
                }
                
                uint8_t flag_val = 0;
                if (offset < Size) {
                    flag_val = Data[offset] % 19;
                    offset++;
                }
                
                aom_metadata_insert_flags_t flag = (aom_metadata_insert_flags_t)flag_val;
                
                const uint8_t *payload = NULL;
                if (sz > 0 && offset < Size) {
                    size_t available = Size - offset;
                    if (sz > available) sz = available;
                    
                    payload = Data + offset;
                    offset += sz;
                    
                    aom_metadata_t *md = aom_img_metadata_alloc(type, payload, sz, flag);
                    if (md) {
                        aom_img_metadata_free(md);
                    }
                }
            }
        }
        else if (op_type == 5) { // aom_img_metadata_alloc
            // Same as op_type 4 but don't free immediately
            if (offset + 4 < Size) {
                uint32_t type = 0;
                if (offset + 4 <= Size) {
                    memcpy(&type, Data + offset, 4);
                    offset += 4;
                }
                
                uint8_t sz = 0;
                if (offset < Size) {
                    sz = Data[offset];
                    offset++;
                }
                
                uint8_t flag_val = 0;
                if (offset < Size) {
                    flag_val = Data[offset] % 19;
                    offset++;
                }
                
                aom_metadata_insert_flags_t flag = (aom_metadata_insert_flags_t)flag_val;
                
                const uint8_t *payload = NULL;
                if (sz > 0 && offset < Size) {
                    size_t available = Size - offset;
                    if (sz > available) sz = available;
                    
                    payload = Data + offset;
                    offset += sz;
                    
                    aom_metadata_t *md = aom_img_metadata_alloc(type, payload, sz, flag);
                    if (md) {
                        // Try to add it to the image
                        int result = aom_img_add_metadata(&img, md->type, md->payload, md->sz, md->insert_flag);
                        (void)result;
                        // Free the allocated metadata (image makes its own copy)
                        aom_img_metadata_free(md);
                    }
                }
            }
        }
    }
    
    // Clean up any remaining metadata
    aom_img_remove_metadata(&img);
    
    return 0;
}