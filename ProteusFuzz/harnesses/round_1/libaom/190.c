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
#include <stdio.h>
#include <limits.h>

#include "aom/aom_decoder.h"
#include "aom/aomdx.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_integer.h"
#include "aom/aom_image.h"
#include "aom/aom_encoder.h"
#include "aom/aomcx.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_frame_buffer.h"

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, int control_id, int value) {
  if (!ctx) return 0;
  return aom_codec_control(ctx, control_id, &value);
}

int LLVMFuzzerTestOneInput_190(const uint8_t *Data, size_t Size) {
  if (Size < 6) return 0;

  aom_codec_ctx_t ctx;
  memset(&ctx, 0, sizeof(ctx));

  const aom_codec_iface_t *iface = aom_codec_av1_cx();
  if (!iface) return 0;

  aom_codec_enc_cfg_t cfg;
  if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
    return 0;
  }

  cfg.g_w = (Data[0] % 4096) + 16;
  cfg.g_h = (Data[1] % 4096) + 16;
  cfg.g_timebase.num = (Data[2] % 255) + 1;
  cfg.g_timebase.den = (Data[3] % 255) + 1;
  cfg.g_threads = (Data[4] % 16) + 1;
  cfg.rc_target_bitrate = (Data[5] % 10000) + 100;
  cfg.g_pass = AOM_RC_ONE_PASS;

  if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
    return 0;
  }

  int control_ids[] = {
    AV1E_SET_ENABLE_OVERLAY,
    AV1E_SET_ENABLE_INTRABC,
    AV1E_SET_ENABLE_ANGLE_DELTA,
    AV1E_SET_ENABLE_DUAL_FILTER,
    AOME_SET_TUNING,
    AV1E_SET_ENABLE_1TO4_PARTITIONS
  };
  int num_controls = sizeof(control_ids) / sizeof(control_ids[0]);

  for (size_t i = 0; i < Size; i++) {
    int control_idx = Data[i] % num_controls;
    int control_id = control_ids[control_idx];
    
    int value;
    if (control_id == AOME_SET_TUNING) {
      value = Data[(i + 1) % Size] % 10; 
    } else {
      value = Data[(i + 1) % Size] % 2;
    }

    fuzz_encoder_control(&ctx, control_id, value);
  }

  aom_codec_destroy(&ctx);

  return 0;
}