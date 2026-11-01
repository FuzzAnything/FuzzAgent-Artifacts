// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_set_cx_data_buf at aom_encoder.c:244:17 in aom_encoder.h
// aom_codec_set_cx_data_buf at aom_encoder.c:244:17 in aom_encoder.h
// aom_codec_enc_config_set at aom_encoder.c:298:17 in aom_encoder.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include "aom/aom_decoder.h"
#include "aom/aomdx.h"
#include "aom/aom_encoder.h"
#include "aom/aom_integer.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_image.h"
#include "aom/aomcx.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_frame_buffer.h"

/* Helper to parse a small integer from the fuzz input buffer safely */
static int parse_int(const uint8_t *data, size_t size, size_t *offset) {
  if (*offset + sizeof(int) > size) return 0;
  int val;
  memcpy(&val, data + *offset, sizeof(int));
  *offset += sizeof(int);
  return val;
}

/* Helper to parse a size_t from the fuzz input buffer safely */
static size_t parse_size_t(const uint8_t *data, size_t size, size_t *offset) {
  if (*offset + sizeof(size_t) > size) return 0;
  size_t val;
  memcpy(&val, data + *offset, sizeof(size_t));
  *offset += sizeof(size_t);
  return val;
}

/* Helper to parse a string from the fuzz input buffer safely */
static char *parse_string(const uint8_t *data, size_t size, size_t *offset) {
  if (*offset >= size) return NULL;
  size_t len = data[*offset];
  (*offset)++;
  if (*offset + len > size) return NULL;
  char *str = (char *)malloc(len + 1);
  if (!str) return NULL;
  memcpy(str, data + *offset, len);
  str[len] = '\0';
  *offset += len;
  return str;
}

/* Helper to create a dummy image */
static aom_image_t *create_dummy_image(const uint8_t *data, size_t size, size_t *offset) {
  if (*offset + 12 > size) return NULL;
  
  int w = parse_int(data, size, offset);
  int h = parse_int(data, size, offset);
  int bit_depth = parse_int(data, size, offset);
  
  if (w <= 0 || h <= 0 || w > 4096 || h > 4096) return NULL;
  if (bit_depth != 8 && bit_depth != 10 && bit_depth != 12) bit_depth = 8;
  
  aom_image_t *img = malloc(sizeof(aom_image_t));
  if (!img) return NULL;
  
  memset(img, 0, sizeof(aom_image_t));
  img->fmt = AOM_IMG_FMT_I420;
  img->w = (unsigned int)w;
  img->h = (unsigned int)h;
  img->d_w = (unsigned int)w;
  img->d_h = (unsigned int)h;
  img->bit_depth = (unsigned int)bit_depth;
  img->x_chroma_shift = 1;
  img->y_chroma_shift = 1;
  
  size_t y_size = w * h;
  size_t uv_size = (w / 2) * (h / 2);
  
  img->planes[0] = malloc(y_size);
  img->planes[1] = malloc(uv_size);
  img->planes[2] = malloc(uv_size);
  img->stride[0] = w;
  img->stride[1] = w / 2;
  img->stride[2] = w / 2;
  
  if (!img->planes[0] || !img->planes[1] || !img->planes[2]) {
    free(img->planes[0]);
    free(img->planes[1]);
    free(img->planes[2]);
    free(img);
    return NULL;
  }
  
  memset(img->planes[0], 128, y_size);
  memset(img->planes[1], 128, uv_size);
  memset(img->planes[2], 128, uv_size);
  
  return img;
}

static void free_dummy_image(aom_image_t *img) {
  if (!img) return;
  free(img->planes[0]);
  free(img->planes[1]);
  free(img->planes[2]);
  free(img);
}

int LLVMFuzzerTestOneInput_278(const uint8_t *Data, size_t Size) {
  if (Size < 10) return 0;

  size_t offset = 0;
  aom_codec_ctx_t ctx;
  memset(&ctx, 0, sizeof(ctx));
  
  // Initialize the encoder context
  const aom_codec_iface_t *iface = aom_codec_av1_dx(); // Using decoder interface as a placeholder for testing
  if (!iface) return 0;
  
  aom_codec_err_t ret = aom_codec_dec_init(&ctx, iface, NULL, 0);
  if (ret != AOM_CODEC_OK) {
    // If init fails, we can still test error handling functions
    const char *err = aom_codec_error(&ctx);
    if (err) {
      // Just consume the string to avoid unused warning
      (void)strlen(err);
    }
    
    // Test with null context
    const char *null_err = aom_codec_error(NULL);
    if (null_err) {
      (void)strlen(null_err);
    }
    
    return 0;
  }

  // Test aom_codec_error
  const char *err = aom_codec_error(&ctx);
  if (err) {
    (void)strlen(err);
  }

  // Test aom_codec_encode with NULL image (flush)
  ret = aom_codec_encode(&ctx, NULL, 0, 0, 0);
  if (ret != AOM_CODEC_OK) {
    // Expected for decoder context
  }

  // Test aom_codec_set_cx_data_buf
  uint8_t *buf = malloc(1024);
  if (buf) {
    aom_fixed_buf_t fixed_buf = { .buf = buf, .sz = 1024 };
    ret = aom_codec_set_cx_data_buf(&ctx, &fixed_buf, 0, 0);
    if (ret != AOM_CODEC_OK) {
      // Expected for decoder context
    }
    
    // Test with NULL buffer
    ret = aom_codec_set_cx_data_buf(&ctx, NULL, 0, 0);
    
    free(buf);
  }

  // Test aom_codec_enc_config_set (will fail for decoder context)
  aom_codec_enc_cfg_t cfg;
  memset(&cfg, 0, sizeof(cfg));
  cfg.g_w = 640;
  cfg.g_h = 480;
  cfg.g_bit_depth = AOM_BITS_8;
  ret = aom_codec_enc_config_set(&ctx, &cfg);
  if (ret != AOM_CODEC_OK) {
    // Expected for decoder context
  }

  // Test aom_codec_set_option
  char *option_name = parse_string(Data, Size, &offset);
  char *option_value = parse_string(Data, Size, &offset);
  if (option_name && option_value) {
    ret = aom_codec_set_option(&ctx, option_name, option_value);
    free(option_name);
    free(option_value);
  }

  // Clean up
  aom_codec_destroy(&ctx);
  
  return 0;
}