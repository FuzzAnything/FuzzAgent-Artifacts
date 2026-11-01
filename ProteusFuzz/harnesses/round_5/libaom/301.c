// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_image.h>
#include <aom/aom_integer.h>

/*
 * Helper function to initialize an encoder context.
 * Returns 0 on success, -1 on failure.
 */
static int init_encoder_context(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg) {
  if (!ctx || !cfg) return -1;

  const aom_codec_iface_t *iface = aom_codec_av1_cx();
  if (!iface) return -1;

  if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) {
    return -1;
  }

  cfg->g_w = 64;
  cfg->g_h = 64;
  cfg->g_timebase.num = 1;
  cfg->g_timebase.den = 30;
  cfg->g_threads = 1;
  cfg->rc_min_quantizer = 0;
  cfg->rc_max_quantizer = 63;

  if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) {
    return -1;
  }

  return 0;
}

/*
 * Helper function to cleanup encoder context.
 */
static void cleanup_encoder_context(aom_codec_ctx_t *ctx) {
  if (ctx) {
    aom_codec_destroy(ctx);
  }
}

/*
 * Helper function to create a dummy active map from fuzz input.
 */
static int create_active_map_from_input(const uint8_t *data, size_t size, aom_active_map_t *am) {
  if (!data || !am) return -1;

  // Extract rows and cols from first 4 bytes if available
  unsigned int rows = 0;
  unsigned int cols = 0;

  if (size >= 4) {
    rows = data[0];
    cols = data[1];
  }

  // Limit dimensions to prevent excessive allocation
  if (rows > 32 || cols > 32) {
    rows = 32;
    cols = 32;
  }

  if (rows == 0 || cols == 0) {
    rows = 1;
    cols = 1;
  }

  size_t map_size = (size_t)rows * cols;
  am->active_map = (unsigned char *)malloc(map_size);
  if (!am->active_map) return -1;

  am->rows = rows;
  am->cols = cols;

  // Fill map with fuzz data
  size_t data_offset = 4;
  size_t data_len = (size > data_offset) ? (size - data_offset) : 0;
  if (data_len > map_size) data_len = map_size;

  memcpy(am->active_map, data + data_offset, data_len);
  // Zero out remaining if input is shorter
  if (data_len < map_size) {
    memset(am->active_map + data_len, 0, map_size - data_len);
  }

  return 0;
}

/*
 * Cleanup active map memory.
 */
static void cleanup_active_map(aom_active_map_t *am) {
  if (am && am->active_map) {
    free(am->active_map);
    am->active_map = NULL;
  }
}

int LLVMFuzzerTestOneInput_301(const uint8_t *Data, size_t Size) {
  if (Size == 0) return 0;

  aom_codec_ctx_t ctx = {0};
  aom_codec_enc_cfg_t cfg = {0};
  aom_active_map_t am = {0};
  int ret;

  // Initialize encoder
  if (init_encoder_context(&ctx, &cfg) != 0) {
    return 0;
  }

  // Create active map from input
  if (create_active_map_from_input(Data, Size, &am) != 0) {
    cleanup_encoder_context(&ctx);
    return 0;
  }

  // 1. aom_codec_control_typechecked_AV1E_SET_ENABLE_INTRABC
  // Control: AV1E_SET_ENABLE_INTRABC, Value: int (0 or 1)
  int enable_intrabc = (Size > 0) ? (Data[0] % 2) : 0;
  ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_INTRABC, &enable_intrabc);
  (void)ret; // Ignore return for fuzzing exploration

  // 2. aom_codec_control_typechecked_AV1E_SET_ENABLE_RESTORATION
  // Control: AV1E_SET_ENABLE_RESTORATION, Value: int (0 or 1)
  int enable_restoration = (Size > 1) ? (Data[1] % 2) : 0;
  ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_RESTORATION, &enable_restoration);
  (void)ret;

  // 3. aom_codec_control_typechecked_AV1E_SET_ENABLE_TX_SIZE_SEARCH
  // Control: AV1E_SET_ENABLE_TX_SIZE_SEARCH, Value: int (0 or 1)
  int enable_tx_size_search = (Size > 2) ? (Data[2] % 2) : 0;
  ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_TX_SIZE_SEARCH, &enable_tx_size_search);
  (void)ret;

  // 4. aom_codec_control_typechecked_AOME_SET_ACTIVEMAP
  // Control: AOME_SET_ACTIVEMAP, Value: aom_active_map_t *
  ret = aom_codec_control(&ctx, AOME_SET_ACTIVEMAP, &am);
  (void)ret;

  // 5. aom_codec_control_typechecked_AV1E_GET_ACTIVEMAP
  // Control: AV1E_GET_ACTIVEMAP, Value: aom_active_map_t *
  aom_active_map_t get_am = {0};
  ret = aom_codec_control(&ctx, AV1E_GET_ACTIVEMAP, &get_am);
  (void)ret;
  cleanup_active_map(&get_am);

  // 6. aom_codec_control_typechecked_AV1E_SET_ENABLE_INTERINTRA_COMP
  // Control: AV1E_SET_ENABLE_INTERINTRA_COMP, Value: int (0 or 1)
  int enable_interintra_comp = (Size > 3) ? (Data[3] % 2) : 0;
  ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_INTERINTRA_COMP, &enable_interintra_comp);
  (void)ret;

  // Cleanup
  cleanup_active_map(&am);
  cleanup_encoder_context(&ctx);

  return 0;
}