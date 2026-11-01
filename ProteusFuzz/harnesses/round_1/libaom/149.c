// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "aom/aom_decoder.h"
#include "aom/aomdx.h"
#include "aom/aom_external_partition.h"
#include "aom/aom_integer.h"
#include "aom/aom_image.h"
#include "aom/aom_encoder.h"
#include "aom/aomcx.h"
#include "aom/aom.h"
#include "aom/aom_codec.h"
#include "aom/aom_frame_buffer.h"

/*
 * Note: The provided function signatures for the target APIs were empty in the prompt.
 * Based on standard libaom conventions and the context of the function names,
 * these functions are likely wrappers around aom_codec_control() with specific
 * control IDs (e.g., AOME_GET_LAST_QUANTIZER, AOMD_GET_LAST_QUANTIZER, etc.).
 *
 * Since the exact signatures are missing, this fuzz driver assumes the standard
 * aom_codec_control pattern:
 *   aom_codec_err_t aom_codec_control(aom_codec_ctx_t *ctx, int request, ...);
 *
 * The "typechecked" variants likely enforce specific argument types.
 * We will attempt to call the underlying aom_codec_control with the likely
 * control IDs. If the specific "typechecked" functions exist as separate
 * symbols with different signatures, the linker will fail unless the exact
 * headers defining them are available.
 *
 * For the purpose of this fuzz driver, we will map the names to their likely
 * control IDs or attempt to call them if they are defined in the included headers.
 *
 * Control IDs (inferred from names):
 * - AOME_GET_LAST_QUANTIZER
 * - AOMD_GET_LAST_QUANTIZER
 * - AV1D_GET_FRAME_HEADER_INFO
 * - AV1D_SET_EXT_REF_PTR
 * - AOME_GET_LAST_QUANTIZER_64
 * - AOMD_GET_BASE_Q_IDX
 *
 * If the specific functions 'aom_codec_control_typechecked_...' are not
 * defined in the headers, we will rely on the standard aom_codec_control
 * with the corresponding enum values.
 */

/* Helper to initialize a dummy image for reference frames */
static int init_dummy_image(aom_image_t *img, int width, int height) {
  if (!img) return 0;
  img->fmt = AOM_IMG_FMT_I420;
  img->w = width;
  img->h = height;
  img->d_w = width;
  img->d_h = height;
  img->bit_depth = 8;
  img->x_chroma_shift = 1;
  img->y_chroma_shift = 1;
  img->stride[0] = width;
  img->stride[1] = (width + 1) / 2;
  img->stride[2] = (width + 1) / 2;
  
  // Allocate memory for planes
  size_t y_size = width * height;
  size_t uv_size = ((width + 1) / 2) * ((height + 1) / 2);
  
  img->planes[0] = malloc(y_size);
  img->planes[1] = malloc(uv_size);
  img->planes[2] = malloc(uv_size);
  
  if (!img->planes[0] || !img->planes[1] || !img->planes[2]) {
    free(img->planes[0]);
    free(img->planes[1]);
    free(img->planes[2]);
    img->planes[0] = img->planes[1] = img->planes[2] = NULL;
    return 0;
  }
  
  memset(img->planes[0], 128, y_size);
  memset(img->planes[1], 128, uv_size);
  memset(img->planes[2], 128, uv_size);
  
  img->sz = y_size + uv_size + uv_size;
  img->user_priv = NULL;
  img->metadata = NULL;
  img->fb_priv = NULL;
  
  return 1;
}

/* Helper to clean up image */
static void cleanup_image(aom_image_t *img) {
  if (!img) return;
  if (img->planes[0]) free(img->planes[0]);
  if (img->planes[1]) free(img->planes[1]);
  if (img->planes[2]) free(img->planes[2]);
  img->planes[0] = img->planes[1] = img->planes[2] = NULL;
}

int LLVMFuzzerTestOneInput_149(const uint8_t *Data, size_t Size) {
  if (Size < 4) return 0;

  // Use the first few bytes to determine which API to test
  int api_choice = Data[0] % 6;
  
  aom_codec_ctx_t ctx;
  memset(&ctx, 0, sizeof(ctx));
  aom_codec_iface_t *decoder_iface = NULL;
  aom_codec_iface_t *encoder_iface = NULL;
  
  // Initialize decoder context
  decoder_iface = aom_codec_av1_dx();
  if (!decoder_iface) return 0;
  
  aom_codec_dec_cfg_t dec_cfg;
  memset(&dec_cfg, 0, sizeof(dec_cfg));
  dec_cfg.threads = 1;
  dec_cfg.w = 32;
  dec_cfg.h = 32;
  
  if (aom_codec_dec_init(&ctx, decoder_iface, &dec_cfg, 0) != AOM_CODEC_OK) {
    return 0;
  }

  // Initialize encoder context (for encoder specific APIs)
  aom_codec_ctx_t enc_ctx;
  memset(&enc_ctx, 0, sizeof(enc_ctx));
  encoder_iface = aom_codec_av1_cx();
  if (!encoder_iface) {
    aom_codec_destroy(&ctx);
    return 0;
  }
  
  aom_codec_enc_cfg_t enc_cfg;
  memset(&enc_cfg, 0, sizeof(enc_cfg));
  aom_codec_enc_config_default(encoder_iface, &enc_cfg, 0);
  enc_cfg.g_w = 32;
  enc_cfg.g_h = 32;
  enc_cfg.rc_target_bitrate = 1000;
  
  if (aom_codec_enc_init(&enc_ctx, encoder_iface, &enc_cfg, 0) != AOM_CODEC_OK) {
    aom_codec_destroy(&ctx);
    return 0;
  }

  int result = 0;

  switch (api_choice) {
    case 0:
      // aom_codec_control_typechecked_AOME_GET_LAST_QUANTIZER
      // Likely maps to AOME_GET_LAST_QUANTIZER
      {
        int quantizer = 0;
        // Assuming standard control interface if typechecked wrapper isn't directly callable
        // If the function exists as a direct call, we would call it here.
        // Since signature is unknown, we use aom_codec_control with the ID.
        // Note: AOME_GET_LAST_QUANTIZER is typically an encoder control.
        aom_codec_control(&enc_ctx, AOME_GET_LAST_QUANTIZER, &quantizer);
        (void)quantizer;
      }
      break;

    case 1:
      // aom_codec_control_typechecked_AOMD_GET_LAST_QUANTIZER
      // Likely maps to AOMD_GET_LAST_QUANTIZER
      {
        int quantizer = 0;
        aom_codec_control(&ctx, AOMD_GET_LAST_QUANTIZER, &quantizer);
        (void)quantizer;
      }
      break;

    case 2:
      // aom_codec_control_typechecked_AV1D_GET_FRAME_HEADER_INFO
      // Likely maps to AV1D_GET_FRAME_HEADER_INFO
      {
        // This usually requires a pointer to a header info struct.
        // We pass a dummy pointer or a stack allocated struct if known.
        // Since the struct isn't provided, we pass a dummy int pointer to trigger the call.
        int dummy_info = 0;
        aom_codec_control(&ctx, AV1D_GET_FRAME_HEADER_INFO, &dummy_info);
        (void)dummy_info;
      }
      break;

    case 3:
      // aom_codec_control_typechecked_AV1D_SET_EXT_REF_PTR
      // Likely maps to AV1D_SET_EXT_REF_PTR
      {
        av1_ext_ref_frame_t ext_ref;
        memset(&ext_ref, 0, sizeof(ext_ref));
        
        aom_image_t img;
        memset(&img, 0, sizeof(img));
        if (init_dummy_image(&img, 32, 32)) {
          ext_ref.img = &img;
          ext_ref.num = 1;
          
          aom_codec_control(&ctx, AV1D_SET_EXT_REF_PTR, &ext_ref);
          cleanup_image(&img);
        }
      }
      break;

    case 4:
      // aom_codec_control_typechecked_AOME_GET_LAST_QUANTIZER_64
      // Likely maps to AOME_GET_LAST_QUANTIZER_64
      {
        int64_t quantizer64 = 0;
        aom_codec_control(&enc_ctx, AOME_GET_LAST_QUANTIZER_64, &quantizer64);
        (void)quantizer64;
      }
      break;

    case 5:
      // aom_codec_control_typechecked_AOMD_GET_BASE_Q_IDX
      // Likely maps to AOMD_GET_BASE_Q_IDX
      {
        int base_q_idx = 0;
        aom_codec_control(&ctx, AOMD_GET_BASE_Q_IDX, &base_q_idx);
        (void)base_q_idx;
      }
      break;

    default:
      break;
  }

  // Cleanup
  aom_codec_destroy(&ctx);
  aom_codec_destroy(&enc_ctx);

  return 0;
}