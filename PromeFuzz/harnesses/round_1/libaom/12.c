// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_uleb_size_in_bytes at aom_integer.c:23:8 in aom_integer.h
// aom_uleb_encode at aom_integer.c:58:5 in aom_integer.h
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
// aom_img_get_metadata at aom_image.c:419:23 in aom_image.h
// aom_img_get_metadata at aom_image.c:419:23 in aom_image.h
// aom_img_get_metadata at aom_image.c:419:23 in aom_image.h
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
// aom_uleb_decode at aom_integer.c:31:5 in aom_integer.h
// aom_uleb_encode_fixed_size at aom_integer.c:79:5 in aom_integer.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom/aom_integer.h"
#include "aom/aom_image.h"

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
    img->range = AOM_CR_STUDIO_RANGE;
    img->csp = AOM_CSP_VERTICAL;
    img->w = 640;
    img->h = 480;
    img->d_w = 640;
    img->d_h = 480;
    img->r_w = 640;
    img->r_h = 480;
    img->bit_depth = 8;
    img->x_chroma_shift = 1;
    img->y_chroma_shift = 1;
    img->bps = 12;
    img->temporal_id = 0;
    img->spatial_id = 0;
    
    // Don't initialize metadata array since it's an incomplete type
    // Just set it to NULL
    img->metadata = NULL;
}

int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    uint64_t value = 0;
    if (Size >= sizeof(uint64_t)) {
        memcpy(&value, Data, sizeof(uint64_t));
    } else {
        for (size_t i = 0; i < Size; i++) {
            value |= ((uint64_t)Data[i] << (8 * i));
        }
    }
    
    size_t uleb_size = aom_uleb_size_in_bytes(value);
    
    uint8_t *buffer = malloc(Size + 10);
    if (!buffer) return 0;
    
    if (Size > 0) {
        memcpy(buffer, Data, Size);
    }
    
    size_t coded_size = 0;
    int encode_result = aom_uleb_encode(value, Size + 10, buffer, &coded_size);
    
    aom_image_t img;
    init_dummy_image(&img);
    
    size_t num_metadata = aom_img_num_metadata(&img);
    
    if (num_metadata > 0) {
        const aom_metadata_t *metadata = aom_img_get_metadata(&img, 0);
        if (metadata) {
            size_t index = value % (num_metadata + 1);
            aom_img_get_metadata(&img, index);
        }
    }
    
    aom_img_get_metadata(NULL, 0);
    aom_img_num_metadata(NULL);
    
    uint64_t decoded_value = 0;
    size_t decoded_length = 0;
    int decode_result = aom_uleb_decode(Data, Size, &decoded_value, &decoded_length);
    
    size_t fixed_coded_size = 0;
    size_t pad_to_size = value % 11;
    if (pad_to_size == 0) pad_to_size = 1;
    
    int fixed_encode_result = aom_uleb_encode_fixed_size(
        value, Size + 10, pad_to_size, buffer, &fixed_coded_size);
    
    free(buffer);
    
    return 0;
}