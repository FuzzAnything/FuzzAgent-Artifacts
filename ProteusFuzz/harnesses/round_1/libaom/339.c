// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_get_global_headers at aom_encoder.c:281:18 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
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

static int create_test_image(const uint8_t *data, size_t size, aom_image_t *img) {
  unsigned int w, h;
  unsigned int bit_depth = 8;
  
  if (size < 8) return 0;
  
  w = *(const unsigned int *)data;
  h = *(const unsigned int *)(data + 4);
  
  if (w == 0 || h == 0) {
    w = 16;
    h = 16;
  }
  
  if (w > 4096) w = 4096;
  if (h > 4096) h = 4096;
  
  if (size > 8 && data[8] > 8) {
    bit_depth = data[8] > 12 ? 8 : data[8];
  }
  
  if (aom_img_alloc(img, AOM_IMG_FMT_I420, w, h, 1) != 0) {
    return 0;
  }
  
  size_t plane_size = img->stride[0] * h;
  size_t total_size = plane_size * 3;
  
  if (size > 8) {
    size_t data_size = size - 8;
    if (data_size > total_size) data_size = total_size;
    
    memcpy(img->planes[0], data + 8, img->stride[0] * h);
    if (data_size > plane_size) {
      memcpy(img->planes[1], data + 8 + plane_size, img->stride[1] * (h/2));
      if (data_size > plane_size * 2) {
        memcpy(img->planes[2], data + 8 + plane_size * 2, img->stride[2] * (h/2));
      }
    }
  }
  
  img->bit_depth = bit_depth;
  return 1;
}

static void cleanup_image(aom_image_t *img) {
  if (img && img->planes[0]) {
    aom_img_free(img);
  }
}

int LLVMFuzzerTestOneInput_339(const uint8_t *Data, size_t Size) {
  if (Size < 16) return 0;
  
  aom_codec_ctx_t ctx;
  aom_codec_iface_t *iface;
  aom_codec_enc_cfg_t cfg;
  aom_image_t img;
  aom_codec_err_t res;
  aom_codec_cx_pkt_t pkt;
  aom_fixed_buf_t *global_headers = NULL;
  
  memset(&ctx, 0, sizeof(ctx));
  memset(&img, 0, sizeof(img));
  
  iface = aom_codec_av1_cx();
  if (!iface) return 0;
  
  if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
    return 0;
  }
  
  cfg.g_w = (Size > 20) ? *(const uint16_t *)(Data + 16) : 16;
  cfg.g_h = (Size > 22) ? *(const uint16_t *)(Data + 18) : 16;
  
  if (cfg.g_w == 0) cfg.g_w = 16;
  if (cfg.g_h == 0) cfg.g_h = 16;
  if (cfg.g_w > 4096) cfg.g_w = 4096;
  if (cfg.g_h > 4096) cfg.g_h = 4096;
  
  cfg.g_timebase.num = (Size > 24) ? Data[24] : 1;
  cfg.g_timebase.den = (Size > 25) ? Data[25] : 1000;
  if (cfg.g_timebase.num == 0) cfg.g_timebase.num = 1;
  if (cfg.g_timebase.den == 0) cfg.g_timebase.den = 1000;
  
  cfg.rc_target_bitrate = (Size > 26) ? *(const uint32_t *)(Data + 26) : 1000;
  cfg.g_threads = (Size > 30) ? Data[30] : 1;
  if (cfg.g_threads == 0) cfg.g_threads = 1;
  if (cfg.g_threads > 16) cfg.g_threads = 16;
  
  if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
    return 0;
  }
  
  if (Size > 32) {
    int enable_flip = Data[32] % 2;
    aom_codec_control(&ctx, AV1E_SET_ENABLE_FLIP_IDTX, enable_flip);
  }
  
  if (Size > 33) {
    int tile_rows = Data[33] % 8;
    aom_codec_control(&ctx, AV1E_SET_TILE_ROWS, tile_rows);
  }
  
  if (Size > 34) {
    int frame_parallel = Data[34] % 2;
    aom_codec_control(&ctx, AV1E_SET_FRAME_PARALLEL_DECODING, frame_parallel);
  }
  
  if (Size > 35) {
    int quantizer_one_pass = Data[35] % 2;
    aom_codec_control(&ctx, AV1E_SET_QUANTIZER_ONE_PASS, quantizer_one_pass);
  }
  
  if (create_test_image(Data, Size, &img)) {
    aom_codec_pts_t pts = 0;
    unsigned long duration = 1;
    aom_enc_frame_flags_t flags = 0;
    
    if (Size > 36) {
      pts = *(const int64_t *)(Data + 36);
    }
    if (Size > 44) {
      duration = *(const unsigned long *)(Data + 44);
      if (duration == 0) duration = 1;
      if (duration > UINT32_MAX) duration = UINT32_MAX;
    }
    if (Size > 48) {
      flags = *(const aom_enc_frame_flags_t *)(Data + 48);
    }
    
    res = aom_codec_encode(&ctx, &img, pts, duration, flags);
    
    cleanup_image(&img);
    
    while (res == AOM_CODEC_OK) {
      if (aom_codec_get_cx_data(&ctx, &pkt) == AOM_CODEC_OK) {
        if (pkt.kind == AOM_CODEC_CX_FRAME_PKT) {
          if (pkt.data.frame.buf) {
            free(pkt.data.frame.buf);
          }
        }
      } else {
        break;
      }
    }
  }
  
  res = aom_codec_encode(&ctx, NULL, 0, 0, 0);
  while (res == AOM_CODEC_OK) {
    if (aom_codec_get_cx_data(&ctx, &pkt) == AOM_CODEC_OK) {
      if (pkt.kind == AOM_CODEC_CX_FRAME_PKT) {
        if (pkt.data.frame.buf) {
          free(pkt.data.frame.buf);
        }
      }
    } else {
      break;
    }
  }
  
  global_headers = aom_codec_get_global_headers(&ctx);
  if (global_headers) {
    if (global_headers->buf) {
      free(global_headers->buf);
    }
    free(global_headers);
  }
  
  aom_codec_destroy(&ctx);
  
  return 0;
}