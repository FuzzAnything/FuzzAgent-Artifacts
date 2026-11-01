// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_enc_config_set at aom_encoder.c:298:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <aom/aomdx.h>
#include <aom/aom_encoder.h>
#include <aom/aom_frame_buffer.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom.h>
#include <aom/aom_integer.h>
#include <aom/aom_codec.h>
#include <aom/aomcx.h>
#include <aom/aom_external_partition.h>

static void init_config_from_fuzz_data(aom_codec_enc_cfg_t *cfg, const uint8_t *Data, size_t Size) {
    if (!cfg || !Data || Size < 2) return;

    // Initialize with defaults first to ensure valid base state
    aom_codec_iface_t *iface = aom_codec_av1_cx();
    if (!iface) return;

    if (aom_codec_enc_config_default(iface, cfg, 0) != AOM_CODEC_OK) {
        return;
    }

    // Fuzz basic parameters based on input data
    size_t idx = 0;

    // Width and Height
    cfg->g_w = (Size > idx) ? Data[idx++] : 64;
    cfg->g_h = (Size > idx) ? Data[idx++] : 64;

    // Ensure minimum dimensions
    if (cfg->g_w < 8) cfg->g_w = 8;
    if (cfg->g_h < 8) cfg->g_h = 8;

    // Bit depth
    if (Size > idx) {
        int bd = Data[idx++] % 3;
        if (bd == 0) cfg->g_bit_depth = AOM_BITS_8;
        else if (bd == 1) cfg->g_bit_depth = AOM_BITS_10;
        else cfg->g_bit_depth = AOM_BITS_12;
    }

    // Input bit depth
    if (Size > idx) cfg->g_input_bit_depth = (Data[idx++] % 5) + 8; // 8 to 12

    // Timebase
    if (Size > idx) cfg->g_timebase.num = Data[idx++];
    if (Size > idx) cfg->g_timebase.den = (Data[idx++] % 100) + 1;

    // Usage
    if (Size > idx) cfg->g_usage = Data[idx++];

    // Threads
    if (Size > idx) cfg->g_threads = (Data[idx++] % 16) + 1;

    // Profile
    if (Size > idx) cfg->g_profile = Data[idx++] % 4;

    // Limit
    if (Size > idx) cfg->g_limit = (Data[idx++] % 100) + 1;

    // Rate Control
    if (Size > idx) cfg->rc_target_bitrate = (Data[idx++] % 1000) + 1;
    if (Size > idx) cfg->rc_min_quantizer = Data[idx++] % 64;
    if (Size > idx) cfg->rc_max_quantizer = Data[idx++] % 64;
    
    // RC Mode
    if (Size > idx) {
        int mode = Data[idx++] % 4;
        if (mode == 0) cfg->rc_end_usage = AOM_VBR;
        else if (mode == 1) cfg->rc_end_usage = AOM_CBR;
        else if (mode == 2) cfg->rc_end_usage = AOM_CQ;
        else cfg->rc_end_usage = AOM_Q;
    }

    // Keyframe settings
    if (Size > idx) cfg->kf_min_dist = Data[idx++];
    if (Size > idx) cfg->kf_max_dist = Data[idx++];

    // Tile settings
    if (Size > idx) cfg->tile_width_count = Data[idx++] % 8;
    if (Size > idx) cfg->tile_height_count = Data[idx++] % 8;

    // Encoder Config Options (cfg_options_t)
    cfg_options_t *opts = &cfg->encoder_cfg;
    if (Size > idx) opts->reduced_reference_set = Data[idx++];
    if (Size > idx) opts->disable_cfl = Data[idx++];
    if (Size > idx) opts->super_block_size = (Data[idx++] % 3) * 64; // 0, 64, 128
    if (Size > idx) opts->max_partition_size = (Data[idx++] % 5) * 8 + 8;
    if (Size > idx) opts->min_partition_size = (Data[idx++] % 5) * 8 + 8;
    if (Size > idx) opts->disable_cdef = Data[idx++];
    if (Size > idx) opts->disable_lr = Data[idx++];
    if (Size > idx) opts->disable_obmc = Data[idx++];
    if (Size > idx) opts->disable_warp_motion = Data[idx++];
    if (Size > idx) opts->disable_global_motion = Data[idx++];
    if (Size > idx) opts->disable_palette = Data[idx++];
    if (Size > idx) opts->disable_intrabc = Data[idx++];
    if (Size > idx) opts->disable_smooth_intra = Data[idx++];
    if (Size > idx) opts->disable_filter_intra = Data[idx++];
    if (Size > idx) opts->disable_dual_filter = Data[idx++];
    if (Size > idx) opts->disable_intra_angle_delta = Data[idx++];
    if (Size > idx) opts->disable_intra_edge_filter = Data[idx++];
    if (Size > idx) opts->disable_tx_64x64 = Data[idx++];
    if (Size > idx) opts->disable_smooth_inter_intra = Data[idx++];
    if (Size > idx) opts->disable_inter_inter_wedge = Data[idx++];
    if (Size > idx) opts->disable_inter_intra_wedge = Data[idx++];
    if (Size > idx) opts->disable_paeth_intra = Data[idx++];
    if (Size > idx) opts->disable_trellis_quant = Data[idx++];
    if (Size > idx) opts->disable_ref_frame_mv = Data[idx++];
    if (Size > idx) opts->reduced_tx_type_set = Data[idx++];
    if (Size > idx) opts->disable_ab_partition_type = Data[idx++];
    if (Size > idx) opts->disable_rect_partition_type = Data[idx++];
    if (Size > idx) opts->disable_1to4_partition_type = Data[idx++];
    if (Size > idx) opts->disable_flip_idtx = Data[idx++];
    if (Size > idx) opts->disable_dist_wtd_comp = Data[idx++];
    if (Size > idx) opts->disable_diff_wtd_comp = Data[idx++];
    if (Size > idx) opts->disable_inter_intra_comp = Data[idx++];
    if (Size > idx) opts->disable_masked_comp = Data[idx++];
    if (Size > idx) opts->disable_one_sided_comp = Data[idx++];
}

int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface = aom_codec_av1_cx();

    if (!iface) return 0;

    // Initialize config
    memset(&cfg, 0, sizeof(cfg));
    init_config_from_fuzz_data(&cfg, Data, Size);

    // Initialize context
    if (aom_codec_enc_init(&ctx, iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    // 1. Fuzz aom_codec_enc_config_set
    // We reuse the same config or modify it slightly to test reconfiguration
    aom_codec_err_t res = aom_codec_enc_config_set(&ctx, &cfg);
    (void)res; // Ignore return for fuzzing stability, just exercise the path

    // 2. Fuzz aom_codec_control_typechecked_AV1E_SET_REDUCED_REFERENCE_SET
    // Signature: aom_codec_control(ctx, AV1E_SET_REDUCED_REFERENCE_SET, value)
    // Value is 0 or 1 typically
    if (Size > 0) {
        int val = Data[0] & 1;
        aom_codec_control(&ctx, AV1E_SET_REDUCED_REFERENCE_SET, val);
    }

    // 3. Fuzz aom_codec_control_typechecked_AV1E_SET_COLOR_RANGE
    // Signature: aom_codec_control(ctx, AV1E_SET_COLOR_RANGE, value)
    // Values: 0 (LIMITED), 1 (FULL)
    if (Size > 1) {
        int val = Data[1] & 1;
        aom_codec_control(&ctx, AV1E_SET_COLOR_RANGE, val);
    }

    // 4. Fuzz aom_codec_control_typechecked_AV1E_SET_TILE_ROWS
    // Signature: aom_codec_control(ctx, AV1E_SET_TILE_ROWS, value)
    // Value is number of tile rows
    if (Size > 2) {
        int val = Data[2] % 8; // Limit to reasonable number
        aom_codec_control(&ctx, AV1E_SET_TILE_ROWS, val);
    }

    // 5. Fuzz aom_codec_control_typechecked_AV1E_SET_ENABLE_CFL_INTRA
    // Signature: aom_codec_control(ctx, AV1E_SET_ENABLE_CFL_INTRA, value)
    // Value is 0 or 1
    if (Size > 3) {
        int val = Data[3] & 1;
        aom_codec_control(&ctx, AV1E_SET_ENABLE_CFL_INTRA, val);
    }

    // 6. Fuzz aom_codec_control_typechecked_AV1E_SET_COLOR_PRIMARIES
    // Signature: aom_codec_control(ctx, AV1E_SET_COLOR_PRIMARIES, value)
    // Values: 1-10 typically (BT.709, BT.601, etc.)
    if (Size > 4) {
        int val = (Data[4] % 10) + 1;
        aom_codec_control(&ctx, AV1E_SET_COLOR_PRIMARIES, val);
    }

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}