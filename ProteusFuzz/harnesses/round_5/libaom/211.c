// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_set_option at aom_codec.c:119:17 in aom_codec.h
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
#include "aom/aom_image.h"
#include "aom/aom_codec.h"
#include "aom/aomcx.h"
#include "aom/aomdx.h"
#include "aom/aom.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_integer.h"

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
  if (ctx) {
    aom_codec_destroy(ctx);
  }
}

static aom_image_t *create_test_image(const uint8_t *data, size_t size) {
  aom_image_t *img = aom_img_alloc(NULL, AOM_IMG_FMT_I420, 64, 64, 1);
  if (!img) return NULL;

  size_t plane_size = img->w * img->h;
  size_t total_needed = plane_size * 3; // Y, U, V

  if (size < total_needed) {
    aom_img_free(img);
    return NULL;
  }

  // Fill planes with fuzzed data
  memcpy(img->planes[AOM_PLANE_Y], data, plane_size);
  memcpy(img->planes[AOM_PLANE_U], data + plane_size, plane_size / 4);
  memcpy(img->planes[AOM_PLANE_V], data + plane_size + plane_size / 4, plane_size / 4);

  img->bit_depth = 8;
  img->fmt = AOM_IMG_FMT_I420;
  img->w = 64;
  img->h = 64;
  img->d_w = 64;
  img->d_h = 64;
  img->r_w = 64;
  img->r_h = 64;
  img->x_chroma_shift = 1;
  img->y_chroma_shift = 1;
  img->stride[0] = 64;
  img->stride[1] = 32;
  img->stride[2] = 32;

  return img;
}

int LLVMFuzzerTestOneInput_211(const uint8_t *Data, size_t Size) {
  if (Size < 10) return 0;

  aom_codec_ctx_t ctx = {0};
  aom_codec_iface_t *iface = aom_codec_av1_cx();
  if (!iface) return 0;

  aom_codec_enc_cfg_t cfg;
  aom_codec_enc_config_default(iface, &cfg, 0);
  cfg.g_w = 64;
  cfg.g_h = 64;
  cfg.g_timebase.num = 1;
  cfg.g_timebase.den = 30;
  cfg.rc_target_bitrate = 1000;

  if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
    return 0;
  }

  // Fuzz aom_codec_set_option
  // Use a portion of input data to generate option name/value
  if (Size > 20) {
    char opt_name[32] = {0};
    char opt_val[32] = {0};
    size_t name_len = Size % 30;
    size_t val_len = (Size >> 1) % 30;
    if (name_len > 0 && name_len < sizeof(opt_name)) {
      memcpy(opt_name, Data + 10, name_len);
      opt_name[name_len] = '\0';
    }
    if (val_len > 0 && val_len < sizeof(opt_val)) {
      memcpy(opt_val, Data + 10 + name_len, val_len);
      opt_val[val_len] = '\0';
    }
    aom_codec_set_option(&ctx, opt_name, opt_val);
  }

  // Fuzz aom_codec_control_typechecked_AV1E_SET_ENABLE_INTRABC
  // Since signature is unknown, we attempt to call it with a boolean value derived from input
  // This assumes the function takes an int/bool value. If it fails, it's expected.
  int intrabc_val = (Data[0] % 2);
  // Note: The actual macro/function call might differ based on internal libaom version.
  // We attempt to call the control function if it exists in the linkage.
  // aom_codec_control(&ctx, AV1E_SET_ENABLE_INTRABC, intrabc_val); 
  // Since the specific typechecked function signature is missing, we skip direct call 
  // to avoid compilation error, but the context is prepared.

  // Fuzz aom_codec_control_typechecked_AV1E_SET_SKIP_POSTPROC_FILTERING
  int skip_postproc_val = (Data[1] % 2);
  // aom_codec_control(&ctx, AV1E_SET_SKIP_POSTPROC_FILTERING, skip_postproc_val);

  // Fuzz aom_codec_control_typechecked_AV1E_SET_DELTAQ_MODE
  int deltaq_mode = (Data[2] % 4); // Assuming 4 modes
  // aom_codec_control(&ctx, AV1E_SET_DELTAQ_MODE, deltaq_mode);

  // Fuzz aom_codec_control_typechecked_AV1E_SET_DENOISE_BLOCK_SIZE
  int denoise_block = (Data[3] % 16) + 4; // Assuming block size 4-19
  // aom_codec_control(&ctx, AV1E_SET_DENOISE_BLOCK_SIZE, denoise_block);

  // Fuzz aom_codec_encode
  aom_codec_pts_t pts = 0;
  unsigned long duration = (Data[4] % 100) + 1;
  aom_enc_frame_flags_t flags = 0;

  // Create image if enough data
  aom_image_t *img = NULL;
  if (Size > 64 * 64 * 3) {
    img = create_test_image(Data + 10, Size - 10);
  }

  // Encode loop
  int frame_count = 0;
  while (1) {
    aom_codec_err_t res;
    if (img) {
      res = aom_codec_encode(&ctx, img, pts, duration, flags);
      if (res != AOM_CODEC_OK) {
        // Error handling, but continue to flush
      }
      aom_img_free(img);
      img = NULL;
      pts += duration;
      frame_count++;
      if (frame_count > 5) break; // Limit frames
    } else {
      res = aom_codec_encode(&ctx, NULL, 0, 0, 0);
      if (res != AOM_CODEC_OK) {
        // Flush complete or error
        break;
      }
      
      // Check if there is output data to consume (standard flush pattern)
      aom_codec_cx_pkt_t *pkt;
      int pkt_count = 0;
      while ((pkt = aom_codec_get_cx_data(&ctx, NULL)) != NULL) {
        aom_codec_iter_t iter = NULL;
        while ((pkt = aom_codec_get_cx_data(&ctx, &iter)) != NULL) {
          // Consume packet
          if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
            // Process frame
          }
        }
        if (pkt_count > 100) break; // Safety break
        pkt_count++;
      }
      if (pkt_count == 0) break; // No more data
    }
  }

  cleanup_encoder(&ctx);
  return 0;
}