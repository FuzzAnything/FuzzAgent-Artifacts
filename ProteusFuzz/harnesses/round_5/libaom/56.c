// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_add_metadata at aom_image.c:380:5 in aom_image.h
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_uleb_size_in_bytes at aom_integer.c:23:8 in aom_integer.h
// aom_uleb_encode at aom_integer.c:58:5 in aom_integer.h
// aom_uleb_decode at aom_integer.c:31:5 in aom_integer.h
// aom_uleb_encode_fixed_size at aom_integer.c:79:5 in aom_integer.h
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

#include "aomcx.h"
#include "aomdx.h"
#include "aom.h"
#include "aom_codec.h"
#include "aom_external_partition.h"
#include "aom_decoder.h"
#include "aom_image.h"
#include "aom_frame_buffer.h"
#include "aom_encoder.h"
#include "aom_integer.h"

static void fuzz_uleb_functions(const uint8_t *Data, size_t Size) {
    if (Size < 2) return;

    // Extract value for encoding/decoding tests
    uint64_t value = 0;
    if (Size >= sizeof(uint64_t)) {
        memcpy(&value, Data, sizeof(uint64_t));
    } else {
        memcpy(&value, Data, Size);
    }

    // Test aom_uleb_size_in_bytes
    size_t expected_size = aom_uleb_size_in_bytes(value);

    // Prepare buffer for encoding
    uint8_t encode_buf[16];
    size_t coded_size = 0;
    int ret = aom_uleb_encode(value, sizeof(encode_buf), encode_buf, &coded_size);
    (void)ret; // Ignore return for fuzzing coverage, but logic is exercised

    // Test aom_uleb_decode
    uint64_t decoded_value = 0;
    size_t decoded_len = 0;
    // Use the encoded buffer if successful, otherwise use fuzz data
    const uint8_t *decode_buf = (ret == 0) ? encode_buf : Data;
    size_t decode_avail = (ret == 0) ? coded_size : Size;
    ret = aom_uleb_decode(decode_buf, decode_avail, &decoded_value, &decoded_len);
    (void)ret;

    // Test aom_uleb_encode_fixed_size
    size_t pad_size = (Size > 10) ? 10 : (Size % 10) + 1;
    uint8_t fixed_buf[16];
    size_t fixed_coded_size = 0;
    ret = aom_uleb_encode_fixed_size(value, sizeof(fixed_buf), pad_size, fixed_buf, &fixed_coded_size);
    (void)ret;
}

static void fuzz_image_metadata(const uint8_t *Data, size_t Size) {
    if (Size < 1) return;

    // Initialize image structure
    aom_image_t img;
    memset(&img, 0, sizeof(img));

    // Set basic valid parameters to avoid immediate NULL checks failing logic paths
    img.fmt = AOM_IMG_FMT_I420;
    img.w = 16;
    img.h = 16;
    img.bit_depth = 8;
    img.d_w = 16;
    img.d_h = 16;
    img.r_w = 16;
    img.r_h = 16;
    img.x_chroma_shift = 1;
    img.y_chroma_shift = 1;
    img.temporal_id = 0;
    img.spatial_id = 0;
    img.metadata = NULL;

    // Test aom_img_num_metadata on empty image
    size_t num_meta = aom_img_num_metadata(&img);
    (void)num_meta;

    // Prepare metadata data from fuzz input
    uint32_t type = 0;
    size_t data_size = 0;
    const uint8_t *data_ptr = NULL;
    aom_metadata_insert_flags_t insert_flag = AOM_MIF_ANY_FRAME;

    if (Size >= 4) {
        memcpy(&type, Data, sizeof(uint32_t));
        type = type % 256; // Limit type range slightly
        Data += 4;
        Size -= 4;
    }

    if (Size >= 4) {
        memcpy(&data_size, Data, sizeof(uint32_t));
        data_size = data_size % 4096; // Limit size to prevent huge allocations
        Data += 4;
        Size -= 4;
    }

    if (Size >= 4) {
        uint32_t flag_val;
        memcpy(&flag_val, Data, sizeof(uint32_t));
        insert_flag = (aom_metadata_insert_flags_t)(flag_val % 19); // Valid range
        Data += 4;
        Size -= 4;
    }

    if (data_size > 0 && Size >= data_size) {
        data_ptr = Data;
    } else if (data_size > 0) {
        // If not enough data, allocate dummy
        data_ptr = malloc(data_size);
        if (data_ptr) {
            memset(data_ptr, 0, data_size);
        }
    }

    // Test aom_img_add_metadata
    int ret = aom_img_add_metadata(&img, type, data_ptr, data_size, insert_flag);
    (void)ret;

    // Test aom_img_num_metadata after addition
    num_meta = aom_img_num_metadata(&img);
    (void)num_meta;

    // Cleanup: Free allocated metadata data if we allocated it
    if (data_size > 0 && Size < data_size) {
        free((void*)data_ptr);
    }

    // Mandatory cleanup for image structure as per instructions
    aom_img_free(&img);
}

int LLVMFuzzerTestOneInput_56(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Fuzz LEB128 related functions
    fuzz_uleb_functions(Data, Size);

    // Fuzz Image Metadata related functions
    fuzz_image_metadata(Data, Size);

    return 0;
}