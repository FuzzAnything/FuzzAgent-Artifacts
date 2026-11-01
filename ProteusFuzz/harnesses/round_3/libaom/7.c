// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control_typechecked_AV1E_SET_ENABLE_WARPED_MOTION at aomcx.h:2129:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_ENABLE_PALETTE at aomcx.h:2153:1 in aomcx.h
// aom_codec_control_typechecked_AV1E_SET_ENABLE_INTRABC at aomcx.h:2156:1 in aomcx.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control_typechecked_AV1E_GET_ACTIVEMAP at aomcx.h:1996:1 in aomcx.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_dx at av1_dx_iface.c:1786:20 in aomdx.h
// aom_codec_dec_init_ver at aom_decoder.c:25:17 in aom_decoder.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aom_codec.h"
#include "aom/aomdx.h"
#include "aom/aomcx.h"
#include "aom/aom_integer.h"
#include "aom/aom_image.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_external_partition.h"

/*
 * Helper function to initialize an encoder context.
 * Returns 0 on success, -1 on failure.
 */
static int init_encoder_context(aom_codec_ctx_t *ctx, aom_codec_enc_cfg_t *cfg) {
  if (ctx == NULL || cfg == NULL) {
    return -1;
  }

  // Initialize config with defaults
  // We must ensure the string "libaom-av1" is valid and the interface exists.
  // The error suggests the interface pointer might be null or invalid.
  // We check if the interface is available before initializing.
  const aom_codec_iface_t *iface = aom_codec_av1_cx();
  if (iface == NULL) {
      return -1;
  }

  // FIX: Pass the 'iface' pointer instead of the string name "libaom-av1".
  // The function signature expects aom_codec_iface_t *, not a string.
  if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) {
    return -1;
  }

  // Initialize the codec context
  if (aom_codec_enc_init(ctx, iface, cfg, 0) != AOM_CODEC_OK) {
    return -1;
  }

  return 0;
}

/*
 * Helper function to cleanup encoder context.
 */
static void cleanup_encoder_context(aom_codec_ctx_t *ctx) {
  if (ctx != NULL) {
    aom_codec_destroy(ctx);
  }
}

/*
 * Helper function to initialize a decoder context.
 * Returns 0 on success, -1 on failure.
 */
static int init_decoder_context(aom_codec_ctx_t *ctx) {
  if (ctx == NULL) {
    return -1;
  }

  const aom_codec_iface_t *iface = aom_codec_av1_dx();
  if (iface == NULL) {
      return -1;
  }

  aom_codec_dec_cfg_t cfg;
  memset(&cfg, 0, sizeof(cfg));
  cfg.threads = 1;

  if (aom_codec_dec_init(ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
    return -1;
  }

  return 0;
}

/*
 * Helper function to cleanup decoder context.
 */
static void cleanup_decoder_context(aom_codec_ctx_t *ctx) {
  if (ctx != NULL) {
    aom_codec_destroy(ctx);
  }
}

int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
  if (Size == 0) {
    return 0;
  }

  // Use data to decide which test path to take
  int choice = Data[0] % 5;
  int value = (Size > 1) ? (Data[1] % 256) : 0;

  aom_codec_ctx_t ctx;
  aom_codec_enc_cfg_t cfg;
  aom_codec_err_t res;

  // Initialize context based on choice
  if (choice < 3) {
    // Encoder tests
    if (init_encoder_context(&ctx, &cfg) != 0) {
      return 0;
    }

    switch (choice) {
      case 0:
        // Test AV1E_SET_ENABLE_WARPED_MOTION
        res = aom_codec_control_typechecked_AV1E_SET_ENABLE_WARPED_MOTION(&ctx, AV1E_SET_ENABLE_WARPED_MOTION, value);
        break;
      case 1:
        // Test AV1E_SET_ENABLE_PALETTE
        res = aom_codec_control_typechecked_AV1E_SET_ENABLE_PALETTE(&ctx, AV1E_SET_ENABLE_PALETTE, value);
        break;
      case 2:
        // Test AV1E_SET_ENABLE_INTRABC
        res = aom_codec_control_typechecked_AV1E_SET_ENABLE_INTRABC(&ctx, AV1E_SET_ENABLE_INTRABC, value);
        break;
      default:
        break;
    }

    cleanup_encoder_context(&ctx);
  } else {
    // Decoder or mixed tests
    if (init_decoder_context(&ctx) != 0) {
      return 0;
    }

    switch (choice) {
      case 3:
        // Test AOME_SET_ACTIVEMAP
        // Note: AOME_SET_ACTIVEMAP is typically an encoder control. 
        // Using generic control on a decoder context may fail, which is expected behavior for fuzzing.
        res = aom_codec_control(&ctx, AOME_SET_ACTIVEMAP, (void *)Data + 1);
        break;
      case 4:
        // Test AV1E_GET_ACTIVEMAP
        // This is an encoder control. Calling on a decoder context is a valid fuzz scenario to test error handling.
        aom_active_map_t active_map;
        memset(&active_map, 0, sizeof(active_map));
        active_map.rows = (Size > 2) ? (Data[2] % 100) : 1;
        active_map.cols = (Size > 3) ? (Data[3] % 100) : 1;
        
        // Allocate memory for the active map buffer
        size_t map_size = active_map.rows * active_map.cols;
        if (map_size > 0 && map_size < 1000000) { // Sanity check to prevent huge allocations
            active_map.active_map = (unsigned char *)malloc(map_size);
            if (active_map.active_map) {
                memset(active_map.active_map, 0, map_size);
                res = aom_codec_control_typechecked_AV1E_GET_ACTIVEMAP(&ctx, AV1E_GET_ACTIVEMAP, &active_map);
                free(active_map.active_map);
            } else {
                res = AOM_CODEC_ERROR;
            }
        } else {
            res = AOM_CODEC_ERROR;
        }
        break;
      default:
        break;
    }

    cleanup_decoder_context(&ctx);
  }

  (void)res;

  return 0;
}