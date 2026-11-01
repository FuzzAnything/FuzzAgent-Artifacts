// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
// aom_codec_encode at aom_encoder.c:168:17 in aom_encoder.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <aom/aom_decoder.h>
#include <aom/aom_encoder.h>
#include <aom/aom_codec.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aomcx.h>
#include <aom/aom_external_partition.h>

/* 
 * Helper function to invoke codec controls.
 * Uses the standard aom_codec_control API.
 */
static int fuzz_encoder_controls(aom_codec_ctx_t *ctx, int control_id, int value) {
    if (!ctx) return 0;
    
    // aom_codec_control expects an integer argument for these specific controls
    return aom_codec_control(ctx, control_id, value);
}

int LLVMFuzzerTestOneInput_51(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;

    // Extract parameters from fuzz input
    int width = Data[0] | (Data[1] << 8);
    int height = Data[2] | (Data[3] << 8);
    
    // Ensure reasonable dimensions to avoid immediate allocation failures
    if (width == 0) width = 16;
    if (height == 0) height = 16;
    if (width > 4096) width = 4096;
    if (height > 4096) height = 4096;

    // Initialize encoder configuration
    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(NULL, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.g_threads = 1;
    cfg.g_lag_in_frames = 0;
    cfg.rc_target_bitrate = 1000;
    cfg.rc_min_quantizer = 0;
    cfg.rc_max_quantizer = 63;

    // Initialize codec context
    aom_codec_ctx_t codec;
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    if (aom_codec_enc_init(&codec, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Prepare a dummy frame
    aom_image_t frame;
    aom_img_alloc(&frame, AOM_IMG_FMT_I420, width, height, 1);
    
    // Fill frame with fuzz data if available
    // Use height and stride[0] for Y plane
    if (Size > 4) {
        for (int i = 0; i < height; i++) {
            size_t bytes_to_copy = (Size - 4 < (size_t)width) ? (Size - 4) : (size_t)width;
            memcpy(frame.planes[0] + i * frame.stride[0], 
                   Data + 4, 
                   bytes_to_copy);
        }
    }

    // Set frame timestamp (pts)
    aom_codec_pts_t pts = 0;

    // Submit frame for encoding
    aom_enc_frame_flags_t flags = 0;
    if (Size > 10 && Data[10] & 0x01) flags |= AOM_EFLAG_FORCE_KF;
    
    // aom_codec_encode signature: (ctx, img, pts, duration, flags)
    if (aom_codec_encode(&codec, &frame, pts, 1, flags) != AOM_CODEC_OK) {
        aom_img_free(&frame);
        aom_codec_destroy(&codec);
        return 0;
    }

    // Fuzz the specific control IDs mentioned in the prompt (mapped to standard AV1E constants)
    // We use the standard AV1E_* constants which correspond to the functionality described.
    
    // 1. AV1E_SET_ENABLE_TX_SIZE_SEARCH
    int val1 = (Size > 5) ? (Data[5] % 2) : 0;
    fuzz_encoder_controls(&codec, AV1E_SET_ENABLE_TX_SIZE_SEARCH, val1);

    // 2. AV1E_SET_CDF_UPDATE_MODE
    int val2 = (Size > 6) ? (Data[6] % 3) : 0; // Assuming 0-2 range
    fuzz_encoder_controls(&codec, AV1E_SET_CDF_UPDATE_MODE, val2);

    // 3. AV1E_SET_ENABLE_GLOBAL_MOTION
    int val3 = (Size > 7) ? (Data[7] % 2) : 0;
    fuzz_encoder_controls(&codec, AV1E_SET_ENABLE_GLOBAL_MOTION, val3);

    // 4. AV1E_SET_DISABLE_TRELLIS_QUANT
    int val4 = (Size > 8) ? (Data[8] % 2) : 0;
    fuzz_encoder_controls(&codec, AV1E_SET_DISABLE_TRELLIS_QUANT, val4);

    // 5. AV1E_SET_MODE_COST_UPD_FREQ
    int val5 = (Size > 9) ? (Data[9] % 100) : 0;
    fuzz_encoder_controls(&codec, AV1E_SET_MODE_COST_UPD_FREQ, val5);

    // 6. AV1E_SET_ENABLE_RESTORATION
    int val6 = (Size > 10) ? (Data[10] % 2) : 0;
    fuzz_encoder_controls(&codec, AV1E_SET_ENABLE_RESTORATION, val6);

    // Flush the encoder
    if (aom_codec_encode(&codec, NULL, pts, 1, 0) != AOM_CODEC_OK) {
        // Ignore errors during flush for fuzzing robustness
    }

    // Cleanup
    aom_img_free(&frame);
    aom_codec_destroy(&codec);

    return 0;
}