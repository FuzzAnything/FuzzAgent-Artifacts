// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aomcx.h>
#include <aom/aom_external_partition.h>

static void free_image(aom_image_t *img) {
  if (img) {
    aom_img_free(img);
  }
}

static aom_codec_ctx_t *init_encoder(const uint8_t *data, size_t size) {
  aom_codec_ctx_t *ctx = NULL;
  aom_codec_iface_t *iface;
  aom_codec_enc_cfg_t cfg;
  int w, h, bit_depth;

  if (!data || size < 12) return NULL;

  // Parse width, height, and bit depth from the first 12 bytes of input
  w = (data[0] << 8) | data[1];
  h = (data[2] << 8) | data[3];
  bit_depth = data[4];

  // Validate dimensions to prevent allocation of excessively large images
  if (w < 1 || w > 4096 || h < 1 || h > 4096) return NULL;
  if (bit_depth < 8 || bit_depth > 12) bit_depth = 8;

  iface = aom_codec_av1_cx();
  if (!iface) return NULL;

  ctx = (aom_codec_ctx_t *)malloc(sizeof(aom_codec_ctx_t));
  if (!ctx) return NULL;
  memset(ctx, 0, sizeof(aom_codec_ctx_t));

  if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
    free(ctx);
    return NULL;
  }

  cfg.g_w = w;
  cfg.g_h = h;
  cfg.g_bit_depth = bit_depth;
  cfg.g_timebase.num = 1;
  cfg.g_timebase.den = 30;
  cfg.rc_target_bitrate = 1000;
  cfg.rc_min_quantizer = 0;
  cfg.rc_max_quantizer = 63;
  cfg.g_profile = 0;

  if (aom_codec_enc_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
    free(ctx);
    return NULL;
  }

  return ctx;
}

static aom_image_t *create_image(const uint8_t *data, size_t size, int w, int h, int bit_depth) {
  aom_image_t *img = NULL;
  aom_img_fmt_t fmt = AOM_IMG_FMT_I420;

  if (bit_depth > 8) {
    fmt = AOM_IMG_FMT_I42016;
  }

  // Allocate image with alignment
  img = aom_img_alloc(NULL, fmt, w, h, 16);
  if (!img) return NULL;

  img->bit_depth = bit_depth;

  // Fill image planes safely
  for (int p = 0; p < 3; p++) {
    int plane_w = w;
    int plane_h = h;
    int stride = img->stride[p];
    
    // Adjust dimensions for chroma planes (4:2:0 subsampling)
    if (p == AOM_PLANE_U || p == AOM_PLANE_V) {
      plane_w = (w + 1) / 2;
      plane_h = (h + 1) / 2;
    }

    // Calculate actual bytes needed for this plane
    size_t plane_bytes = (size_t)stride * plane_h;
    
    // Fill plane with data, ensuring we don't overflow
    for (int y = 0; y < plane_h; y++) {
      for (int x = 0; x < plane_w; x++) {
        // Calculate source index, wrapping around available data
        size_t src_idx = ((size_t)y * (size_t)plane_w + (size_t)x) % size;
        uint8_t val = data[src_idx];
        
        if (bit_depth > 8) {
          // For 10/12 bit, write as 16-bit little endian
          uint16_t *ptr = (uint16_t *)(img->planes[p] + y * stride);
          ptr[x] = (uint16_t)val | ((uint16_t)val << 8);
        } else {
          img->planes[p][y * stride + x] = val;
        }
      }
    }
  }

  return img;
}

int LLVMFuzzerTestOneInput_311(const uint8_t *Data, size_t Size) {
  if (Size == 0) return 0;

  aom_codec_ctx_t *ctx = init_encoder(Data, Size);
  if (!ctx) return 0;

  // Extract parameters for image creation
  int w = (Data[0] << 8) | Data[1];
  int h = (Data[2] << 8) | Data[3];
  int bit_depth = Data[4];
  if (bit_depth < 8 || bit_depth > 12) bit_depth = 8;

  aom_image_t *img = create_image(Data, Size, w, h, bit_depth);
  if (!img) {
    aom_codec_destroy(ctx);
    free(ctx);
    return 0;
  }

  // Fuzz aom_codec_set_option
  // Use data to generate key/value pairs safely
  if (Size > 20) {
    size_t key_len = Data[5] % (Size - 20);
    size_t val_len = Data[6] % (Size - 20 - key_len);
    if (key_len > 0 && val_len > 0 && key_len + val_len < Size - 20) {
      char key[256] = {0};
      char val[256] = {0};
      size_t k_len = key_len > 255 ? 255 : key_len;
      size_t v_len = val_len > 255 ? 255 : val_len;
      memcpy(key, Data + 20, k_len);
      memcpy(val, Data + 20 + key_len, v_len);
      aom_codec_set_option(ctx, key, val);
    }
  }

  // Fuzz aom_codec_encode
  // Generate PTS and duration from data
  aom_codec_pts_t pts = 0;
  unsigned long duration = 1;
  aom_enc_frame_flags_t flags = 0;

  if (Size > 12) {
    pts = (int64_t)(Data[7] | (Data[8] << 8) | (Data[9] << 16));
    duration = (unsigned long)(Data[10] | (Data[11] << 8));
    if (duration == 0) duration = 1;
    flags = (aom_enc_frame_flags_t)(Data[12] % 16);
  }

  // Encode one frame
  aom_codec_err_t res = aom_codec_encode(ctx, img, pts, duration, flags);
  if (res != AOM_CODEC_OK) {
    // If encoding fails, we still try to flush or just cleanup
  }

  // Flush the encoder
  while (1) {
    aom_codec_cx_pkt_t pkt;
    memset(&pkt, 0, sizeof(pkt));
    
    // Call encode with NULL to flush
    res = aom_codec_encode(ctx, NULL, 0, 0, 0);
    if (res != AOM_CODEC_OK) break;

    // Get data
    const aom_codec_cx_pkt_t *pkt_ptr = aom_codec_get_cx_data(ctx, &pkt);
    if (!pkt_ptr) break;

    // Process packet if needed, but for fuzzing we just iterate
    // If we get a frame, we can stop or continue. Let's stop after one flush cycle to avoid infinite loops
    break;
  }

  // Cleanup
  free_image(img);
  aom_codec_destroy(ctx);
  free(ctx);

  return 0;
}