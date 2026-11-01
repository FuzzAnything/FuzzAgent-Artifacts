// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <aom/aom_decoder.h>
#include <aom/aomdx.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_encoder.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>

static int setup_encoder(aom_codec_ctx_t *codec, aom_codec_enc_cfg_t *cfg, const uint8_t *data, size_t size) {
  if (!codec || !cfg) return -1;

  // Initialize default config
  if (aom_codec_enc_config_default(aom_codec_av1_cx(), cfg, 0) != AOM_CODEC_OK) {
    return -1;
  }

  // Fuzz some configuration parameters based on input data
  if (size > 0) cfg->g_w = (data[0] % 1024) + 16;
  if (size > 1) cfg->g_h = (data[1] % 1024) + 16;
  if (size > 2) cfg->g_timebase.num = (data[2] % 100) + 1;
  if (size > 3) cfg->g_timebase.den = (data[3] % 100) + 1;
  if (size > 4) cfg->g_threads = (data[4] % 8) + 1;
  if (size > 5) cfg->g_error_resilient = data[5] % 2;
  if (size > 6) cfg->rc_target_bitrate = (data[6] % 10000) + 100;

  // Initialize codec
  if (aom_codec_enc_init(codec, aom_codec_av1_cx(), cfg, 0) != AOM_CODEC_OK) {
    return -1;
  }

  return 0;
}

static void cleanup_encoder(aom_codec_ctx_t *codec) {
  if (codec) {
    aom_codec_destroy(codec);
  }
}

int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
  if (Size < 2) return 0;

  aom_codec_ctx_t codec;
  aom_codec_enc_cfg_t cfg;
  memset(&codec, 0, sizeof(codec));
  memset(&cfg, 0, sizeof(cfg));

  if (setup_encoder(&codec, &cfg, Data, Size) != 0) {
    return 0;
  }

  // Fuzz AV1E_SET_ENABLE_INTERINTRA_WEDGE
  int enable_interintra_wedge = (Size > 2) ? (Data[2] % 2) : 0;
  aom_codec_control(&codec, AV1E_SET_ENABLE_INTERINTRA_WEDGE, enable_interintra_wedge);

  // Fuzz AV1E_SET_ENABLE_FILTER_INTRA
  int enable_filter_intra = (Size > 3) ? (Data[3] % 2) : 0;
  aom_codec_control(&codec, AV1E_SET_ENABLE_FILTER_INTRA, enable_filter_intra);

  // Fuzz AV1E_SET_TRANSFER_CHARACTERISTICS
  int transfer_chars = (Size > 4) ? (Data[4] % 10) : 0;
  aom_codec_control(&codec, AV1E_SET_TRANSFER_CHARACTERISTICS, transfer_chars);

  // Fuzz AV1E_GET_ACTIVEMAP
  aom_active_map_t active_map;
  memset(&active_map, 0, sizeof(active_map));
  if (Size > 5) {
    active_map.rows = (Data[5] % 16) + 1;
    active_map.cols = (Data[6] % 16) + 1;
    active_map.active_map = (unsigned char *)malloc(active_map.rows * active_map.cols);
    if (active_map.active_map) {
      memset(active_map.active_map, 0, active_map.rows * active_map.cols);
      aom_codec_control(&codec, AV1E_GET_ACTIVEMAP, &active_map);
      free(active_map.active_map);
    }
  }

  // Fuzz AV1E_SET_ENABLE_INTERINTRA_COMP
  int enable_interintra_comp = (Size > 7) ? (Data[7] % 2) : 0;
  aom_codec_control(&codec, AV1E_SET_ENABLE_INTERINTRA_COMP, enable_interintra_comp);

  // Fuzz AOME_SET_ACTIVEMAP
  if (Size > 8) {
    aom_active_map_t set_active_map;
    memset(&set_active_map, 0, sizeof(set_active_map));
    set_active_map.rows = (Data[8] % 16) + 1;
    set_active_map.cols = (Data[9] % 16) + 1;
    set_active_map.active_map = (unsigned char *)malloc(set_active_map.rows * set_active_map.cols);
    if (set_active_map.active_map) {
      memset(set_active_map.active_map, 0, set_active_map.rows * set_active_map.cols);
      aom_codec_control(&codec, AOME_SET_ACTIVEMAP, &set_active_map);
      free(set_active_map.active_map);
    }
  }

  cleanup_encoder(&codec);
  return 0;
}