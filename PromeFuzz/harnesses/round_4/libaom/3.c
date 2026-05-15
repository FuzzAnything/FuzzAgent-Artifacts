// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_uleb_decode at aom_integer.c:31:5 in aom_integer.h
// aom_img_add_metadata at aom_image.c:380:5 in aom_image.h
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
// aom_uleb_encode at aom_integer.c:58:5 in aom_integer.h
// aom_uleb_encode_fixed_size at aom_integer.c:79:5 in aom_integer.h
// aom_uleb_size_in_bytes at aom_integer.c:23:8 in aom_integer.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aom/aom_integer.h"
#include "aom/aom_image.h"

static void dummy_image_init(aom_image_t *img) {
  memset(img, 0, sizeof(*img));
  img->fmt = AOM_IMG_FMT_I420;
  img->w = 64;
  img->h = 64;
  img->d_w = 64;
  img->d_h = 64;
  img->r_w = 64;
  img->r_h = 64;
  img->bit_depth = 8;
  img->x_chroma_shift = 1;
  img->y_chroma_shift = 1;
  img->stride[0] = 64;
  img->stride[1] = 32;
  img->stride[2] = 32;
  img->planes[0] = (unsigned char*)malloc(64 * 64);
  img->planes[1] = (unsigned char*)malloc(32 * 32);
  img->planes[2] = (unsigned char*)malloc(32 * 32);
  img->sz = 64 * 64 + 2 * 32 * 32;
  img->bps = 12;
  img->monochrome = 0;
  img->cp = AOM_CICP_CP_BT_709;
  img->tc = AOM_CICP_TC_BT_709;
  img->mc = AOM_CICP_MC_BT_709;
  img->csp = AOM_CSP_VERTICAL;
  img->range = AOM_CR_STUDIO_RANGE;
  img->temporal_id = 0;
  img->spatial_id = 0;
  img->self_allocd = 1;
  img->img_data_owner = 1;
}

static void dummy_image_cleanup(aom_image_t *img) {
  if (img) {
    free(img->planes[0]);
    free(img->planes[1]);
    free(img->planes[2]);
  }
}

int LLVMFuzzerTestOneInput_3(const uint8_t *Data, size_t Size) {
  if (Size < 4) return 0;

  // Test aom_img_add_metadata
  aom_image_t img;
  dummy_image_init(&img);
  
  uint32_t type = *(const uint32_t*)Data;
  size_t data_size = Size > 8 ? Size - 4 : 0;
  const uint8_t *metadata_data = data_size > 0 ? Data + 4 : NULL;
  aom_metadata_insert_flags_t flags[] = {
    AOM_MIF_NON_KEY_FRAME,
    AOM_MIF_KEY_FRAME,
    AOM_MIF_ANY_FRAME,
    AOM_MIF_NON_KEY_FRAME_LAYER_SPECIFIC,
    AOM_MIF_KEY_FRAME_LAYER_SPECIFIC,
    AOM_MIF_ANY_FRAME_LAYER_SPECIFIC
  };
  int flag_idx = type % (sizeof(flags)/sizeof(flags[0]));
  
  aom_img_add_metadata(&img, type, metadata_data, data_size, flags[flag_idx]);
  
  // Test aom_img_num_metadata
  size_t num_meta = aom_img_num_metadata(&img);
  (void)num_meta;
  
  // Test LEB functions
  uint64_t value = 0;
  if (Size >= 12) {
    value = *(const uint64_t*)(Data + 4);
  } else if (Size >= 8) {
    value = *(const uint64_t*)Data;
  }
  
  size_t available = 20;
  uint8_t buffer[20];
  size_t coded_size = 0;
  
  // Test aom_uleb_encode
  aom_uleb_encode(value, available, buffer, &coded_size);
  
  // Test aom_uleb_encode_fixed_size
  size_t pad_to_size = (value % 10) + 1;
  aom_uleb_encode_fixed_size(value, available, pad_to_size, buffer, &coded_size);
  
  // Test aom_uleb_size_in_bytes
  size_t size_bytes = aom_uleb_size_in_bytes(value);
  (void)size_bytes;
  
  // Test aom_uleb_decode
  uint64_t decoded_value = 0;
  size_t decoded_length = 0;
  if (Size > 0) {
    aom_uleb_decode(Data, Size, &decoded_value, &decoded_length);
  }
  
  dummy_image_cleanup(&img);
  return 0;
}