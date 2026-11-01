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

static int fuzz_encoder_control(aom_codec_ctx_t *ctx, int control_id, int value) {
  if (!ctx) return 0;
  return aom_codec_control(ctx, control_id, value);
}

int LLVMFuzzerTestOneInput_173(const uint8_t *Data, size_t Size) {
  if (Size < 6) return 0;

  aom_codec_ctx_t ctx;
  memset(&ctx, 0, sizeof(ctx));

  const aom_codec_iface_t *iface = aom_codec_av1_cx();
  if (!iface) return 0;

  aom_codec_enc_cfg_t cfg;
  if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) return 0;

  cfg.g_w = 128;
  cfg.g_h = 128;
  cfg.g_timebase.num = 1;
  cfg.g_timebase.den = 30;
  cfg.rc_target_bitrate = 1000;
  cfg.g_lag_in_frames = 0;

  if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) return 0;

  // Extract values from input safely
  int reduced_ref = (Data[0] % 2);
  int chroma_pos = (Data[1] % 3);
  int color_prim = (Data[2] % 10);
  int warped_motion = (Data[3] % 2);
  int transfer_char = (Data[4] % 10);
  int color_range = (Data[5] % 2);

  // Fuzz AV1E_SET_REDUCED_REFERENCE_SET
  fuzz_encoder_control(&ctx, AV1E_SET_REDUCED_REFERENCE_SET, reduced_ref);

  // Fuzz AV1E_SET_CHROMA_SAMPLE_POSITION
  fuzz_encoder_control(&ctx, AV1E_SET_CHROMA_SAMPLE_POSITION, chroma_pos);

  // Fuzz AV1E_SET_COLOR_PRIMARIES
  fuzz_encoder_control(&ctx, AV1E_SET_COLOR_PRIMARIES, color_prim);

  // Fuzz AV1E_SET_ALLOW_WARPED_MOTION
  fuzz_encoder_control(&ctx, AV1E_SET_ALLOW_WARPED_MOTION, warped_motion);

  // Fuzz AV1E_SET_TRANSFER_CHARACTERISTICS
  fuzz_encoder_control(&ctx, AV1E_SET_TRANSFER_CHARACTERISTICS, transfer_char);

  // Fuzz AV1E_SET_COLOR_RANGE
  fuzz_encoder_control(&ctx, AV1E_SET_COLOR_RANGE, color_range);

  // Cleanup
  aom_codec_destroy(&ctx);

  return 0;
}