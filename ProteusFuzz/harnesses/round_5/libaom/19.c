// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_get_frame at aom_decoder.c:109:14 in aom_decoder.h
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
#include <aom/aomdx.h>
#include <aom/aomcx.h>
#include <aom/aom.h>
#include <aom/aom_codec.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_encoder.h>
#include <aom/aom_integer.h>

static int setup_decoder(aom_codec_ctx_t *ctx, const uint8_t *data, size_t size) {
  aom_codec_dec_cfg_t cfg;
  aom_codec_err_t ret;
  aom_codec_iface_t *iface;

  iface = aom_codec_av1_dx();
  if (!iface) return -1;

  memset(&cfg, 0, sizeof(cfg));
  cfg.threads = 1;

  ret = aom_codec_dec_init(ctx, iface, &cfg, 0);
  if (ret != AOM_CODEC_OK) return -1;

  // Feed the fuzzer input as a packet to the decoder
  if (size > 0) {
    aom_codec_decode(ctx, data, size, NULL);
  }

  return 0;
}

static void cleanup_decoder(aom_codec_ctx_t *ctx) {
  if (ctx && ctx->iface) {
    aom_codec_destroy(ctx);
  }
}

int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
  if (Size == 0) return 0;

  aom_codec_ctx_t ctx;
  memset(&ctx, 0, sizeof(ctx));

  // Initialize decoder
  if (setup_decoder(&ctx, Data, Size) != 0) {
    return 0;
  }

  // Retrieve frames and query control functions
  aom_codec_iter_t iter = NULL;
  const aom_codec_cx_pkt_t *pkt;

  while ((pkt = aom_codec_get_frame(&ctx, &iter)) != NULL) {
    if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
      // 1. AOMD_GET_ALTREF_PRESENT
      int altref_present;
      aom_codec_control(&ctx, AOMD_GET_ALTREF_PRESENT, &altref_present);

      // 2. AOMD_GET_ORDER_HINT
      int order_hint;
      aom_codec_control(&ctx, AOMD_GET_ORDER_HINT, &order_hint);

      // 3. AOMD_GET_STILL_PICTURE
      aom_still_picture_info still_info;
      memset(&still_info, 0, sizeof(still_info));
      aom_codec_control(&ctx, AOMD_GET_STILL_PICTURE, &still_info);

      // 4. AOMD_GET_FWD_KF_PRESENT
      int fwd_kf_present;
      aom_codec_control(&ctx, AOMD_GET_FWD_KF_PRESENT, &fwd_kf_present);

      // 5. AOMD_GET_LAST_REF_USED
      int last_ref_used;
      aom_codec_control(&ctx, AOMD_GET_LAST_REF_USED, &last_ref_used);

      // 6. AOMD_GET_S_FRAME_INFO
      aom_s_frame_info s_frame_info;
      memset(&s_frame_info, 0, sizeof(s_frame_info));
      aom_codec_control(&ctx, AOMD_GET_S_FRAME_INFO, &s_frame_info);
    }
  }

  cleanup_decoder(&ctx);
  return 0;
}