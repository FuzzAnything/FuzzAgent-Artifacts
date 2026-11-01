// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
// aom_codec_peek_stream_info at aom_decoder.c:57:17 in aom_decoder.h
// aom_codec_peek_stream_info at aom_decoder.c:57:17 in aom_decoder.h
// aom_codec_decode at aom_decoder.c:94:17 in aom_decoder.h
// aom_codec_get_stream_info at aom_decoder.c:75:17 in aom_decoder.h
// aom_codec_peek_stream_info at aom_decoder.c:57:17 in aom_decoder.h
// aom_codec_peek_stream_info at aom_decoder.c:57:17 in aom_decoder.h
// aom_codec_error at aom_codec.c:56:13 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

#include "aom/aom_codec.h"
#include "aom/aom_encoder.h"
#include "aom/aom_decoder.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_external_partition.h"
#include "aom/aomcx.h"
#include "aom/aom_image.h"
#include "aom/aomdx.h"
#include "aom/aom_integer.h"
#include "aom/aom.h"

static void cleanup_codec(aom_codec_ctx_t *ctx) {
  if (ctx) {
    aom_codec_destroy(ctx);
  }
}

int LLVMFuzzerTestOneInput_209(const uint8_t *Data, size_t Size) {
  if (Size == 0) return 0;

  aom_codec_ctx_t ctx;
  aom_codec_iface_t *iface = aom_codec_av1_dx();
  if (!iface) return 0;

  aom_codec_dec_cfg_t cfg;
  memset(&cfg, 0, sizeof(cfg));
  cfg.threads = 1;
  cfg.w = 0;
  cfg.h = 0;
  cfg.allow_lowbitdepth = 1;

  aom_codec_flags_t flags = 0;

  // Test 1: Initialize decoder
  aom_codec_err_t res = aom_codec_dec_init_ver(&ctx, iface, &cfg, flags, AOM_DECODER_ABI_VERSION);
  if (res != AOM_CODEC_OK) {
    // If init fails, try to test error retrieval and peek functions without context
    const char *err = aom_codec_error(&ctx);
    (void)err;
    
    aom_codec_stream_info_t si;
    memset(&si, 0, sizeof(si));
    si.is_annexb = 1;
    aom_codec_peek_stream_info(iface, Data, Size, &si);
    si.is_annexb = 0;
    aom_codec_peek_stream_info(iface, Data, Size, &si);
    
    return 0;
  }

  // Test 2: Decode data
  if (Size > 0) {
    aom_codec_decode(&ctx, Data, Size, NULL);
  }

  // Test 3: Get stream info
  aom_codec_stream_info_t si;
  memset(&si, 0, sizeof(si));
  aom_codec_get_stream_info(&ctx, &si);

  // Test 4: Peek stream info (without full decode context dependency)
  si.is_annexb = 1;
  aom_codec_peek_stream_info(iface, Data, Size, &si);
  si.is_annexb = 0;
  aom_codec_peek_stream_info(iface, Data, Size, &si);

  // Test 5: Get error string
  const char *err = aom_codec_error(&ctx);
  (void)err;

  // Cleanup
  cleanup_codec(&ctx);

  return 0;
}