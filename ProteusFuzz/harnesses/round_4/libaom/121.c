// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_err_to_string at aom_codec.c:36:13 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
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

static aom_codec_enc_cfg_t *get_fuzzed_config(const uint8_t *Data, size_t Size) {
    aom_codec_enc_cfg_t *cfg = (aom_codec_enc_cfg_t *)malloc(sizeof(aom_codec_enc_cfg_t));
    if (!cfg) return NULL;

    memset(cfg, 0, sizeof(aom_codec_enc_cfg_t));

    // Initialize with defaults first to ensure valid base state
    if (aom_codec_enc_config_default(aom_codec_av1_cx(), cfg, 0) != AOM_CODEC_OK) {
        free(cfg);
        return NULL;
    }

    // Ensure we have enough data before accessing specific indices
    // Minimum required size to safely access up to Data[44]
    size_t min_size = 45;
    if (Size < min_size) {
        free(cfg);
        return NULL;
    }

    // Use fuzzing data to perturb configuration values
    cfg->g_w = (Data[0] % 4096) + 16;
    cfg->g_h = (Data[1] % 4096) + 16;
    cfg->g_threads = (Data[2] % 8) + 1;
    cfg->g_limit = (Data[3] % 100) + 1;
    cfg->g_bit_depth = (Data[4] % 3 == 0) ? AOM_BITS_8 : ((Data[4] % 3 == 1) ? AOM_BITS_10 : AOM_BITS_12);
    cfg->g_input_bit_depth = cfg->g_bit_depth;
    
    cfg->g_timebase.num = (Data[5] % 100) + 1;
    cfg->g_timebase.den = (Data[6] % 100) + 1;

    cfg->rc_target_bitrate = (Data[7] % 1000) + 100;
    cfg->rc_min_quantizer = (Data[8] % 64);
    cfg->rc_max_quantizer = (Data[9] % 64) + cfg->rc_min_quantizer;
    
    cfg->kf_min_dist = (Data[10] % 500);
    cfg->kf_max_dist = (Data[11] % 500) + cfg->kf_min_dist;
    
    cfg->rc_end_usage = (enum aom_rc_mode)(Data[12] % 4);
    cfg->kf_mode = (enum aom_kf_mode)(Data[13] % 3);

    // Superres settings
    cfg->rc_superres_mode = (aom_superres_mode)(Data[14] % 3);
    cfg->rc_superres_denominator = (Data[15] % 9) + 8; // 8 to 16
    cfg->rc_superres_qthresh = (Data[16] % 63) + 1;

    // Error resilient flags
    cfg->g_error_resilient = (aom_codec_er_flags_t)(Data[17] % 7);

    // Tile settings
    cfg->tile_width_count = (Data[18] % 5);
    cfg->tile_height_count = (Data[19] % 5);
    
    // Safe access for tile arrays
    int tile_idx = 20;
    for (int i = 0; i < cfg->tile_width_count && i < MAX_TILE_WIDTHS; i++) {
        if (tile_idx < Size) {
            cfg->tile_widths[i] = (Data[tile_idx] % 100) + 1;
            tile_idx++;
        }
    }
    for (int i = 0; i < cfg->tile_height_count && i < MAX_TILE_HEIGHTS; i++) {
        if (tile_idx < Size) {
            cfg->tile_heights[i] = (Data[tile_idx] % 100) + 1;
            tile_idx++;
        }
    }

    // cfg_options_t inside
    if (tile_idx + 7 < Size) {
        cfg->encoder_cfg.super_block_size = (Data[tile_idx] % 3 == 0) ? 0 : ((Data[tile_idx] % 3 == 1) ? 64 : 128);
        tile_idx++;
        cfg->encoder_cfg.disable_cdef = (Data[tile_idx] % 2);
        tile_idx++;
        cfg->encoder_cfg.disable_lr = (Data[tile_idx] % 2);
        tile_idx++;
        cfg->encoder_cfg.disable_warp_motion = (Data[tile_idx] % 2);
        tile_idx++;
        cfg->encoder_cfg.disable_global_motion = (Data[tile_idx] % 2);
        tile_idx++;
        cfg->encoder_cfg.disable_palette = (Data[tile_idx] % 2);
        tile_idx++;
        cfg->encoder_cfg.disable_intrabc = (Data[tile_idx] % 2);
    }

    return cfg;
}

int LLVMFuzzerTestOneInput_121(const uint8_t *Data, size_t Size) {
    if (Size < 45) {
        return 0;
    }

    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    memset(&ctx, 0, sizeof(ctx));

    aom_codec_enc_cfg_t *cfg = get_fuzzed_config(Data, Size);
    if (!cfg) {
        return 0;
    }

    // Flags for initialization
    aom_codec_flags_t flags = 0;
    if (Size > 40) {
        flags = (aom_codec_flags_t)(Data[40] % 1024);
    }

    // Initialize encoder
    aom_codec_err_t init_err = aom_codec_enc_init_ver(&ctx, iface, cfg, flags, AOM_ENCODER_ABI_VERSION);
    
    if (init_err == AOM_CODEC_OK) {
        // Try to invoke control functions if encoder is initialized
        // We use aom_codec_control directly with the specific control IDs
        
        // AV1E_SET_ENABLE_WARPED_MOTION
        int warped_motion_val = (Data[41] % 2);
        aom_codec_control(&ctx, AV1E_SET_ENABLE_WARPED_MOTION, warped_motion_val);

        // AV1E_SET_LOOPFILTER_CONTROL
        int lf_val = (Data[42] % 100);
        aom_codec_control(&ctx, AV1E_SET_LOOPFILTER_CONTROL, lf_val);

        // AV1E_SET_COLOR_RANGE
        int color_range_val = (Data[43] % 2);
        aom_codec_control(&ctx, AV1E_SET_COLOR_RANGE, color_range_val);

        // AV1E_SET_ENABLE_SUPERRES
        int superres_val = (Data[44] % 2);
        aom_codec_control(&ctx, AV1E_SET_ENABLE_SUPERRES, superres_val);

        // Try to get error string for the last operation
        const char *err_str = aom_codec_err_to_string(ctx.err);
        (void)err_str;

        // Cleanup
        aom_codec_destroy(&ctx);
    } else {
        // If init fails, we still want to test the error reporting
        const char *err_str = aom_codec_err_to_string(init_err);
        (void)err_str;
    }

    free(cfg);
    return 0;
}