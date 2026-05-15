// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_uleb_size_in_bytes at aom_integer.c:23:8 in aom_integer.h
// aom_uleb_encode at aom_integer.c:58:5 in aom_integer.h
// aom_uleb_encode_fixed_size at aom_integer.c:79:5 in aom_integer.h
// aom_uleb_decode at aom_integer.c:31:5 in aom_integer.h
// aom_img_add_metadata at aom_image.c:380:5 in aom_image.h
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
    memset(img, 0, sizeof(aom_image_t));
    img->fmt = AOM_IMG_FMT_I420;
    img->cp = AOM_CICP_CP_BT_709;
    img->tc = AOM_CICP_TC_BT_709;
    img->mc = AOM_CICP_MC_BT_709;
    img->monochrome = 0;
    img->csp = AOM_CSP_VERTICAL;
    img->range = AOM_CR_STUDIO_RANGE;
    img->w = 640;
    img->h = 480;
    img->bit_depth = 8;
    img->d_w = 640;
    img->d_h = 480;
    img->r_w = 640;
    img->r_h = 480;
    img->x_chroma_shift = 1;
    img->y_chroma_shift = 1;
    img->sz = 0;
    img->bps = 0;
    img->temporal_id = 0;
    img->spatial_id = 0;
    img->user_priv = NULL;
    img->img_data = NULL;
    img->img_data_owner = 0;
    img->self_allocd = 0;
    img->metadata = NULL;
    img->fb_priv = NULL;
}

int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Test aom_uleb_size_in_bytes
    uint64_t value = 0;
    if (Size >= sizeof(uint64_t)) {
        memcpy(&value, Data, sizeof(uint64_t));
    } else {
        memcpy(&value, Data, Size);
    }
    size_t needed_size = aom_uleb_size_in_bytes(value);

    // Test aom_uleb_encode
    uint8_t buffer[10];
    size_t coded_size = 0;
    int ret = aom_uleb_encode(value, sizeof(buffer), buffer, &coded_size);

    // Test aom_uleb_encode_fixed_size
    size_t pad_to_size = (value % 10) + 1;
    if (pad_to_size > sizeof(buffer)) pad_to_size = sizeof(buffer);
    size_t fixed_coded_size = 0;
    int ret_fixed = aom_uleb_encode_fixed_size(value, sizeof(buffer), pad_to_size, buffer, &fixed_coded_size);

    // Test aom_uleb_decode
    uint64_t decoded_value = 0;
    size_t decoded_length = 0;
    int decode_ret = aom_uleb_decode(buffer, sizeof(buffer), &decoded_value, &decoded_length);

    // Test image metadata functions
    aom_image_t img;
    initialize_image(&img);

    // Prepare metadata data
    size_t metadata_size = Size > 100 ? 100 : Size;
    uint8_t *metadata_data = malloc(metadata_size);
    if (metadata_data) {
        memcpy(metadata_data, Data, metadata_size);
        
        // Test aom_img_add_metadata with various flags
        uint32_t type = value & 0xFFFFFFFF;
        aom_metadata_insert_flags_t flags[] = {
            AOM_MIF_NON_KEY_FRAME,
            AOM_MIF_KEY_FRAME,
            AOM_MIF_ANY_FRAME,
            AOM_MIF_NON_KEY_FRAME_LAYER_SPECIFIC,
            AOM_MIF_KEY_FRAME_LAYER_SPECIFIC,
            AOM_MIF_ANY_FRAME_LAYER_SPECIFIC
        };
        
        for (size_t i = 0; i < sizeof(flags)/sizeof(flags[0]); i++) {
            int add_ret = aom_img_add_metadata(&img, type, metadata_data, metadata_size, flags[i]);
            
            // Test aom_img_num_metadata after each addition
            size_t num_meta = aom_img_num_metadata(&img);
            (void)num_meta; // Avoid unused variable warning
        }
        
        free(metadata_data);
    }

    // Note: We cannot free the metadata array because its internal structure
    // is opaque (only forward declared in the header). The library is expected
    // to handle cleanup internally when the image is destroyed properly.
    // For fuzzing purposes, we'll let the image go out of scope without
    // attempting to free the metadata array.

    return 0;
}