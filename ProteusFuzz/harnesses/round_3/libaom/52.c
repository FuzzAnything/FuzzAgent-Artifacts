// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control_typechecked_AV1E_SET_CDF_UPDATE_MODE at aomcx.h:1975:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_MODE_COST_UPD_FREQ at aomcx.h:2222:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_AQ_MODE at aomcx.h:1963:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_ROW_MT at aomcx.h:1939:1 in aomcx.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control_typechecked_AV1E_SET_ENABLE_TX_SIZE_SEARCH at aomcx.h:2285:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_ENABLE_GLOBAL_MOTION at aomcx.h:2126:1 in aomcx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aom/aom_codec.h"
#include "aom/aom_encoder.h"
#include "aom/aomdx.h"
#include "aom/aom_integer.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_image.h"
#include "aom/aom_decoder.h"
#include "aom/aom.h"
#include "aom/aomcx.h"
#include "aom/aom_external_partition.h"

/*
 * Fuzz driver for libaom AV1 encoder control functions.
 * Target functions:
 * - aom_codec_control_typechecked_AV1E_SET_ENABLE_TX_SIZE_SEARCH
 * - aom_codec_control_typechecked_AV1E_SET_ENABLE_GLOBAL_MOTION
 * - aom_codec_control_typechecked_AV1E_SET_CDF_UPDATE_MODE
 * - aom_codec_control_typechecked_AV1E_SET_MODE_COST_UPD_FREQ
 * - aom_codec_control_typechecked_AV1E_SET_AQ_MODE
 * - aom_codec_control_typechecked_AV1E_SET_ROW_MT
 */

static int init_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg,
                        const uint8_t *data, size_t size) {
  aom_codec_iface_t *iface;
  aom_codec_err_t ret;
  int width = 64;
  int height = 64;

  if (size >= 4) {
    width = (data[0] % 128) + 16;
    height = (data[1] % 128) + 16;
    if (width < 16 || height < 16) {
      width = 64;
      height = 64;
    }
    if (width > 2048) width = 2048;
    if (height > 2048) height = 2048;
  }

  iface = aom_codec_av1_cx();
  if (!iface) return -1;

  ret = aom_codec_enc_config_default(iface, cfg, 0);
  if (ret != AOM_CODEC_OK) return -1;

  cfg->g_w = width;
  cfg->g_h = height;
  cfg->g_timebase.num = 1;
  cfg->g_timebase.den = 30;
  cfg->g_threads = 1;
  cfg->g_lag_in_frames = 0;
  cfg->rc_target_bitrate = 1000;

  ret = aom_codec_enc_init(ctx, iface, cfg, 0);
  if (ret != AOM_CODEC_OK) {
    aom_codec_destroy(ctx);
    return -1;
  }

  return 0;
}

static void cleanup_encoder(aom_codec_ctx_t *ctx) {
  aom_codec_destroy(ctx);
}

int LLVMFuzzerTestOneInput_52(const uint8_t *Data, size_t Size) {
  aom_codec_ctx_t ctx;
  aom_codec_enc_cfg_t cfg;
  aom_codec_err_t ret;
  int val;

  if (Size < 1) return 0;

  memset(&ctx, 0, sizeof(ctx));
  memset(&cfg, 0, sizeof(cfg));

  if (init_encoder(&ctx, &cfg, Data, Size) != 0) {
    return 0;
  }

  // Extract values from input to control the fuzzing parameters
  val = (Size > 0) ? (Data[0] % 2) : 0;

  // 1. AV1E_SET_ENABLE_TX_SIZE_SEARCH
  ret = aom_codec_control_typechecked_AV1E_SET_ENABLE_TX_SIZE_SEARCH(&ctx, AV1E_SET_ENABLE_TX_SIZE_SEARCH, val);
  (void)ret;

  val = (Size > 1) ? (Data[1] % 2) : 0;
  // 2. AV1E_SET_ENABLE_GLOBAL_MOTION
  ret = aom_codec_control_typechecked_AV1E_SET_ENABLE_GLOBAL_MOTION(&ctx, AV1E_SET_ENABLE_GLOBAL_MOTION, val);
  (void)ret;

  val = (Size > 2) ? (Data[2] % 3) : 0; // Assuming 3 modes for CDF update
  // 3. AV1E_SET_CDF_UPDATE_MODE
  ret = aom_codec_control_typechecked_AV1E_SET_CDF_UPDATE_MODE(&ctx, AV1E_SET_CDF_UPDATE_MODE, val);
  (void)ret;

  val = (Size > 3) ? (Data[3] % 16) : 0; // Assuming frequency range 0-15
  // 4. AV1E_SET_MODE_COST_UPD_FREQ
  ret = aom_codec_control_typechecked_AV1E_SET_MODE_COST_UPD_FREQ(&ctx, AV1E_SET_MODE_COST_UPD_FREQ, val);
  (void)ret;

  val = (Size > 4) ? (Data[4] % 5) : 0; // Assuming 5 AQ modes
  // 5. AV1E_SET_AQ_MODE
  ret = aom_codec_control_typechecked_AV1E_SET_AQ_MODE(&ctx, AV1E_SET_AQ_MODE, val);
  (void)ret;

  val = (Size > 5) ? (Data[5] % 2) : 0;
  // 6. AV1E_SET_ROW_MT
  ret = aom_codec_control_typechecked_AV1E_SET_ROW_MT(&ctx, AV1E_SET_ROW_MT, val);
  (void)ret;

  // Attempt to encode a frame to exercise the state changes
  if (Size > 6) {
    aom_image_t img;
    aom_codec_cx_pkt_t *pkt;
    aom_codec_flags_t flags = 0;
    int64_t ts = (Size > 7) ? Data[7] : 0;

    aom_img_alloc(&img, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 1);
    // Fill with dummy data
    if (img.planes[0]) {
        memset(img.planes[0], Data[6], img.stride[0] * img.h);
    }
    if (img.planes[1]) {
        memset(img.planes[1], Data[6] + 1, img.stride[1] * (img.h / 2));
    }
    if (img.planes[2]) {
        memset(img.planes[2], Data[6] + 2, img.stride[2] * (img.h / 2));
    }

    ret = aom_codec_encode(&ctx, &img, ts, 1, flags);
    (void)ret;

    // Drain the encoder
    while (ret == AOM_CODEC_OK) {
      pkt = aom_codec_get_cx_data(&ctx, &pkt);
      if (pkt) {
        // Process packet if needed, but for fuzzing just consume it
        if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
          // Optionally free data if allocated, but libaom usually manages it
        }
      } else {
        break;
      }
    }

    aom_img_free(&img);
  }

  cleanup_encoder(&ctx);
  return 0;
}