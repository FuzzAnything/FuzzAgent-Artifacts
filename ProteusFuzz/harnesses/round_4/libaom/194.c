// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aomdx.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aom_integer.h>
#include <aom/aom_external_partition.h>

int LLVMFuzzerTestOneInput_194(const uint8_t *Data, size_t Size) {
  if (Size < 1) return 0;

  aom_codec_ctx_t ctx;
  memset(&ctx, 0, sizeof(ctx));

  aom_codec_dec_cfg_t cfg;
  memset(&cfg, 0, sizeof(cfg));
  cfg.threads = 1;
  cfg.w = 32;
  cfg.h = 32;

  const aom_codec_iface_t *iface = aom_codec_av1_dx();
  if (!iface) return 0;

  aom_codec_err_t ret = aom_codec_dec_init(&ctx, iface, &cfg, 0);
  if (ret != AOM_CODEC_OK) return 0;

  // Fuzz aom_codec_decode
  ret = aom_codec_decode(&ctx, Data, Size, NULL);
  (void)ret;

  // Fuzz control commands
  int ref_updates[16];
  memset(ref_updates, 0, sizeof(ref_updates));
  ret = aom_codec_control(&ctx, AOMD_GET_LAST_REF_UPDATES, ref_updates);
  (void)ret;

  int quantizer;
  ret = aom_codec_control(&ctx, AOMD_GET_LAST_QUANTIZER, &quantizer);
  (void)ret;

  int base_q_idx;
  ret = aom_codec_control(&ctx, AOMD_GET_BASE_Q_IDX, &base_q_idx);
  (void)ret;

  // Cleanup
  aom_codec_destroy(&ctx);

  return 0;
}