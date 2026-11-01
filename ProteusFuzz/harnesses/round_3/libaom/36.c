// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>

// Helper to safely initialize encoder context
static int init_encoder(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg) {
  aom_codec_iface_t *iface = aom_codec_av1_cx();
  if (!iface) return 0;

  if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) {
    return 0;
  }

  // Set some basic config to ensure valid initialization
  cfg->g_w = 64;
  cfg->g_h = 64;
  cfg->g_timebase.num = 1;
  cfg->g_timebase.den = 30;
  cfg->g_threads = 1;
  cfg->g_error_resilient = 0;
  cfg->rc_target_bitrate = 1000;
  cfg->rc_min_quantizer = 0;
  cfg->rc_max_quantizer = 63;

  if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) {
    return 0;
  }
  return 1;
}

int LLVMFuzzerTestOneInput_36(const uint8_t *Data, size_t Size) {
  if (Size < 3) return 0;

  aom_codec_ctx_t ctx;
  aom_codec_enc_cfg_t cfg;
  memset(&ctx, 0, sizeof(ctx));
  memset(&cfg, 0, sizeof(cfg));

  if (!init_encoder(&ctx, &cfg)) {
    return 0;
  }

  // Use input data to derive control command index and parameters
  int cmd_index = Data[0] % 6; // 0 to 5 for the 6 target functions
  int param_val = (Data[1] % 2) ? 1 : 0; // 0 or 1 for boolean flags
  int param_int = (Data[2] % 10) + 4; // 4 to 13 for partition sizes

  // Define commands based on libaom AV1 encoder control constants
  int control_cmd = 0;
  void *control_arg = NULL;

  switch (cmd_index) {
    case 0: // AV1E_SET_ENABLE_1TO4_PARTITIONS
      control_cmd = AV1E_SET_ENABLE_1TO4_PARTITIONS;
      control_arg = (void *)(intptr_t)param_val;
      break;
    case 1: // AV1E_SET_EXTERNAL_PARTITION
      control_cmd = AV1E_SET_EXTERNAL_PARTITION;
      control_arg = NULL; 
      break;
    case 2: // AV1E_SET_ENABLE_AB_PARTITIONS
      control_cmd = AV1E_SET_ENABLE_AB_PARTITIONS;
      control_arg = (void *)(intptr_t)param_val;
      break;
    case 3: // AV1E_SET_MIN_PARTITION_SIZE
      control_cmd = AV1E_SET_MIN_PARTITION_SIZE;
      control_arg = (void *)(intptr_t)param_int;
      break;
    case 4: // AV1E_SET_RTC_EXTERNAL_RC
      control_cmd = AV1E_SET_RTC_EXTERNAL_RC;
      control_arg = (void *)(intptr_t)param_val;
      break;
    case 5: // AV1E_SET_MAX_PARTITION_SIZE
      control_cmd = AV1E_SET_MAX_PARTITION_SIZE;
      control_arg = (void *)(intptr_t)param_int;
      break;
    default:
      break;
  }

  // Invoke the target function via the generic control interface
  aom_codec_err_t res = aom_codec_control(&ctx, control_cmd, control_arg);

  // Cleanup
  aom_codec_destroy(&ctx);

  return 0;
}