// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aom_image.h>
#include <aom/aom_integer.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_decoder.h>
#include <aom/aom.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
  if (ctx) {
    aom_codec_destroy(ctx);
  }
}

static int init_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg, const uint8_t *data, size_t size) {
  if (!ctx || !cfg) return 0;

  memset(ctx, 0, sizeof(*ctx));
  memset(cfg, 0, sizeof(*cfg));

  aom_codec_iface_t *iface = aom_codec_av1_cx();
  if (!iface) return 0;

  if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) {
    return 0;
  }

  // Use fuzz data to set some config parameters if available
  if (size > 0) cfg->g_w = data[0] ? data[0] : 128;
  if (size > 1) cfg->g_h = data[1] ? data[1] : 128;
  if (size > 2) cfg->g_timebase.num = data[2] ? data[2] : 1;
  if (size > 3) cfg->g_timebase.den = data[3] ? data[3] : 30;
  cfg->g_threads = 1;
  cfg->g_lag_in_frames = 0;

  if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) {
    return 0;
  }

  return 1;
}

int LLVMFuzzerTestOneInput_300(const uint8_t *Data, size_t Size) {
  if (Size == 0) return 0;

  aom_codec_ctx_t ctx;
  aom_codec_enc_cfg_t cfg;
  aom_roi_map_t roi_map;
  aom_active_map_t active_map;

  if (!init_encoder(&ctx, &cfg, Data, Size)) {
    return 0;
  }

  // Fuzz AV1E_SET_ENABLE_CFL_INTRA
  int enable_cfl = (Size > 0) ? Data[0] % 2 : 0;
  aom_codec_control(&ctx, AV1E_SET_ENABLE_CFL_INTRA, enable_cfl);

  // Fuzz AV1E_SET_ENABLE_GLOBAL_MOTION
  int enable_gm = (Size > 1) ? Data[1] % 2 : 0;
  aom_codec_control(&ctx, AV1E_SET_ENABLE_GLOBAL_MOTION, enable_gm);

  // Fuzz AV1E_SET_TRANSFER_CHARACTERISTICS
  int transfer_chars = (Size > 2) ? Data[2] % 10 : 0; // Assuming valid range 0-9
  aom_codec_control(&ctx, AV1E_SET_TRANSFER_CHARACTERISTICS, transfer_chars);

  // Fuzz AOME_SET_ROI_MAP
  if (Size > 3) {
    roi_map.enabled = Data[3] % 2;
    unsigned int rows = (Size > 4) ? Data[4] % 32 : 8;
    unsigned int cols = (Size > 5) ? Data[5] % 32 : 8;
    roi_map.rows = rows;
    roi_map.cols = cols;
    size_t map_size = rows * cols;
    roi_map.roi_map = malloc(map_size);
    if (roi_map.roi_map) {
      if (Size > 6) {
        size_t copy_size = (Size - 6) < map_size ? (Size - 6) : map_size;
        memcpy(roi_map.roi_map, Data + 6, copy_size);
        if (copy_size < map_size) {
          memset(roi_map.roi_map + copy_size, 0, map_size - copy_size);
        }
      } else {
        memset(roi_map.roi_map, 0, map_size);
      }
      // Initialize other fields to safe defaults
      memset(roi_map.delta_q, 0, sizeof(roi_map.delta_q));
      memset(roi_map.delta_lf, 0, sizeof(roi_map.delta_lf));
      memset(roi_map.skip, 0, sizeof(roi_map.skip));
      memset(roi_map.ref_frame, 0, sizeof(roi_map.ref_frame));
      roi_map.delta_qp_enabled = 0;
      roi_map.reference_enabled = 0;
      roi_map.rdmult_delta_qp = 1;
      aom_codec_control(&ctx, AOME_SET_ROI_MAP, &roi_map);
      free(roi_map.roi_map);
    }
  }

  // Fuzz AV1E_SET_ENABLE_INTRABC
  int enable_intrabc = (Size > 6) ? Data[6] % 2 : 0;
  aom_codec_control(&ctx, AV1E_SET_ENABLE_INTRABC, enable_intrabc);

  // Fuzz AOME_SET_ACTIVEMAP
  if (Size > 7) {
    unsigned int am_rows = (Size > 8) ? Data[8] % 32 : 8;
    unsigned int am_cols = (Size > 9) ? Data[9] % 32 : 8;
    active_map.rows = am_rows;
    active_map.cols = am_cols;
    size_t am_size = am_rows * am_cols;
    active_map.active_map = malloc(am_size);
    if (active_map.active_map) {
      if (Size > 10) {
        size_t copy_size = (Size - 10) < am_size ? (Size - 10) : am_size;
        memcpy(active_map.active_map, Data + 10, copy_size);
        if (copy_size < am_size) {
          memset(active_map.active_map + copy_size, 0, am_size - copy_size);
        }
      } else {
        memset(active_map.active_map, 0, am_size);
      }
      aom_codec_control(&ctx, AOME_SET_ACTIVEMAP, &active_map);
      free(active_map.active_map);
    }
  }

  cleanup_codec(&ctx);
  return 0;
}