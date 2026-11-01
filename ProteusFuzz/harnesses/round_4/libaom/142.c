// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

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

// Helper to safely read integers from fuzz input
static int read_int(const uint8_t *data, size_t *offset, size_t size) {
    if (*offset + 4 > size) return 0;
    int val = (int)data[*offset] | 
              ((int)data[*offset+1] << 8) | 
              ((int)data[*offset+2] << 16) | 
              ((int)data[*offset+3] << 24);
    *offset += 4;
    return val;
}

// Helper to safely read bytes from fuzz input
static int read_byte(const uint8_t *data, size_t *offset, size_t size) {
    if (*offset >= size) return 0;
    int val = (int)data[*offset];
    *offset += 1;
    return val;
}

int LLVMFuzzerTestOneInput_142(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_iface_t *iface;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t ret;
    size_t idx = 0;
    
    // Initialize context
    memset(&ctx, 0, sizeof(ctx));
    
    // Get encoder interface
    iface = aom_codec_av1_cx();
    if (!iface) return 0;

    // Initialize default config
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // Use fuzz data to configure width/height safely
    int w = read_int(Data, &idx, Size);
    int h = read_int(Data, &idx, Size);
    
    if (w <= 0 || h <= 0) {
        w = 64;
        h = 64;
    }
    // Clamp to reasonable limits to avoid allocation issues
    if (w > 4096) w = 4096;
    if (h > 4096) h = 4096;
    
    cfg.g_w = (unsigned int)w;
    cfg.g_h = (unsigned int)h;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 1000;
    cfg.g_threads = 1;
    cfg.g_error_resilient = 1;

    // Initialize encoder
    ret = aom_codec_enc_init(&ctx, iface, &cfg, 0);
    if (ret != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz the control functions based on input data
    while (idx < Size) {
        int cmd = read_byte(Data, &idx, Size);
        int arg = read_int(Data, &idx, Size);
        
        switch (cmd) {
            case 0:
                // AV1E_SET_ENABLE_DIRECTIONAL_INTRA
                ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_DIRECTIONAL_INTRA, arg);
                break;
            case 1:
                // AV1E_SET_ENABLE_DNL_DENOISING
                ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_DNL_DENOISING, arg);
                break;
            case 2:
                // AV1E_SET_RENDER_SIZE
                // Requires two integers for width and height
                if (idx + 8 <= Size) {
                    int rw = read_int(Data, &idx, Size);
                    int rh = read_int(Data, &idx, Size);
                    if (rw > 0 && rh > 0 && rw <= 4096 && rh <= 4096) {
                        ret = aom_codec_control(&ctx, AV1E_SET_RENDER_SIZE, rw, rh);
                    }
                }
                break;
            case 3:
                // AV1E_SET_ENABLE_DIAGONAL_INTRA
                ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_DIAGONAL_INTRA, arg);
                break;
            case 4:
                // AV1E_SET_ENABLE_SMOOTH_INTERINTRA
                ret = aom_codec_control(&ctx, AV1E_SET_ENABLE_SMOOTH_INTERINTRA, arg);
                break;
            case 5:
                // AV1E_SET_SVC_REF_FRAME_CONFIG
                // Requires aom_svc_ref_frame_config_t
                // We need 7 + 7 + 8 = 22 bytes minimum for this struct
                if (idx + 22 <= Size) {
                    aom_svc_ref_frame_config_t svc_cfg;
                    memset(&svc_cfg, 0, sizeof(svc_cfg));
                    
                    // Read reference array (7 bytes)
                    for (int i = 0; i < 7 && idx + 1 <= Size; i++) {
                        svc_cfg.reference[i] = read_byte(Data, &idx, Size);
                        if (svc_cfg.reference[i] > 1) svc_cfg.reference[i] = 1;
                    }
                    
                    // Read ref_idx array (7 bytes)
                    for (int i = 0; i < 7 && idx + 1 <= Size; i++) {
                        svc_cfg.ref_idx[i] = read_byte(Data, &idx, Size);
                        if (svc_cfg.ref_idx[i] > 7) svc_cfg.ref_idx[i] = 7;
                    }
                    
                    // Read refresh array (8 bytes)
                    for (int i = 0; i < 8 && idx + 1 <= Size; i++) {
                        svc_cfg.refresh[i] = read_byte(Data, &idx, Size);
                        if (svc_cfg.refresh[i] > 1) svc_cfg.refresh[i] = 1;
                    }
                    
                    ret = aom_codec_control(&ctx, AV1E_SET_SVC_REF_FRAME_CONFIG, &svc_cfg);
                }
                break;
            default:
                // Skip unknown commands
                break;
        }
        
        // Check for errors but continue fuzzing
        if (ret != AOM_CODEC_OK) {
            // We can continue to test other controls even if one fails
        }
    }

    // Cleanup
    aom_codec_destroy(&ctx);
    
    return 0;
}