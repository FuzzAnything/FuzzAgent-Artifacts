// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
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

static void cleanup_codec(aom_codec_ctx_t *ctx) {
  if (ctx) {
    aom_codec_destroy(ctx);
  }
}

int LLVMFuzzerTestOneInput_294(const uint8_t *Data, size_t Size) {
  if (Size < 2) {
    return 0;
  }

  aom_codec_ctx_t ctx;
  aom_codec_iface_t *iface = aom_codec_av1_cx();
  if (!iface) {
    return 0;
  }

  aom_codec_err_t err = aom_codec_enc_init(&ctx, iface, NULL, 0);
  if (err != AOM_CODEC_OK) {
    return 0;
  }

  // Use input data to diversify control parameters
  int val1 = Data[0];
  int val2 = Data[1];
  int val3 = (Size > 2) ? Data[2] : 0;
  int val4 = (Size > 3) ? Data[3] : 0;
  int val5 = (Size > 4) ? Data[4] : 0;

  // Fuzz AV1E_SET_ENABLE_TX_SIZE_SEARCH
  aom_codec_control(&ctx, AV1E_SET_ENABLE_TX_SIZE_SEARCH, val1);

  // Fuzz AV1E_SET_DELTALF_MODE
  aom_codec_control(&ctx, AV1E_SET_DELTALF_MODE, val2);

  // Fuzz AV1E_SET_ENABLE_INTRABC
  aom_codec_control(&ctx, AV1E_SET_ENABLE_INTRABC, val3);

  // Fuzz AV1E_GET_ACTIVEMAP
  aom_active_map_t active_map;
  memset(&active_map, 0, sizeof(active_map));
  active_map.rows = (Size > 5) ? Data[5] : 0;
  active_map.cols = (Size > 6) ? Data[6] : 0;
  if (active_map.rows > 0 && active_map.cols > 0) {
    active_map.active_map = (unsigned char *)malloc(active_map.rows * active_map.cols);
    if (active_map.active_map) {
      if (Size > 7) {
        size_t copy_size = (Size - 7);
        if (copy_size > active_map.rows * active_map.cols) {
          copy_size = active_map.rows * active_map.cols;
        }
        memcpy(active_map.active_map, Data + 7, copy_size);
      }
      aom_codec_control(&ctx, AV1E_GET_ACTIVEMAP, &active_map);
      free(active_map.active_map);
    }
  }

  // Fuzz AV1E_SET_INTRA_DEFAULT_TX_ONLY
  aom_codec_control(&ctx, AV1E_SET_INTRA_DEFAULT_TX_ONLY, val4);

  // Fuzz AOME_SET_ACTIVEMAP
  aom_active_map_t set_active_map;
  memset(&set_active_map, 0, sizeof(set_active_map));
  set_active_map.rows = (Size > 8) ? Data[8] : 0;
  set_active_map.cols = (Size > 9) ? Data[9] : 0;
  if (set_active_map.rows > 0 && set_active_map.cols > 0) {
    set_active_map.active_map = (unsigned char *)malloc(set_active_map.rows * set_active_map.cols);
    if (set_active_map.active_map) {
      if (Size > 10) {
        size_t copy_size = (Size - 10);
        if (copy_size > set_active_map.rows * set_active_map.cols) {
          copy_size = set_active_map.rows * set_active_map.cols;
        }
        memcpy(set_active_map.active_map, Data + 10, copy_size);
      }
      aom_codec_control(&ctx, AOME_SET_ACTIVEMAP, &set_active_map);
      free(set_active_map.active_map);
    }
  }

  cleanup_codec(&ctx);
  return 0;
}