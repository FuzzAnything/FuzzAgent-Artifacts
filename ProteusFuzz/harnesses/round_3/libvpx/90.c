// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_peek_stream_info at vpx_decoder.c:65:17 in vpx_decoder.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
// vpx_codec_get_stream_info at vpx_decoder.c:85:17 in vpx_decoder.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <vpx/vpx_decoder.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8dx.h>

static vpx_codec_ctx_t *g_ctx = NULL;
static vpx_codec_iter_t g_iter = NULL;

int LLVMFuzzerTestOneInput_90(const uint8_t *Data, size_t Size) {
  if (Size == 0) return 0;

  // Get the VP8 decoder interface using the standard library function
  vpx_codec_iface_t *iface = vpx_codec_vp8_dx();
  if (!iface) return 0;

  // Try to peek stream info without initializing a decoder context
  vpx_codec_stream_info_t si;
  memset(&si, 0, sizeof(si));
  si.sz = sizeof(si);
  
  // Attempt to parse stream info directly from fuzzed data
  vpx_codec_peek_stream_info(iface, Data, Size, &si);

  // Initialize decoder context if not already done
  if (g_ctx == NULL) {
    g_ctx = (vpx_codec_ctx_t *)malloc(sizeof(vpx_codec_ctx_t));
    if (!g_ctx) return 0;
    memset(g_ctx, 0, sizeof(vpx_codec_ctx_t));

    vpx_codec_dec_cfg_t cfg;
    memset(&cfg, 0, sizeof(cfg));
    cfg.threads = 1;
    cfg.w = 0;
    cfg.h = 0;

    vpx_codec_err_t init_res = vpx_codec_dec_init_ver(
        g_ctx,
        iface,
        &cfg,
        0,
        VPX_DECODER_ABI_VERSION
    );

    if (init_res != VPX_CODEC_OK) {
      // Initialization failed, but we can still try to get error details
      const char *err = vpx_codec_error_detail(g_ctx);
      (void)err; // Suppress unused warning
      free(g_ctx);
      g_ctx = NULL;
      return 0;
    }
  }

  // Decode the fuzzed data
  vpx_codec_err_t decode_res = vpx_codec_decode(
      g_ctx,
      Data,
      Size,
      NULL,
      0
  );

  // Check for errors and retrieve details
  if (decode_res != VPX_CODEC_OK) {
    const char *err_detail = vpx_codec_error_detail(g_ctx);
    (void)err_detail; // Suppress unused warning
  }

  // Try to get stream info after decoding
  vpx_codec_stream_info_t si_out;
  memset(&si_out, 0, sizeof(si_out));
  si_out.sz = sizeof(si_out);
  vpx_codec_get_stream_info(g_ctx, &si_out);

  // Iterate over decoded frames
  g_iter = NULL;
  while (1) {
    vpx_image_t *img = vpx_codec_get_frame(g_ctx, &g_iter);
    if (img == NULL) break;
    // Frame is valid until next decode call, but we don't need to do anything with it for fuzzing
  }

  // Flush the decoder
  vpx_codec_decode(g_ctx, NULL, 0, NULL, 0);

  return 0;
}