// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_uleb_size_in_bytes at aom_integer.c:23:8 in aom_integer.h
// aom_uleb_encode at aom_integer.c:58:5 in aom_integer.h
// aom_uleb_decode at aom_integer.c:31:5 in aom_integer.h
// aom_uleb_encode_fixed_size at aom_integer.c:79:5 in aom_integer.h
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
// aom_img_add_metadata at aom_image.c:380:5 in aom_image.h
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include "aom/aom_decoder.h"
#include "aom/aomdx.h"
#include "aom/aom_image.h"
#include "aom/aom_integer.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_encoder.h"
#include "aom/aomcx.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_frame_buffer.h"

static void free_image(aom_image_t *img) {
    if (img) {
        aom_img_free(img);
    }
}

static aom_image_t *create_test_image(const uint8_t *data, size_t size) {
    if (size < 4) return NULL;
    
    uint32_t w = (data[0] % 1024) + 1;
    uint32_t h = (data[1] % 1024) + 1;
    uint32_t fmt_idx = data[2] % 8;
    uint32_t bit_depth = (data[3] % 5) + 8; // 8, 10, 12, 13, 14
    
    aom_img_fmt_t fmt = AOM_IMG_FMT_I420;
    switch (fmt_idx) {
        case 0: fmt = AOM_IMG_FMT_I420; break;
        case 1: fmt = AOM_IMG_FMT_YV12; break;
        case 2: fmt = AOM_IMG_FMT_I422; break;
        case 3: fmt = AOM_IMG_FMT_I444; break;
        case 4: fmt = AOM_IMG_FMT_NV12; break;
        case 5: fmt = AOM_IMG_FMT_I42016; break;
        case 6: fmt = AOM_IMG_FMT_YV1216; break;
        case 7: fmt = AOM_IMG_FMT_I42216; break;
    }

    aom_image_t *img = aom_img_alloc(NULL, fmt, w, h, 1);
    if (!img) return NULL;
    
    img->bit_depth = bit_depth;
    
    // Calculate available data for planes
    size_t data_offset = 4;
    size_t available_data = (size > data_offset) ? (size - data_offset) : 0;
    
    if (available_data > 0) {
        // Fill Y plane safely
        size_t y_plane_size = (size_t)w * h;
        size_t y_copy_size = (available_data < y_plane_size) ? available_data : y_plane_size;
        
        if (y_copy_size > 0 && img->planes[0] != NULL) {
            memcpy(img->planes[0], data + data_offset, y_copy_size);
        }
        
        // Fill U and V planes if applicable
        if (y_copy_size < y_plane_size) {
            // Not enough data for full Y plane, skip UV
        } else {
            available_data -= y_copy_size;
            data_offset += y_copy_size;
            
            if (fmt != AOM_IMG_FMT_NV12) {
                size_t uv_plane_size = (size_t)w * h / 4;
                size_t uv_copy_size = (available_data < uv_plane_size) ? available_data : uv_plane_size;
                
                if (uv_copy_size > 0 && img->planes[1] != NULL) {
                    memcpy(img->planes[1], data + data_offset, uv_copy_size);
                }
                
                if (uv_copy_size > 0 && img->planes[2] != NULL) {
                    memcpy(img->planes[2], data + data_offset, uv_copy_size);
                }
            } else {
                // NV12: U and V interleaved
                size_t uv_interleaved_size = (size_t)w * h / 2;
                size_t uv_copy_size = (available_data < uv_interleaved_size) ? available_data : uv_interleaved_size;
                
                if (uv_copy_size > 0 && img->planes[1] != NULL) {
                    memcpy(img->planes[1], data + data_offset, uv_copy_size);
                }
            }
        }
    }
    
    return img;
}

int LLVMFuzzerTestOneInput_130(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Test aom_uleb_size_in_bytes
    uint64_t uleb_val = 0;
    if (Size >= 8) {
        memcpy(&uleb_val, Data, 8);
    } else if (Size > 0) {
        uleb_val = Data[0];
    }
    size_t uleb_size = aom_uleb_size_in_bytes(uleb_val);
    (void)uleb_size;

    // Test aom_uleb_encode
    uint8_t encode_buf[16];
    size_t coded_size = 0;
    int ret = aom_uleb_encode(uleb_val, sizeof(encode_buf), encode_buf, &coded_size);
    (void)ret;

    // Test aom_uleb_decode
    uint64_t decoded_val = 0;
    size_t decoded_len = 0;
    // Use the encoded buffer if we have one, otherwise use input data
    const uint8_t *decode_src = (coded_size > 0) ? encode_buf : Data;
    size_t decode_avail = (coded_size > 0) ? coded_size : Size;
    ret = aom_uleb_decode(decode_src, decode_avail, &decoded_val, &decoded_len);
    (void)ret;

    // Test aom_uleb_encode_fixed_size
    uint8_t fixed_buf[16];
    size_t fixed_coded_size = 0;
    size_t pad_size = (Size > 0) ? (Data[0] % 10) + 1 : 5;
    ret = aom_uleb_encode_fixed_size(uleb_val, sizeof(fixed_buf), pad_size, fixed_buf, &fixed_coded_size);
    (void)ret;

    // Test image related functions
    aom_image_t *img = NULL;
    if (Size >= 4) {
        img = create_test_image(Data, Size);
    }

    if (img) {
        // Test aom_img_num_metadata
        size_t num_meta = aom_img_num_metadata(img);
        (void)num_meta;

        // Test aom_img_add_metadata
        if (Size >= 10) {
            uint32_t meta_type = Data[4] | (Data[5] << 8) | (Data[6] << 16) | (Data[7] << 24);
            size_t meta_sz = (Data[8] % 100) + 1;
            aom_metadata_insert_flags_t insert_flag = (aom_metadata_insert_flags_t)(Data[9] % 19);
            
            // Use remaining data as metadata content
            const uint8_t *meta_data = (Size > 10) ? Data + 10 : Data;
            if (Size <= 10) meta_sz = 0;
            
            // Ensure we don't read past the buffer
            size_t actual_meta_sz = (Size > 10) ? (Size - 10) : 0;
            if (meta_sz > actual_meta_sz) meta_sz = actual_meta_sz;
            
            if (meta_sz > 0) {
                ret = aom_img_add_metadata(img, meta_type, meta_data, meta_sz, insert_flag);
                (void)ret;
            }
        }
        
        // Re-check metadata count after adding
        num_meta = aom_img_num_metadata(img);
        (void)num_meta;
        
        free_image(img);
    }

    return 0;
}