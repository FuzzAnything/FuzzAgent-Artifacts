// This fuzz driver is generated for library libvpx, aiming to fuzz the following functions:
// vpx_codec_get_cx_data at vpx_encoder.c:248:27 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_destroy at vpx_codec.c:66:17 in vpx_codec.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_img_alloc at vpx_image.c:162:14 in vpx_image.h
// vpx_img_set_rect at vpx_image.c:176:5 in vpx_image.h
// vpx_img_flip at vpx_image.c:229:6 in vpx_image.h
// vpx_codec_vp8_cx at vp8_cx_iface.c:1428:1 in vp8cx.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_enc_config_default at vpx_encoder.c:157:17 in vpx_encoder.h
// vpx_codec_enc_init_ver at vpx_encoder.c:29:17 in vpx_encoder.h
// vpx_img_free at vpx_image.c:252:6 in vpx_image.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
// vpx_codec_encode at vpx_encoder.c:193:17 in vpx_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vpx/vpx_image.h>
#include <vpx/vpx_encoder.h>
#include <vpx/vpx_codec.h>
#include <vpx/vp8cx.h>

int LLVMFuzzerTestOneInput_39(const uint8_t *Data, size_t Size) {
  if (Size < 24) return 0;

  // Extract parameters from input data safely
  uint32_t width = Data[0] | (Data[1] << 8);
  uint32_t height = Data[2] | (Data[3] << 8);
  
  // Limit dimensions to reasonable values to avoid OOM or invalid params
  if (width == 0 || height == 0) {
    width = 16;
    height = 16;
  }
  if (width > 4096) width = 4096;
  if (height > 4096) height = 4096;

  // Fuzz image format selection
  vpx_img_fmt_t fmt = (vpx_img_fmt_t)(Data[4] % 12);
  // Ensure valid format
  if (fmt == VPX_IMG_FMT_NONE) fmt = VPX_IMG_FMT_I420;

  // Fuzz alignment
  uint32_t align = 16;
  align = Data[5] % 65536;
  if (align == 0) align = 16;

  // Allocate image using vpx_img_alloc
  vpx_image_t *img = vpx_img_alloc(NULL, fmt, width, height, align);
  if (!img) return 0;

  // Fuzz vpx_img_set_rect
  unsigned int x = Data[6] % width;
  unsigned int y = Data[7] % height;
  unsigned int w = (Data[8] % width) + 1;
  unsigned int h = (Data[9] % height) + 1;
  // Ensure rectangle fits
  if (x + w > width) w = width - x;
  if (y + h > height) h = height - y;
  if (w > 0 && h > 0) {
    vpx_img_set_rect(img, x, y, w, h);
  }

  // Fuzz vpx_img_flip
  if (Data[10] & 1) {
    vpx_img_flip(img);
  }

  // Initialize encoder context
  vpx_codec_ctx_t ctx;
  memset(&ctx, 0, sizeof(ctx));
  
  // Get encoder interface
  vpx_codec_iface_t *iface = vpx_codec_vp8_cx();
  if (!iface) {
    vpx_img_free(img);
    return 0;
  }

  // Setup encoder config
  vpx_codec_enc_cfg_t cfg;
  vpx_codec_enc_config_default(iface, &cfg, 0);
  cfg.g_w = width;
  cfg.g_h = height;
  cfg.g_timebase.num = 1;
  cfg.g_timebase.den = 30;
  cfg.rc_target_bitrate = 1000;

  // Initialize encoder
  vpx_codec_err_t init_err = vpx_codec_enc_init(&ctx, iface, &cfg, 0);
  if (init_err != VPX_CODEC_OK) {
    vpx_img_free(img);
    return 0;
  }

  // Fuzz vpx_codec_encode
  // Generate PTS and duration
  int64_t pts = (int64_t)(Data[11] | (Data[12] << 8) | (Data[13] << 16) | (Data[14] << 24));
  unsigned long duration = (unsigned long)(Data[15] | (Data[16] << 8));
  if (duration == 0) duration = 1;
  vpx_enc_frame_flags_t flags = (vpx_enc_frame_flags_t)(Data[17] | (Data[18] << 8));
  vpx_enc_deadline_t deadline = (vpx_enc_deadline_t)(Data[19] | (Data[20] << 8));

  // Encode a frame
  vpx_codec_err_t err = vpx_codec_encode(&ctx, img, pts, duration, flags, deadline);
  (void)err; // Suppress unused warning

  // Flush encoder by passing NULL image
  err = vpx_codec_encode(&ctx, NULL, pts + 1, duration, flags, deadline);
  (void)err;

  // Drain any remaining data
  while (1) {
    const vpx_codec_cx_pkt_t *pkt;
    while ((pkt = vpx_codec_get_cx_data(&ctx, NULL)) != NULL) {
      // Process packet if needed, but do not free it manually
    }
    err = vpx_codec_encode(&ctx, NULL, pts + 2, duration, flags, deadline);
    if (err != VPX_CODEC_OK) break;
  }

  // Cleanup
  vpx_codec_destroy(&ctx);
  vpx_img_free(img);

  return 0;
}