// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_remove_metadata at aom_image.c:412:6 in aom_image.h
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
// aom_img_metadata_alloc at aom_image.c:327:17 in aom_image.h
// aom_img_metadata_free at aom_image.c:345:6 in aom_image.h
// aom_img_add_metadata at aom_image.c:380:5 in aom_image.h
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
// aom_img_get_metadata at aom_image.c:419:23 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>
#include <aom/aomcx.h>
#include <aom/aom_image.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom.h>

int LLVMFuzzerTestOneInput_49(const uint8_t *Data, size_t Size) {
  if (Size < 6) {
    return 0;
  }

  // Initialize image structure
  aom_image_t img;
  memset(&img, 0, sizeof(img));
  
  // Set basic image properties
  img.w = 64;
  img.h = 64;
  img.bit_depth = 8;
  img.d_w = 64;
  img.d_h = 64;
  img.r_w = 64;
  img.r_h = 64;
  img.x_chroma_shift = 1;
  img.y_chroma_shift = 1;
  img.bps = 8;
  img.temporal_id = 0;
  img.spatial_id = 0;
  img.monochrome = 0;
  img.range = AOM_CR_STUDIO_RANGE;
  img.csp = AOM_CSP_COLOCATED;
  img.mc = AOM_CICP_MC_BT_709;
  img.tc = AOM_CICP_TC_BT_709;
  img.cp = AOM_CICP_CP_BT_709;
  img.fmt = AOM_IMG_FMT_I420;

  // Allocate image planes
  size_t y_size = img.w * img.h;
  size_t uv_size = (img.w / 2) * (img.h / 2);
  
  img.planes[0] = (unsigned char *)malloc(y_size);
  img.planes[1] = (unsigned char *)malloc(uv_size);
  img.planes[2] = (unsigned char *)malloc(uv_size);
  
  if (!img.planes[0] || !img.planes[1] || !img.planes[2]) {
    free(img.planes[0]);
    free(img.planes[1]);
    free(img.planes[2]);
    return 0;
  }
  
  img.stride[0] = img.w;
  img.stride[1] = img.w / 2;
  img.stride[2] = img.w / 2;
  img.sz = y_size + 2 * uv_size;
  img.img_data = NULL;
  img.img_data_owner = 0;
  img.self_allocd = 1;
  img.metadata = NULL;
  img.fb_priv = NULL;
  img.user_priv = NULL;

  // Extract parameters from fuzz input safely
  uint32_t type = Data[0] | (Data[1] << 8) | (Data[2] << 16) | (Data[3] << 24);
  aom_metadata_insert_flags_t insert_flag = (aom_metadata_insert_flags_t)(Data[4] & 0x1F);
  size_t metadata_size = (Size > 5) ? (Data[5] % (Size - 5)) : 0;
  const uint8_t *metadata_data = (Size > 6) ? &Data[6] : NULL;

  // Test aom_img_metadata_alloc
  aom_metadata_t *metadata = aom_img_metadata_alloc(type, metadata_data, metadata_size, insert_flag);
  
  if (metadata) {
    // Test aom_img_metadata_free
    aom_img_metadata_free(metadata);
    metadata = NULL;
  }

  // Test aom_img_add_metadata
  if (metadata_size > 0 && metadata_data) {
    int add_result = aom_img_add_metadata(&img, type, metadata_data, metadata_size, insert_flag);
    (void)add_result; // Suppress unused variable warning
  }

  // Test aom_img_num_metadata
  size_t num_metadata = aom_img_num_metadata(&img);
  
  // Test aom_img_get_metadata
  for (size_t i = 0; i < num_metadata && i < 10; i++) {
    const aom_metadata_t *meta = aom_img_get_metadata(&img, i);
    if (meta) {
      // Just access to ensure it's valid
      (void)meta->type;
      (void)meta->payload;
      (void)meta->sz;
      (void)meta->insert_flag;
    }
  }

  // Test aom_img_remove_metadata
  aom_img_remove_metadata(&img);
  
  // Verify metadata was removed
  size_t num_after_remove = aom_img_num_metadata(&img);
  if (num_after_remove != 0) {
    // This shouldn't happen, but handle gracefully
  }

  // Cleanup
  free(img.planes[0]);
  free(img.planes[1]);
  free(img.planes[2]);
  
  return 0;
}