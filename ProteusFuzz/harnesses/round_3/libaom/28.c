// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
// aom_img_add_metadata at aom_image.c:380:5 in aom_image.h
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_uleb_decode at aom_integer.c:31:5 in aom_integer.h
// aom_uleb_size_in_bytes at aom_integer.c:23:8 in aom_integer.h
// aom_uleb_encode at aom_integer.c:58:5 in aom_integer.h
// aom_uleb_encode_fixed_size at aom_integer.c:79:5 in aom_integer.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_image.h>
#include <aom/aom_integer.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom_codec.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_frame_buffer.h>

static void cleanup_image(aom_image_t *img) {
  if (img != NULL) {
    aom_img_free(img);
  }
}

int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
  if (Size == 0) {
    return 0;
  }

  // Buffer for LEB128 operations
  uint8_t buffer[64];
  size_t available = sizeof(buffer);
  uint64_t value;
  size_t length;
  size_t coded_size;
  int ret;

  // 1. Fuzz aom_uleb_decode
  // Try decoding from the input data directly if size allows
  if (Size >= 1) {
    ret = aom_uleb_decode(Data, Size, &value, &length);
    (void)ret; // Ignore return for fuzzing stability, just ensure no crash
  }

  // 2. Fuzz aom_uleb_size_in_bytes
  // Use input data to derive a value
  uint64_t test_val = 0;
  if (Size >= sizeof(uint64_t)) {
    memcpy(&test_val, Data, sizeof(uint64_t));
  } else if (Size > 0) {
    memcpy(&test_val, Data, Size);
  }
  size_t size_needed = aom_uleb_size_in_bytes(test_val);
  (void)size_needed;

  // 3. Fuzz aom_uleb_encode
  // Ensure buffer is large enough for max LEB128 (10 bytes)
  if (available >= 10) {
    ret = aom_uleb_encode(test_val, available, buffer, &coded_size);
    (void)ret;
  }

  // 4. Fuzz aom_uleb_encode_fixed_size
  // Use a small pad size derived from input to test edge cases
  size_t pad_to = (Size > 0) ? (Data[0] % 10) + 1 : 1;
  if (available >= pad_to) {
    ret = aom_uleb_encode_fixed_size(test_val, available, pad_to, buffer, &coded_size);
    (void)ret;
  }

  // 5. Fuzz aom_img_num_metadata and aom_img_add_metadata
  // Initialize a dummy image
  aom_image_t img;
  memset(&img, 0, sizeof(img));
  
  // Set basic valid fields to prevent immediate NULL checks in some paths
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
  img.stride[0] = 16;
  img.stride[1] = 8;
  img.stride[2] = 8;
  img.planes[0] = malloc(16 * 16);
  img.planes[1] = malloc(8 * 8);
  img.planes[2] = malloc(8 * 8);
  img.sz = 16 * 16 + 8 * 8 + 8 * 8;
  img.img_data = NULL;
  img.img_data_owner = 0;
  img.self_allocd = 0;
  img.metadata = NULL;

  if (img.planes[0] && img.planes[1] && img.planes[2]) {
    // Test aom_img_num_metadata with NULL metadata
    size_t num_meta = aom_img_num_metadata(&img);
    (void)num_meta;

    // Test aom_img_add_metadata
    // Prepare metadata data from input
    size_t meta_sz = (Size > 100) ? 100 : Size;
    const uint8_t *meta_data = (Size > 0) ? Data : (const uint8_t *)"\0";
    if (meta_sz == 0) meta_sz = 1;
    if (meta_data == NULL) meta_data = (const uint8_t *)"\0";

    uint32_t meta_type = (Size > 0) ? Data[0] : 0;
    aom_metadata_insert_flags_t insert_flag = AOM_MIF_ANY_FRAME;
    if (Size > 1) {
      int flag_idx = Data[1] % 6; // 0 to 5
      switch (flag_idx) {
        case 0: insert_flag = AOM_MIF_NON_KEY_FRAME; break;
        case 1: insert_flag = AOM_MIF_KEY_FRAME; break;
        case 2: insert_flag = AOM_MIF_ANY_FRAME; break;
        case 3: insert_flag = AOM_MIF_NON_KEY_FRAME_LAYER_SPECIFIC; break;
        case 4: insert_flag = AOM_MIF_KEY_FRAME_LAYER_SPECIFIC; break;
        case 5: insert_flag = AOM_MIF_ANY_FRAME_LAYER_SPECIFIC; break;
      }
    }

    ret = aom_img_add_metadata(&img, meta_type, meta_data, meta_sz, insert_flag);
    (void)ret;

    // Test aom_img_num_metadata again after potential addition
    num_meta = aom_img_num_metadata(&img);
    (void)num_meta;
  }

  cleanup_image(&img);

  return 0;
}