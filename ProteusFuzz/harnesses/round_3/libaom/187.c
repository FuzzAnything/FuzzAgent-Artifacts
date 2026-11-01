// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

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

static void setup_encoder_config(aom_codec_enc_cfg_t *cfg) {
  cfg->g_w = 128;
  cfg->g_h = 128;
  cfg->g_timebase.num = 1;
  cfg->g_timebase.den = 30;
  cfg->g_threads = 1;
  cfg->g_error_resilient = 0;
  cfg->rc_target_bitrate = 1000;
  cfg->rc_min_quantizer = 0;
  cfg->rc_max_quantizer = 63;
  cfg->rc_end_usage = AOM_CBR;
  cfg->g_lag_in_frames = 0;
  cfg->g_pass = AOM_RC_ONE_PASS;
  cfg->g_profile = 0;
}

int LLVMFuzzerTestOneInput_187(const uint8_t *Data, size_t Size) {
  if (Size < 2) {
    return 0;
  }

  aom_codec_ctx_t ctx;
  aom_codec_enc_cfg_t cfg;
  aom_codec_iface_t *iface = aom_codec_av1_cx();
  if (!iface) {
    return 0;
  }

  setup_encoder_config(&cfg);

  if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
    return 0;
  }

  // Use input data to fuzz control parameters
  int val = (int)Data[0];
  int val2 = (int)Data[1];

  // Fuzz AV1E_SET_S_FRAME_MODE
  aom_codec_control(&ctx, AV1E_SET_S_FRAME_MODE, val);

  // Fuzz AV1E_SET_MAX_REFERENCE_FRAMES
  // Valid range is typically 1-7 for AV1, but we fuzz to test bounds
  aom_codec_control(&ctx, AV1E_SET_MAX_REFERENCE_FRAMES, val2);

  // Fuzz AV1E_SET_ENABLE_REF_FRAME_MVS
  aom_codec_control(&ctx, AV1E_SET_ENABLE_REF_FRAME_MVS, val & 1);

  // Fuzz AV1E_SET_ENABLE_INTERINTRA_WEDGE
  aom_codec_control(&ctx, AV1E_SET_ENABLE_INTERINTRA_WEDGE, val & 1);

  // Fuzz AV1E_SET_ENABLE_FILTER_INTRA
  aom_codec_control(&ctx, AV1E_SET_ENABLE_FILTER_INTRA, val & 1);

  // Fuzz AV1E_SET_RTC_EXTERNAL_RC
  aom_codec_control(&ctx, AV1E_SET_RTC_EXTERNAL_RC, val2 & 1);

  // Cleanup
  aom_codec_destroy(&ctx);

  return 0;
}