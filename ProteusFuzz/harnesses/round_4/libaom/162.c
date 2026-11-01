// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_add_metadata at aom_image.c:380:5 in aom_image.h
// aom_img_add_metadata at aom_image.c:380:5 in aom_image.h
// aom_img_add_metadata at aom_image.c:380:5 in aom_image.h
// aom_img_get_metadata at aom_image.c:419:23 in aom_image.h
// aom_img_num_metadata at aom_image.c:429:8 in aom_image.h
// aom_img_remove_metadata at aom_image.c:412:6 in aom_image.h
// aom_img_metadata_alloc at aom_image.c:327:17 in aom_image.h
// aom_img_metadata_free at aom_image.c:345:6 in aom_image.h
// aom_img_metadata_alloc at aom_image.c:327:17 in aom_image.h
// aom_img_metadata_free at aom_image.c:345:6 in aom_image.h
// aom_img_metadata_free at aom_image.c:345:6 in aom_image.h
// aom_img_remove_metadata at aom_image.c:412:6 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "aom/aomdx.h"
#include "aom/aom_encoder.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_image.h"
#include "aom/aom_decoder.h"
#include "aom/aom.h"
#include "aom/aom_integer.h"
#include "aom/aom_codec.h"
#include "aom/aomcx.h"
#include "aom/aom_external_partition.h"

static void init_image(aom_image_t *img) {
  img->fmt = AOM_IMG_FMT_I420;
  img->w = 64;
  img->h = 64;
  img->bit_depth = 8;
  img->d_w = 64;
  img->d_h = 64;
  img->r_w = 64;
  img->r_h = 64;
  img->x_chroma_shift = 1;
  img->y_chroma_shift = 1;
  img->monochrome = 0;
  img->cp = AOM_CICP_CP_BT_709;
  img->tc = AOM_CICP_TC_BT_709;
  img->mc = AOM_CICP_MC_BT_709;
  img->range = AOM_CR_STUDIO_RANGE;
  img->csp = AOM_CSP_COLOCATED;
  img->bps = 8;
  img->temporal_id = 0;
  img->spatial_id = 0;
  img->user_priv = NULL;
  img->img_data = NULL;
  img->img_data_owner = 0;
  img->self_allocd = 0;
  img->metadata = NULL;
  img->fb_priv = NULL;

  // Allocate planes
  size_t stride_y = img->w;
  size_t stride_uv = img->w / 2;
  size_t plane_size_y = stride_y * img->h;
  size_t plane_size_uv = stride_uv * (img->h / 2);

  img->planes[0] = malloc(plane_size_y);
  img->stride[0] = stride_y;
  img->planes[1] = malloc(plane_size_uv);
  img->stride[1] = stride_uv;
  img->planes[2] = malloc(plane_size_uv);
  img->stride[2] = stride_uv;

  if (img->planes[0]) memset(img->planes[0], 128, plane_size_y);
  if (img->planes[1]) memset(img->planes[1], 128, plane_size_uv);
  if (img->planes[2]) memset(img->planes[2], 128, plane_size_uv);

  img->sz = plane_size_y + 2 * plane_size_uv;
}

static void free_image(aom_image_t *img) {
  if (img->planes[0]) free(img->planes[0]);
  if (img->planes[1]) free(img->planes[1]);
  if (img->planes[2]) free(img->planes[2]);
  img->planes[0] = NULL;
  img->planes[1] = NULL;
  img->planes[2] = NULL;
  img->stride[0] = 0;
  img->stride[1] = 0;
  img->stride[2] = 0;
  img->sz = 0;
}

int LLVMFuzzerTestOneInput_162(const uint8_t *Data, size_t Size) {
  if (Size < 1) return 0;

  aom_image_t img;
  init_image(&img);

  // Parse input to determine operations
  uint8_t op = Data[0] % 6;
  size_t offset = 1;
  size_t remaining = Size - offset;

  if (op == 0) {
    // Test aom_img_add_metadata
    if (remaining >= 8) {
      uint32_t type = (uint32_t)Data[offset];
      offset++;
      aom_metadata_insert_flags_t flag = (aom_metadata_insert_flags_t)(Data[offset] % 19);
      offset++;
      size_t sz = (remaining - offset) > 0 ? (remaining - offset) : 0;
      if (sz > 1024) sz = 1024; // Limit size
      const uint8_t *data = offset < Size ? &Data[offset] : NULL;

      if (data && sz > 0) {
        aom_img_add_metadata(&img, type, data, sz, flag);
      } else {
        // Test with NULL or 0 size to trigger error paths
        aom_img_add_metadata(&img, type, NULL, 0, flag);
      }
    } else {
      aom_img_add_metadata(&img, 0, NULL, 0, AOM_MIF_ANY_FRAME);
    }
  } else if (op == 1) {
    // Test aom_img_get_metadata
    size_t index = 0;
    if (remaining > 0) {
      index = Data[offset] % 100;
    }
    const aom_metadata_t *meta = aom_img_get_metadata(&img, index);
    (void)meta; // Use result to avoid unused warning
  } else if (op == 2) {
    // Test aom_img_num_metadata
    size_t count = aom_img_num_metadata(&img);
    (void)count;
  } else if (op == 3) {
    // Test aom_img_remove_metadata
    aom_img_remove_metadata(&img);
  } else if (op == 4) {
    // Test aom_img_metadata_alloc
    if (remaining >= 4) {
      uint32_t type = (uint32_t)Data[offset];
      offset++;
      aom_metadata_insert_flags_t flag = (aom_metadata_insert_flags_t)(Data[offset] % 19);
      offset++;
      size_t sz = (remaining - offset) > 0 ? (remaining - offset) : 0;
      if (sz > 1024) sz = 1024;
      const uint8_t *data = offset < Size ? &Data[offset] : NULL;

      aom_metadata_t *meta = aom_img_metadata_alloc(type, data, sz, flag);
      if (meta) {
        aom_img_metadata_free(meta);
      }
    } else {
      aom_metadata_t *meta = aom_img_metadata_alloc(0, NULL, 0, AOM_MIF_ANY_FRAME);
      if (meta) {
        aom_img_metadata_free(meta);
      }
    }
  } else if (op == 5) {
    // Test aom_img_metadata_free with NULL
    aom_img_metadata_free(NULL);
  }

  // Cleanup
  aom_img_remove_metadata(&img);
  free_image(&img);

  return 0;
}