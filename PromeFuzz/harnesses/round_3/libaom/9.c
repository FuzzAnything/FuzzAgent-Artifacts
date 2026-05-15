// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_add_metadata at aom_image.c:380:5 in aom_image.h
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
// aom_uleb_size_in_bytes at aom_integer.c:23:8 in aom_integer.h
// aom_uleb_encode at aom_integer.c:58:5 in aom_integer.h
// aom_uleb_encode_fixed_size at aom_integer.c:79:5 in aom_integer.h
// aom_uleb_decode at aom_integer.c:31:5 in aom_integer.h
// aom_uleb_decode at aom_integer.c:31:5 in aom_integer.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom/aom_image.h"
#include "aom/aom_integer.h"
#include <stdlib.h>
#include <string.h>

static void init_dummy_image(aom_image_t *img) {
    memset(img, 0, sizeof(*img));
    img->fmt = AOM_IMG_FMT_I420;
    img->cp = AOM_CICP_CP_BT_709;
    img->tc = AOM_CICP_TC_BT_709;
    img->mc = AOM_CICP_MC_BT_709;
    img->range = AOM_CR_STUDIO_RANGE;
    img->csp = AOM_CSP_VERTICAL;
    img->w = 16;
    img->h = 16;
    img->d_w = 16;
    img->d_h = 16;
    img->r_w = 16;
    img->r_h = 16;
    img->bit_depth = 8;
    img->x_chroma_shift = 1;
    img->y_chroma_shift = 1;
    img->bps = 12;
    img->stride[0] = 16;
    img->stride[1] = 8;
    img->stride[2] = 8;
    img->metadata = NULL;
}

int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    // Seed for deterministic behavior
    unsigned int seed = 0;
    if (Size >= 4) {
        seed = *(const unsigned int*)Data;
    }

    // Use first byte to decide which functions to test
    uint8_t selector = Data[0];
    size_t data_offset = 1;

    // Test aom_uleb_size_in_bytes
    if (Size >= data_offset + 8) {
        uint64_t value = *(const uint64_t*)(Data + data_offset);
        (void)aom_uleb_size_in_bytes(value);
        data_offset += 8;
    }

    // Test aom_uleb_encode
    if (Size >= data_offset + 8 + 1) {
        uint64_t value = *(const uint64_t*)(Data + data_offset);
        size_t available = Data[data_offset + 8] % 20 + 1; // 1-20 bytes
        uint8_t buffer[20];
        size_t coded_size = 0;
        (void)aom_uleb_encode(value, available, buffer, &coded_size);
        data_offset += 9;
    }

    // Test aom_uleb_encode_fixed_size
    if (Size >= data_offset + 8 + 2) {
        uint64_t value = *(const uint64_t*)(Data + data_offset);
        size_t available = Data[data_offset + 8] % 20 + 1;
        size_t pad_to_size = Data[data_offset + 9] % 20 + 1;
        uint8_t buffer[20];
        size_t coded_size = 0;
        (void)aom_uleb_encode_fixed_size(value, available, pad_to_size, buffer, &coded_size);
        data_offset += 10;
    }

    // Test aom_uleb_decode - FIXED: Ensure we don't read beyond available data
    if (Size >= data_offset + 1) {
        size_t decode_available = Data[data_offset] % 20 + 1;
        // Ensure we have enough data for the buffer
        if (Size >= data_offset + 1 + decode_available) {
            const uint8_t *buffer = Data + data_offset + 1;
            uint64_t decoded_value = 0;
            size_t decoded_length = 0;
            (void)aom_uleb_decode(buffer, decode_available, &decoded_value, &decoded_length);
            data_offset += 1 + decode_available;
        } else {
            // Not enough data for decode_available, use what's available
            size_t remaining = Size - (data_offset + 1);
            if (remaining > 0) {
                const uint8_t *buffer = Data + data_offset + 1;
                uint64_t decoded_value = 0;
                size_t decoded_length = 0;
                (void)aom_uleb_decode(buffer, remaining, &decoded_value, &decoded_length);
            }
            data_offset = Size; // Consume all remaining data
        }
    }

    // Test image metadata functions
    if ((selector & 0x1) && Size >= data_offset + 6) {
        aom_image_t img;
        init_dummy_image(&img);

        uint32_t type = 0;
        if (Size >= data_offset + 4) {
            type = *(const uint32_t*)(Data + data_offset);
            data_offset += 4;
        }

        size_t metadata_size = 0;
        if (Size >= data_offset + 2) {
            metadata_size = *(const uint16_t*)(Data + data_offset) % 256;
            data_offset += 2;
        }

        const uint8_t *metadata_data = NULL;
        if (metadata_size > 0 && Size >= data_offset + metadata_size) {
            metadata_data = Data + data_offset;
            data_offset += metadata_size;
        } else {
            metadata_size = 0;
        }

        aom_metadata_insert_flags_t flags = AOM_MIF_ANY_FRAME;
        if (metadata_size > 0) {
            (void)aom_img_add_metadata(&img, type, metadata_data, metadata_size, flags);
        }

        (void)aom_img_num_metadata(&img);

        // Cleanup
        if (img.metadata) {
            free(img.metadata);
        }
    }

    return 0;
}