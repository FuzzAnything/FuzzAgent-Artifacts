// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_codec_get_cx_data at aom_encoder.c:198:27 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
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

static int create_dummy_image(aom_image_t *img, uint32_t w, uint32_t h, uint8_t bit_depth) {
  if (!img || w == 0 || h == 0) return 0;

  aom_img_fmt_t fmt = (bit_depth > 8) ? AOM_IMG_FMT_I42016 : AOM_IMG_FMT_I420;
  // aom_img_alloc returns AOM_OK (0) on success
  if (aom_img_alloc(img, fmt, w, h, 32) != 0) {
    return 0;
  }
  img->bit_depth = bit_depth;
  img->range = AOM_CR_STUDIO_RANGE;
  img->cp = AOM_CICP_CP_BT_709;
  img->tc = AOM_CICP_TC_BT_709;
  img->mc = AOM_CICP_MC_BT_709;
  img->csp = AOM_CSP_COLOCATED;
  img->monochrome = 0;

  // Fill with deterministic data based on dimensions to avoid uninitialized memory
  for (int p = 0; p < 3; p++) {
    if (img->planes[p]) {
      uint8_t *plane = img->planes[p];
      int stride = img->stride[p];
      uint32_t pw = (p == 0) ? w : (w + 1) / 2;
      uint32_t ph = (p == 0) ? h : (h + 1) / 2;
      for (uint32_t y = 0; y < ph; y++) {
        for (uint32_t x = 0; x < pw; x++) {
          uint8_t val = (x + y) % (1 << bit_depth);
          if (bit_depth > 8) {
            uint16_t *plane16 = (uint16_t *)plane;
            plane16[y * (stride / 2) + x] = val;
          } else {
            plane[y * stride + x] = val;
          }
        }
      }
    }
  }
  return 1;
}

int LLVMFuzzerTestOneInput_149(const uint8_t *Data, size_t Size) {
  if (Size < 4) return 0;

  // Initialize context
  aom_codec_ctx_t ctx;
  aom_codec_iface_t *iface = aom_codec_av1_cx();
  if (!iface) return 0;

  aom_codec_enc_cfg_t cfg;
  if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) return 0;

  // Fuzz configuration parameters
  uint32_t w = (Data[0] % 128) + 16;
  uint32_t h = (Data[1] % 128) + 16;
  uint8_t bit_depth = (Data[2] % 4) + 8; // 8, 10, 11, 12
  uint32_t gop_size = (Data[3] % 10) + 1;

  // Fix: Use correct member names for aom_codec_enc_cfg_t
  cfg.g_w = w;
  cfg.g_h = h;
  cfg.g_bit_depth = bit_depth;
  cfg.kf_max_dist = gop_size; 
  cfg.rc_target_bitrate = 1000 + (Data[0] * 100);
  cfg.rc_min_quantizer = 0;
  cfg.rc_max_quantizer = 63;
  cfg.g_error_resilient = 0;
  // cfg.tune is not a direct member in older versions or specific structs, 
  // it is often set via aom_codec_control or specific config fields. 
  // For libaom, we can set it via cfg.tune if available, otherwise ignore.
  // In standard aom_codec_enc_cfg_t, 'tune' is not a direct member in all versions.
  // We will skip setting it directly to avoid compilation errors if the struct differs.
  // If the struct has 'tune', it would be: cfg.tune = AOM_TUNE_PSNR; 
  // Since the error says no member named 'tune', we omit it.

  if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) return 0;

  // Prepare image
  aom_image_t img;
  memset(&img, 0, sizeof(img));
  if (!create_dummy_image(&img, w, h, bit_depth)) {
    aom_codec_destroy(&ctx);
    return 0;
  }

  // Fuzz encoding loop
  uint64_t pts = 0;
  uint64_t duration = 1 + (Data[0] % 10);
  int frames_to_encode = (Size > 10) ? (Data[10] % 5) + 1 : 1;

  for (int i = 0; i < frames_to_encode; i++) {
    aom_enc_frame_flags_t flags = 0;
    if (Size > i + 11) {
      flags = Data[i + 11] % 4;
    }

    aom_codec_err_t res = aom_codec_encode(&ctx, &img, pts, duration, flags);
    if (res != AOM_CODEC_OK && res != AOM_CODEC_ERROR) {
      // Expected errors like INVALID_PARAM are acceptable in fuzzing
    }

    // Check for output
    aom_codec_cx_pkt_t *pkt;
    while ((pkt = aom_codec_get_cx_data(&ctx, NULL)) != NULL) {
      if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
        // Process encoded frame if needed
        // aom_codec_cx_pkt_data_free is not a standard public API function in libaom.
        // The data is managed by the codec context. We just discard the packet pointer.
        // The actual data inside pkt->data.frame is managed by the codec.
        // We do not free it manually here.
      }
      // Move to next packet
    }

    pts += duration;
  }

  // Flush encoder
  while (1) {
    aom_codec_err_t res = aom_codec_encode(&ctx, NULL, 0, 0, 0);
    if (res != AOM_CODEC_OK) break;

    aom_codec_cx_pkt_t *pkt;
    while ((pkt = aom_codec_get_cx_data(&ctx, NULL)) != NULL) {
      if (pkt->kind == AOM_CODEC_CX_FRAME_PKT) {
        // Data managed by codec
      }
    }
    // Break if no more data
    if (aom_codec_get_cx_data(&ctx, NULL) == NULL) break;
  }

  // Cleanup
  aom_img_free(&img);
  aom_codec_destroy(&ctx);

  return 0;
}