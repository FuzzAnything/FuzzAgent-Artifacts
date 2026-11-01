// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_img_free at aom_image.c:304:6 in aom_image.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_img_alloc at aom_image.c:200:14 in aom_image.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "aomdx.h"
#include "aom_encoder.h"
#include "aom_frame_buffer.h"
#include "aom_decoder.h"
#include "aom_image.h"
#include "aom.h"
#include "aom_integer.h"
#include "aom_codec.h"
#include "aomcx.h"
#include "aom_external_partition.h"

static int fuzz_codec_control(aom_codec_ctx_t *ctx, int control_id, int value) {
    if (!ctx) return 0;
    
    aom_codec_err_t res = AOM_CODEC_OK;
    
    switch (control_id) {
        case 1: // AV1E_SET_ENABLE_QM
            res = aom_codec_control(ctx, AV1E_SET_ENABLE_QM, value);
            break;
        case 2: // AOME_SET_ENABLEAUTOBWDREF
            res = aom_codec_control(ctx, AOME_SET_ENABLEAUTOBWDREF, value);
            break;
        case 3: // AV1E_SET_DELTAQ_MODE
            res = aom_codec_control(ctx, AV1E_SET_DELTAQ_MODE, value);
            break;
        case 4: // AV1E_SET_QM_MAX
            res = aom_codec_control(ctx, AV1E_SET_QM_MAX, value);
            break;
        case 5: // AV1E_SET_TILE_ROWS
            res = aom_codec_control(ctx, AV1E_SET_TILE_ROWS, value);
            break;
        case 6: // AV1E_SET_DELTAQ_STRENGTH
            res = aom_codec_control(ctx, AV1E_SET_DELTAQ_STRENGTH, value);
            break;
        default:
            return 0;
    }
    
    return (res == AOM_CODEC_OK) ? 1 : 0;
}

int LLVMFuzzerTestOneInput_52(const uint8_t *Data, size_t Size) {
    // Ensure minimum size for safe access to initial bytes
    if (Size < 4) return 0;

    aom_codec_ctx_t ctx = {0};
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return 0;

    aom_codec_enc_cfg_t cfg;
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use input data to configure encoder settings safely
    uint32_t width = (Data[0] % 64) + 16;
    uint32_t height = (Data[1] % 64) + 16;
    uint32_t fps_num = (Data[2] % 60) + 1;
    uint32_t fps_den = (Data[3] % 60) + 1;

    cfg.g_w = width;
    cfg.g_h = height;
    cfg.g_timebase.num = fps_num;
    cfg.g_timebase.den = fps_den;
    cfg.g_threads = 0; // Let libaom decide
    cfg.rc_target_bitrate = 100; // Default safe value
    
    // Only access Data[4] if Size allows
    if (Size > 4) {
        cfg.rc_target_bitrate = (Data[4] % 1000) + 100;
    }
    
    cfg.g_error_resilient = 0;

    // Initialize encoder
    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Prepare a dummy frame to encode to ensure the encoder is in a valid state
    aom_image_t frame;
    aom_img_alloc(&frame, AOM_IMG_FMT_I420, width, height, 1);
    
    // Fill frame with data from input safely
    size_t frame_size = width * height;
    memset(frame.planes[0], 0, frame_size);
    
    size_t data_offset = 4;
    if (Size > data_offset) {
        size_t copy_len = (Size - data_offset) < frame_size ? (Size - data_offset) : frame_size;
        for (size_t i = 0; i < copy_len; i++) {
            frame.planes[0][i] = Data[data_offset + i];
        }
    }
    
    // Encode one frame to warm up the context
    // We send a frame to ensure the encoder context is fully initialized
    aom_codec_frame_flags_t flags = 0;
    aom_codec_control(&ctx, AV1E_SET_TILE_COLUMNS, 0); 
    
    // Fuzz the specific control functions
    // We iterate through the 6 target functions
    for (int i = 0; i < 6; i++) {
        int control_id = i + 1;
        int value = 0;
        
        // Calculate index safely
        size_t val_idx = 4 + i;
        if (Size > val_idx) {
            value = Data[val_idx];
        }
        
        // For some controls, values might need to be constrained
        if (control_id == 3) { // AV1E_SET_DELTAQ_MODE: usually 0 or 1
            value = value % 2;
        } else if (control_id == 5) { // AV1E_SET_TILE_ROWS: usually small int
            value = value % 8;
        } else if (control_id == 6) { // AV1E_SET_DELTAQ_STRENGTH: usually 0-63
            value = value % 64;
        } else if (control_id == 4) { // AV1E_SET_QM_MAX: usually 0-255
            value = value % 256;
        } else {
            // Boolean flags
            value = value % 2;
        }

        fuzz_codec_control(&ctx, control_id, value);
    }

    // Cleanup
    aom_codec_destroy(&ctx);
    aom_img_free(&frame);

    return 0;
}