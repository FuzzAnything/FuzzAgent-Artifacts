// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aom_decoder.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>
#include <aom/aomcx.h>
#include <aom/aom_image.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom.h>

static int fuzz_encode_control_int(aom_codec_ctx_t *ctx, int control_id, int value) {
  if (ctx == NULL) return 0;
  aom_codec_err_t err = aom_codec_control(ctx, control_id, value);
  (void)err;
  return 1;
}

int LLVMFuzzerTestOneInput_148(const uint8_t *Data, size_t Size) {
  if (Size < 2) return 0;

  // Initialize encoder configuration
  aom_codec_enc_cfg_t cfg;
  if (aom_codec_enc_config_default(NULL, &cfg, 0) != AOM_CODEC_OK) {
    return 0;
  }

  // Use input data to configure basic settings
  cfg.g_w = (Data[0] % 128) + 16; // Width
  cfg.g_h = (Data[1] % 128) + 16; // Height
  cfg.g_timebase.num = 1;
  cfg.g_timebase.den = 30;
  cfg.g_threads = 1;
  cfg.g_lag_in_frames = 0;
  cfg.rc_target_bitrate = 1000;
  cfg.g_profile = 0;

  // Initialize codec context
  aom_codec_ctx_t ctx;
  memset(&ctx, 0, sizeof(ctx));
  
  aom_codec_iface_t *iface = aom_codec_av1_cx();
  if (iface == NULL) return 0;

  aom_codec_err_t err = aom_codec_enc_init(&ctx, iface, &cfg, 0);
  if (err != AOM_CODEC_OK) {
    return 0;
  }

  // Extract fuzzing parameters
  int val_int = 0;
  if (Size >= 4) {
    val_int = *(int*)(Data + 2);
  }

  // Fuzz AV1E_SET_CHROMA_SAMPLE_POSITION
  // Expected: int (0-2)
  int chroma_pos = val_int % 3;
  if (chroma_pos < 0) chroma_pos = 0;
  fuzz_encode_control_int(&ctx, AV1E_SET_CHROMA_SAMPLE_POSITION, chroma_pos);

  // Fuzz AV1E_SET_REDUCED_REFERENCE_SET
  // Expected: int (0-1)
  int reduced_ref = (val_int & 1);
  fuzz_encode_control_int(&ctx, AV1E_SET_REDUCED_REFERENCE_SET, reduced_ref);

  // Fuzz AV1E_SET_ENABLE_RECT_TX
  // Expected: int (0-1)
  int enable_rect_tx = (val_int & 1);
  fuzz_encode_control_int(&ctx, AV1E_SET_ENABLE_RECT_TX, enable_rect_tx);

  // Fuzz AV1E_SET_ENABLE_RECT_PARTITIONS
  // Expected: int (0-1)
  int enable_rect_part = (val_int & 1);
  fuzz_encode_control_int(&ctx, AV1E_SET_ENABLE_RECT_PARTITIONS, enable_rect_part);

  // Fuzz AV1E_SET_QUANTIZER_ONE_PASS
  // Expected: int (0-1)
  int quant_one_pass = (val_int & 1);
  fuzz_encode_control_int(&ctx, AV1E_SET_QUANTIZER_ONE_PASS, quant_one_pass);

  // Fuzz AV1E_SET_TILE_ROWS
  // Expected: int (0-31)
  int tile_rows = val_int % 32;
  if (tile_rows < 0) tile_rows = 0;
  fuzz_encode_control_int(&ctx, AV1E_SET_TILE_ROWS, tile_rows);

  // Cleanup
  aom_codec_destroy(&ctx);

  return 0;
}