// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

#include "aomcx.h"
#include "aomdx.h"
#include "aom.h"
#include "aom_codec.h"
#include "aom_external_partition.h"
#include "aom_decoder.h"
#include "aom_image.h"
#include "aom_frame_buffer.h"
#include "aom_encoder.h"
#include "aom_integer.h"

static int invoke_control(aom_codec_ctx_t *ctx, int control_id, int value) {
  if (ctx == NULL) return 0;
  return aom_codec_control(ctx, control_id, value);
}

int LLVMFuzzerTestOneInput_77(const uint8_t *Data, size_t Size) {
  if (Size < 6) return 0;

  aom_codec_ctx_t ctx;
  memset(&ctx, 0, sizeof(ctx));

  const aom_codec_iface_t *iface = aom_codec_av1_cx();
  if (iface == NULL) return 0;

  aom_codec_enc_cfg_t cfg;
  if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) return 0;

  // Use input data to configure encoder settings
  cfg.g_w = (Data[0] % 256) + 16;
  cfg.g_h = (Data[1] % 256) + 16;
  cfg.g_timebase.num = (Data[2] % 60) + 1;
  cfg.g_timebase.den = 1000;
  cfg.rc_target_bitrate = (Data[3] % 10000) + 100;

  if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) return 0;

  // Extract values from input for fuzzing control parameters
  int enable_intrabc = (Data[4 % Size] % 2);
  int target_seq_level_idx = (Data[(4 + 1) % Size] % 7); // AV1 levels 0-6
  int noise_sensitivity = (Data[(4 + 2) % Size] % 64);
  int enable_tpl_model = (Data[(4 + 3) % Size] % 2);
  int gf_cbr_boost_pct = (Data[(4 + 4) % Size] % 200);
  int qm_max = (Data[(4 + 5) % Size] % 255);

  // Invoke target control functions with corrected identifiers
  invoke_control(&ctx, AV1E_SET_ENABLE_INTRABC, enable_intrabc);
  invoke_control(&ctx, AV1E_SET_TARGET_SEQ_LEVEL_IDX, target_seq_level_idx);
  invoke_control(&ctx, AV1E_SET_NOISE_SENSITIVITY, noise_sensitivity);
  invoke_control(&ctx, AV1E_SET_ENABLE_TPL_MODEL, enable_tpl_model);
  invoke_control(&ctx, AV1E_SET_GF_CBR_BOOST_PCT, gf_cbr_boost_pct);
  invoke_control(&ctx, AV1E_SET_QM_MAX, qm_max);

  // Cleanup
  aom_codec_destroy(&ctx);

  return 0;
}