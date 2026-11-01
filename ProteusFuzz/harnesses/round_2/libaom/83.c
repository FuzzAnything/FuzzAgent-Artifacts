// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_external_partition.h>
#include <aom/aomcx.h>
#include <aom/aom_image.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom.h>

static void cleanup_codec(aom_codec_ctx_t *ctx) {
  if (ctx) {
    aom_codec_destroy(ctx);
  }
}

int LLVMFuzzerTestOneInput_83(const uint8_t *Data, size_t Size) {
  if (Size < 4) return 0;

  aom_codec_ctx_t ctx;
  memset(&ctx, 0, sizeof(ctx));

  const aom_codec_iface_t *iface = aom_codec_av1_cx();
  if (!iface) return 0;

  aom_codec_enc_cfg_t cfg;
  aom_codec_enc_config_default(iface, &cfg, 0);

  // Use fuzzed data to set some configuration parameters safely
  uint32_t val = 0;
  memcpy(&val, Data, sizeof(val));
  cfg.g_w = (val % 4096) + 16;
  cfg.g_h = ((val >> 16) % 4096) + 16;
  cfg.g_timebase.num = (val % 100) + 1;
  cfg.g_timebase.den = ((val >> 8) % 100) + 1;
  cfg.rc_target_bitrate = (val % 1000000) + 100;

  // Initialize encoder
  if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
    return 0;
  }

  // Prepare a dummy frame
  aom_image_t frame;
  aom_img_alloc(&frame, AOM_IMG_FMT_I420, cfg.g_w, cfg.g_h, 16);
  
  // Correctly clear planes based on their actual dimensions
  // Y plane
  memset(frame.planes[0], 0, frame.stride[0] * frame.h);
  // U and V planes: use frame.h >> frame.y_chroma_shift for height
  // For I420, y_chroma_shift is 1, so height is halved.
  int chroma_h = frame.h >> frame.y_chroma_shift;
  memset(frame.planes[1], 0, frame.stride[1] * chroma_h);
  memset(frame.planes[2], 0, frame.stride[2] * chroma_h);

  // Fuzz control parameters
  int param_val = 0;
  if (Size >= 8) {
    memcpy(&param_val, Data + 4, sizeof(param_val));
  }

  // Try various control calls with fuzzed values
  // AV1E_SET_DISABLE_TRELLIS_QUANT
  aom_codec_control(&ctx, AV1E_SET_DISABLE_TRELLIS_QUANT, param_val % 2);
  
  // AV1E_SET_QM_MIN
  aom_codec_control(&ctx, AV1E_SET_QM_MIN, param_val % 64);
  
  // AV1E_SET_QM_U
  aom_codec_control(&ctx, AV1E_SET_QM_U, param_val % 64);
  
  // AV1E_SET_AQ_MODE
  aom_codec_control(&ctx, AV1E_SET_AQ_MODE, param_val % 5);
  
  // AV1E_SET_MODE_COST_UPD_FREQ
  aom_codec_control(&ctx, AV1E_SET_MODE_COST_UPD_FREQ, param_val % 100);
  
  // AV1E_SET_QM_Y
  aom_codec_control(&ctx, AV1E_SET_QM_Y, param_val % 64);

  // Encode a frame to exercise the state
  aom_codec_frame_flags_t flags = 0;
  if (Size >= 12) {
    uint32_t temp_flags;
    memcpy(&temp_flags, Data + 8, sizeof(temp_flags));
    flags = temp_flags;
  }
  aom_codec_encode(&ctx, &frame, 0, 1, flags);

  // Get output packets
  aom_codec_iter_t iter = NULL;
  const aom_codec_cx_pkt_t *pkt;
  while ((pkt = aom_codec_get_cx_data(&ctx, &iter))) {
    if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
      // Optionally process encoded data
      if (pkt->data.frame.sz > 0) {
        // Use the data to avoid unused variable warnings
        volatile uint8_t *p = pkt->data.frame.buf;
        (void)p;
      }
    }
  }

  aom_img_free(&frame);
  cleanup_codec(&ctx);

  return 0;
}