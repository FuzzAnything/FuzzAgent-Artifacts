// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_error_detail at aom_codec.c:61:13 in aom_codec.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_error_detail at aom_codec.c:61:13 in aom_codec.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aomcx.h"
#include "aom/aomdx.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_image.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_integer.h"

static int parse_uint8(const uint8_t *data, size_t size, size_t *offset) {
  if (*offset + 1 > size) return 0;
  return *(data + (*offset)++);
}

static int parse_uint32(const uint8_t *data, size_t size, size_t *offset) {
  if (*offset + 4 > size) return 0;
  uint32_t val = 0;
  val |= (uint32_t)data[*offset] << 0;
  val |= (uint32_t)data[*offset + 1] << 8;
  val |= (uint32_t)data[*offset + 2] << 16;
  val |= (uint32_t)data[*offset + 3] << 24;
  *offset += 4;
  return val;
}

static int parse_int64(const uint8_t *data, size_t size, size_t *offset) {
  if (*offset + 8 > size) return 0;
  int64_t val = 0;
  for (int i = 0; i < 8; i++) {
    val |= (int64_t)data[*offset + i] << (i * 8);
  }
  *offset += 8;
  return val;
}

static void fill_image_with_data(aom_image_t *img, const uint8_t *data, size_t size) {
  if (!img || !img->planes[0]) return;
  size_t plane_size = img->stride[0] * img->h;
  size_t total_size = plane_size;
  if (img->planes[1]) total_size += img->stride[1] * img->h;
  if (img->planes[2]) total_size += img->stride[2] * img->h;

  if (size < total_size) {
    memset(img->planes[0], 0, total_size);
    if (img->planes[1]) memset(img->planes[1], 0, img->stride[1] * img->h);
    if (img->planes[2]) memset(img->planes[2], 0, img->stride[2] * img->h);
  } else {
    memcpy(img->planes[0], data, plane_size);
    if (img->planes[1]) memcpy(img->planes[1], data + plane_size, img->stride[1] * img->h);
    if (img->planes[2]) memcpy(img->planes[2], data + plane_size + (img->stride[1] * img->h), img->stride[2] * img->h);
  }
}

int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
  if (Size < 10) return 0;

  size_t offset = 0;
  int width = parse_uint32(Data, Size, &offset);
  int height = parse_uint32(Data, Size, &offset);
  int bit_depth = parse_uint8(Data, Size, &offset);
  int img_fmt_idx = parse_uint8(Data, Size, &offset);
  int option_count = parse_uint8(Data, Size, &offset);

  // Sanitize dimensions
  if (width == 0 || height == 0 || width > 4096 || height > 4096) {
    width = 64;
    height = 64;
  }
  if (bit_depth == 0) bit_depth = 8;
  if (bit_depth > 12) bit_depth = 8;

  aom_img_fmt_t img_fmt = AOM_IMG_FMT_NONE;
  switch (img_fmt_idx % 5) {
    case 0: img_fmt = AOM_IMG_FMT_I420; break;
    case 1: img_fmt = AOM_IMG_FMT_YV12; break;
    case 2: img_fmt = AOM_IMG_FMT_I42016; break;
    case 3: img_fmt = AOM_IMG_FMT_YV1216; break;
    case 4: img_fmt = AOM_IMG_FMT_I444; break;
    default: img_fmt = AOM_IMG_FMT_I420;
  }

  if (bit_depth > 8) {
    if (img_fmt == AOM_IMG_FMT_I420) img_fmt = AOM_IMG_FMT_I42016;
    if (img_fmt == AOM_IMG_FMT_YV12) img_fmt = AOM_IMG_FMT_YV1216;
    if (img_fmt == AOM_IMG_FMT_I444) img_fmt = AOM_IMG_FMT_I44416;
  }

  aom_codec_ctx_t ctx;
  aom_codec_iface_t *iface = aom_codec_av1_cx();
  if (!iface) return 0;

  aom_codec_enc_cfg_t cfg;
  aom_codec_enc_config_default(iface, &cfg, 0);
  cfg.g_w = width;
  cfg.g_h = height;
  cfg.g_bit_depth = bit_depth;
  cfg.g_timebase.num = 1;
  cfg.g_timebase.den = 30;
  cfg.rc_target_bitrate = 1000;

  aom_codec_err_t err = aom_codec_enc_init(&ctx, iface, &cfg, 0);
  if (err != AOM_CODEC_OK) return 0;

  // Try setting options
  for (int i = 0; i < option_count && offset < Size; i++) {
    int key_len = parse_uint8(Data, Size, &offset);
    int val_len = parse_uint8(Data, Size, &offset);
    if (offset + key_len + val_len > Size) break;
    char key[256], val[256];
    if (key_len > 255) key_len = 255;
    if (val_len > 255) val_len = 255;
    memcpy(key, Data + offset, key_len);
    key[key_len] = '\0';
    offset += key_len;
    memcpy(val, Data + offset, val_len);
    val[val_len] = '\0';
    offset += val_len;
    
    // Only try safe options to avoid immediate crashes
    if (strcmp(key, "cpu-used") == 0 || strcmp(key, "crf") == 0 || strcmp(key, "lag-in-frames") == 0) {
      aom_codec_set_option(&ctx, key, val);
    }
  }

  aom_image_t img;
  aom_img_alloc(&img, img_fmt, width, height, 16);
  if (!img.planes[0]) {
    aom_codec_destroy(&ctx);
    return 0;
  }

  aom_codec_pts_t pts = 0;
  unsigned long duration = 1;
  int frame_count = 0;
  int max_frames = 10; // Limit to prevent infinite loops

  // Fill image with fuzz data
  fill_image_with_data(&img, Data, Size);

  // Encode some frames
  while (frame_count < max_frames) {
    // Vary flags
    int flag_val = parse_uint8(Data, Size, &offset);
    aom_enc_frame_flags_t flags = (aom_enc_frame_flags_t)flag_val;

    // Vary duration slightly
    if (frame_count > 0) {
      duration = parse_uint8(Data, Size, &offset);
      if (duration == 0) duration = 1;
    }

    err = aom_codec_encode(&ctx, &img, pts, duration, flags);
    
    if (err != AOM_CODEC_OK) {
      // Check error details
      const char *err_str = aom_codec_err_to_string(err);
      (void)err_str; // Use to avoid unused warning
      const char *detail = aom_codec_error_detail(&ctx);
      (void)detail;
      const char *synopsis = aom_codec_error(&ctx);
      (void)synopsis;
    }

    // Collect output data to ensure state changes
    aom_codec_cx_pkt_t *pkt;
    while ((pkt = aom_codec_get_cx_data(&ctx, NULL)) != NULL) {
      aom_codec_destroy(&ctx); // Early exit to test cleanup paths
      aom_img_free(&img);
      return 0;
    }

    pts += duration;
    frame_count++;
  }

  // Flush the encoder
  while (1) {
    err = aom_codec_encode(&ctx, NULL, 0, 0, 0);
    if (err != AOM_CODEC_OK) {
      const char *err_str = aom_codec_err_to_string(err);
      (void)err_str;
      const char *detail = aom_codec_error_detail(&ctx);
      (void)detail;
      const char *synopsis = aom_codec_error(&ctx);
      (void)synopsis;
    }

    aom_codec_cx_pkt_t *pkt = aom_codec_get_cx_data(&ctx, NULL);
    if (!pkt) break;
  }

  aom_codec_destroy(&ctx);
  aom_img_free(&img);

  return 0;
}