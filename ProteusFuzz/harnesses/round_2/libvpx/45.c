// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_codec_vp8_dx at vp8_dx_iface.c:726:1 in vp8dx.h
// vpx_codec_vp9_dx at vp9_dx_iface.c:712:1 in vp8dx.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_dec_init_ver at vpx_decoder.c:24:17 in vpx_decoder.h
// vpx_codec_register_put_frame_cb at vpx_decoder.c:133:17 in vpx_decoder.h
// vpx_codec_register_put_slice_cb at vpx_decoder.c:153:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_decode at vpx_decoder.c:104:17 in vpx_decoder.h
// vpx_codec_get_stream_info at vpx_decoder.c:85:17 in vpx_decoder.h
// vpx_codec_get_frame at vpx_decoder.c:122:14 in vpx_decoder.h
// vpx_codec_error_detail at vpx_codec.c:59:13 in vpx_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_decoder.h>
#include <vpx/vp8dx.h>
#include <vpx/vp8cx.h>
#include <vpx/vpx_codec.h>

static void dummy_put_frame_cb(void *user_priv, const vpx_image_t *frame) {
  (void)user_priv;
  (void)frame;
}

static void dummy_put_slice_cb(void *user_priv, const vpx_image_t *frame,
                               const struct vpx_image_rect *slice_rect,
                               const struct vpx_image_rect *frame_rect) {
  (void)user_priv;
  (void)frame;
  (void)slice_rect;
  (void)frame_rect;
}

int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size) {
  vpx_codec_ctx_t ctx;
  vpx_codec_dec_cfg_t cfg;
  vpx_codec_stream_info_t si;
  vpx_codec_iter_t iter = NULL;
  vpx_image_t *img = NULL;
  vpx_codec_iface_t *iface;
  vpx_codec_err_t res;

  memset(&ctx, 0, sizeof(ctx));
  memset(&cfg, 0, sizeof(cfg));
  memset(&si, 0, sizeof(si));

  // Select interface based on input data to diversify states
  if (Size > 0 && Data[0] % 2 == 0) {
    iface = vpx_codec_vp8_dx();
  } else {
    iface = vpx_codec_vp9_dx();
  }

  if (!iface) {
    return 0;
  }

  // Initialize decoder configuration
  cfg.threads = 1;
  if (Size > 1) {
    cfg.w = Data[1] * 16;
    cfg.h = (Size > 2 ? Data[2] : 1) * 16;
  }
  if (cfg.w == 0) cfg.w = 16;
  if (cfg.h == 0) cfg.h = 16;

  // Initialize decoder
  res = vpx_codec_dec_init_ver(&ctx, iface, &cfg, 0, VPX_DECODER_ABI_VERSION);
  if (res != VPX_CODEC_OK) {
    // Try with NULL config as well
    res = vpx_codec_dec_init_ver(&ctx, iface, NULL, 0, VPX_DECODER_ABI_VERSION);
    if (res != VPX_CODEC_OK) {
      return 0;
    }
  }

  // Register put frame callback (required for decode to work properly)
  vpx_codec_register_put_frame_cb(&ctx, dummy_put_frame_cb, NULL);

  // Register put slice callback (Target API 1)
  vpx_codec_register_put_slice_cb(&ctx, dummy_put_slice_cb, (void *)Data);

  // Decode data (Target API 5)
  // Pass data directly or NULL to flush
  if (Size > 0) {
    res = vpx_codec_decode(&ctx, Data, Size, NULL, 0);
  } else {
    res = vpx_codec_decode(&ctx, NULL, 0, NULL, 0);
  }

  // Get stream info (Target API 2)
  si.sz = sizeof(si);
  res = vpx_codec_get_stream_info(&ctx, &si);
  (void)res;

  // Iterate frames (Target API 6)
  iter = NULL;
  while ((img = vpx_codec_get_frame(&ctx, &iter)) != NULL) {
    // Use image data to ensure path coverage
    if (img->planes[VPX_PLANE_Y]) {
      (void)img->planes[VPX_PLANE_Y][0];
    }
  }

  // Get error detail (Target API 3)
  const char *err_detail = vpx_codec_error_detail(&ctx);
  (void)err_detail;

  // Cleanup
  vpx_codec_destroy(&ctx);

  return 0;
}