// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_add_metadata at aom_image.c:380:5 in aom_image.h
// aom_img_add_metadata at aom_image.c:380:5 in aom_image.h
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_uleb_size_in_bytes at aom_integer.c:23:8 in aom_integer.h
// aom_uleb_encode at aom_integer.c:58:5 in aom_integer.h
// aom_uleb_decode at aom_integer.c:31:5 in aom_integer.h
// aom_uleb_encode_fixed_size at aom_integer.c:79:5 in aom_integer.h
// aom_uleb_decode at aom_integer.c:31:5 in aom_integer.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aomdx.h>
#include <aom/aom_encoder.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_image.h>
#include <aom/aom_decoder.h>
#include <aom/aom.h>
#include <aom/aom_integer.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aom_external_partition.h>

static void cleanup_image(aom_image_t *img) {
    if (img != NULL) {
        aom_img_free(img);
    }
}

static uint32_t get_u32(const uint8_t *d, size_t s, size_t *off) {
    if (*off + 4 > s) return 0;
    uint32_t v = d[*off] | (d[*off+1] << 8) | (d[*off+2] << 16) | (d[*off+3] << 24);
    *off += 4;
    return v;
}

static uint64_t get_u64(const uint8_t *d, size_t s, size_t *off) {
    if (*off + 8 > s) return 0;
    uint64_t v = (uint64_t)d[*off] | ((uint64_t)d[*off+1] << 8) | 
                 ((uint64_t)d[*off+2] << 16) | ((uint64_t)d[*off+3] << 24) |
                 ((uint64_t)d[*off+4] << 32) | ((uint64_t)d[*off+5] << 40) |
                 ((uint64_t)d[*off+6] << 48) | ((uint64_t)d[*off+7] << 56);
    *off += 8;
    return v;
}

int LLVMFuzzerTestOneInput_179(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    size_t off = 0;
    uint32_t w = get_u32(Data, Size, &off);
    uint32_t h = get_u32(Data, Size, &off);
    uint32_t fmt_val = get_u32(Data, Size, &off);
    uint32_t bit_depth = get_u32(Data, Size, &off);

    // Sanity checks for image dimensions to avoid allocation failures or huge allocations
    if (w == 0 || h == 0 || w > 4096 || h > 4096) {
        return 0;
    }
    if (bit_depth == 0) bit_depth = 8;
    if (bit_depth > 16) bit_depth = 16;

    aom_img_fmt_t fmt = (aom_img_fmt_t)(fmt_val % 10); // Limit to known formats roughly
    if (fmt == AOM_IMG_FMT_NONE) fmt = AOM_IMG_FMT_I420;

    aom_image_t img;
    if (aom_img_alloc(&img, fmt, w, h, bit_depth) != 0) {
        return 0;
    }

    // Fuzz LEB128 functions first as they don't depend on image state
    {
        uint64_t value = get_u64(Data, Size, &off);
        uint8_t buf[16];
        size_t coded_size = 0;
        size_t available = sizeof(buf);
        size_t pad_to_size = (Size % 10) + 1;

        // Test aom_uleb_size_in_bytes
        size_t expected_size = aom_uleb_size_in_bytes(value);
        (void)expected_size; // Use to ensure compilation

        // Test aom_uleb_encode
        if (aom_uleb_encode(value, available, buf, &coded_size) == 0) {
            // Verify decode
            uint64_t decoded_val;
            size_t decoded_len;
            if (aom_uleb_decode(buf, coded_size, &decoded_val, &decoded_len) == 0) {
                if (decoded_val != value) {
                    // Logic error in library, but fuzzing continues
                }
            }
        }

        // Test aom_uleb_encode_fixed_size
        if (pad_to_size <= available) {
            aom_uleb_encode_fixed_size(value, available, pad_to_size, buf, &coded_size);
        }

        // Test aom_uleb_decode with various buffer sizes
        if (off + 10 <= Size) {
            uint64_t d_val;
            size_t d_len;
            for (size_t len = 1; len <= 10; len++) {
                if (off + len <= Size) {
                    aom_uleb_decode(&Data[off], len, &d_val, &d_len);
                }
            }
        }
    }

    // Fuzz image metadata functions
    if (off < Size) {
        uint32_t type = get_u32(Data, Size, &off);
        aom_metadata_insert_flags_t insert_flag = (aom_metadata_insert_flags_t)(Data[off % Size]);
        
        // Prepare metadata data
        size_t data_start = off;
        size_t data_len = (Size - off) > 1024 ? 1024 : (Size - off);
        const uint8_t *data = (data_len > 0) ? &Data[data_start] : NULL;

        // Call aom_img_add_metadata
        if (data_len > 0 && data != NULL) {
            aom_img_add_metadata(&img, type, data, data_len, insert_flag);
        } else {
            // Test with NULL data or 0 size to trigger error paths
            aom_img_add_metadata(&img, type, NULL, 0, insert_flag);
        }

        // Call aom_img_num_metadata
        size_t num_meta = aom_img_num_metadata(&img);
        (void)num_meta; // Use the result
    }

    // Cleanup
    cleanup_image(&img);

    return 0;
}