// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_get_caps at aom_codec.c:84:18 in aom_codec.h
// aom_codec_get_caps at aom_codec.c:84:18 in aom_codec.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_set_frame_buffer_functions at aom_decoder.c:120:17 in aom_decoder.h
// aom_codec_set_frame_buffer_functions at aom_decoder.c:120:17 in aom_decoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_set_frame_buffer_functions at aom_decoder.c:120:17 in aom_decoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_set_frame_buffer_functions at aom_decoder.c:120:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include "aom/aom_codec.h"
#include "aom/aom_encoder.h"
#include "aom/aom_decoder.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_external_partition.h"
#include "aom/aomcx.h"
#include "aom/aom_image.h"
#include "aom/aomdx.h"
#include "aom/aom_integer.h"
#include "aom/aom.h"

/* Helper to extract a C string from fuzz input */
static const char *get_string(const uint8_t *data, size_t size, size_t *offset) {
  if (*offset >= size) return NULL;
  size_t len = 0;
  while (*offset + len < size && data[*offset + len]) {
    len++;
  }
  if (*offset + len >= size) return NULL;
  *offset += len + 1;
  return (const char *)&data[*offset - len - 1];
}

/* Helper to extract a 32-bit unsigned integer */
static uint32_t get_uint32(const uint8_t *data, size_t size, size_t *offset) {
  if (*offset + 4 > size) return 0;
  uint32_t val = 0;
  val |= (uint32_t)data[*offset] << 0;
  val |= (uint32_t)data[*offset + 1] << 8;
  val |= (uint32_t)data[*offset + 2] << 16;
  val |= (uint32_t)data[*offset + 3] << 24;
  *offset += 4;
  return val;
}

/* Helper to extract a 64-bit integer */
static int64_t get_int64(const uint8_t *data, size_t size, size_t *offset) {
  if (*offset + 8 > size) return 0;
  int64_t val = 0;
  for (int i = 0; i < 8; i++) {
    val |= ((int64_t)data[*offset + i]) << (i * 8);
  }
  *offset += 8;
  return val;
}

/* Simple frame buffer callback implementations for fuzzing */
static int fuzz_get_frame_buffer(void *cb_priv, unsigned long size,
                                 aom_codec_frame_buffer_t *fb) {
  (void)cb_priv;
  (void)size;
  (void)fb;
  /* Return failure to simulate lack of buffers or success depending on fuzz logic */
  return 0; 
}

static int fuzz_release_frame_buffer(void *cb_priv, aom_codec_frame_buffer_t *fb) {
  (void)cb_priv;
  (void)fb;
  return 0;
}

static void cleanup_ctx(aom_codec_ctx_t *ctx) {
  if (ctx) {
    aom_codec_destroy(ctx);
  }
}

static void cleanup_image(aom_image_t *img) {
  if (img) {
    aom_img_free(img);
  }
}

int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
  if (Size < 2) return 0;

  size_t offset = 0;
  uint8_t mode = Data[offset++];
  uint8_t action = Data[offset++];

  aom_codec_iface_t *iface = NULL;
  aom_codec_ctx_t ctx = {0};
  aom_codec_err_t ret;
  aom_codec_caps_t caps;

  /* Initialize interface based on mode */
  if (mode == 0) {
    iface = aom_codec_av1_dx();
  } else if (mode == 1) {
    iface = aom_codec_av1_cx();
  } else {
    /* Invalid mode, test with NULL interface */
    caps = aom_codec_get_caps(NULL);
    (void)caps;
    return 0;
  }

  if (!iface) return 0;

  /* Test aom_codec_get_caps */
  caps = aom_codec_get_caps(iface);
  (void)caps;

  /* Initialize context */
  if (mode == 0) {
    /* Decoder */
    aom_codec_dec_cfg_t cfg = {0};
    cfg.threads = 1;
    /* Note: intra_only and error_resilient are not standard members in all versions, 
       relying on default init if they don't exist. */
    
    ret = aom_codec_dec_init(&ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
      cleanup_ctx(&ctx);
      return 0;
    }
  } else {
    /* Encoder */
    aom_codec_enc_cfg_t cfg = {0};
    aom_codec_enc_config_default(iface, &cfg, 0);
    cfg.g_w = 64;
    cfg.g_h = 64;
    cfg.rc_target_bitrate = 100;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    
    ret = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
      cleanup_ctx(&ctx);
      return 0;
    }
  }

  /* Fuzz actions */
  switch (action) {
    case 0: {
      /* Test aom_codec_set_option */
      const char *name = get_string(Data, Size, &offset);
      const char *value = get_string(Data, Size, &offset);
      if (name && value) {
        ret = aom_codec_set_option(&ctx, name, value);
        (void)ret;
      } else {
        /* Test with NULL or partial strings */
        ret = aom_codec_set_option(&ctx, name, value);
        (void)ret;
      }
      break;
    }
    case 1: {
      /* Test aom_codec_set_frame_buffer_functions */
      /* Only valid for decoders */
      if (mode == 0) {
        ret = aom_codec_set_frame_buffer_functions(&ctx,
            fuzz_get_frame_buffer, fuzz_release_frame_buffer, NULL);
        (void)ret;
      } else {
        /* Test with encoder (should fail or be ignored) */
        ret = aom_codec_set_frame_buffer_functions(&ctx,
            fuzz_get_frame_buffer, fuzz_release_frame_buffer, NULL);
        (void)ret;
      }
      break;
    }
    case 2: {
      /* Test aom_codec_encode (only for encoders) */
      if (mode == 1) {
        uint32_t w = get_uint32(Data, Size, &offset);
        uint32_t h = get_uint32(Data, Size, &offset);
        uint32_t bit_depth = get_uint32(Data, Size, &offset);
        int64_t pts = get_int64(Data, Size, &offset);
        unsigned long duration = get_uint32(Data, Size, &offset);
        aom_enc_frame_flags_t flags = get_uint32(Data, Size, &offset);

        if (w == 0) w = 64;
        if (h == 0) h = 64;
        if (bit_depth == 0) bit_depth = 8;
        if (duration == 0) duration = 1;

        /* aom_img_alloc signature: (img, fmt, d_w, d_h, align) */
        aom_image_t *img = aom_img_alloc(NULL, AOM_IMG_FMT_I420, w, h, 1);
        if (img) {
          img->bit_depth = bit_depth;
          /* Fill image with fuzz data */
          size_t plane_size = img->planes[0] ? img->stride[0] * h : 0;
          if (plane_size > 0 && Size > offset) {
            size_t copy_size = (Size - offset < plane_size) ? Size - offset : plane_size;
            memcpy(img->planes[0], &Data[offset], copy_size);
          }
          
          ret = aom_codec_encode(&ctx, img, pts, duration, flags);
          (void)ret;
          
          /* Flush encoder */
          ret = aom_codec_encode(&ctx, NULL, 0, 0, 0);
          (void)ret;
          
          cleanup_image(img);
        }
      }
      break;
    }
    case 3: {
      /* Test with NULL context */
      ret = aom_codec_set_option(NULL, "test", "value");
      (void)ret;
      ret = aom_codec_set_frame_buffer_functions(NULL, NULL, NULL, NULL);
      (void)ret;
      ret = aom_codec_encode(NULL, NULL, 0, 0, 0);
      (void)ret;
      break;
    }
    case 4: {
      /* Test with uninitialized context */
      aom_codec_ctx_t uninit_ctx = {0};
      ret = aom_codec_set_option(&uninit_ctx, "test", "value");
      (void)ret;
      ret = aom_codec_set_frame_buffer_functions(&uninit_ctx, NULL, NULL, NULL);
      (void)ret;
      break;
    }
    default:
      break;
  }

  cleanup_ctx(&ctx);
  return 0;
}