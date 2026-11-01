// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aomdx.h>
#include <aom/aom_integer.h>
#include <aom/aom_image.h>
#include <aom/aom_decoder.h>
#include <aom/aom.h>
#include <aom/aomcx.h>
#include <aom/aom_external_partition.h>
#include <aom/aom_frame_buffer.h>

/*
 * Helper function to initialize encoder configuration with fuzzed data.
 * Returns 0 on success, -1 on failure.
 */
static int init_encoder_cfg(aom_codec_enc_cfg_t *cfg, const uint8_t *data, size_t size) {
    if (!cfg || !data || size < 2) {
        return -1;
    }

    memset(cfg, 0, sizeof(aom_codec_enc_cfg_t));

    // Parse some basic parameters from the fuzzed data to vary states
    unsigned int width = (data[0] % 32) + 16; // 16 to 47
    unsigned int height = (data[1] % 32) + 16; // 16 to 47

    cfg->g_w = width;
    cfg->g_h = height;
    cfg->g_bit_depth = AOM_BITS_8;
    cfg->g_input_bit_depth = 8;
    cfg->g_timebase.num = 1;
    cfg->g_timebase.den = (data[2] % 60) + 1; // 1 to 60
    cfg->g_threads = (data[3] % 8) + 1;
    cfg->g_profile = (data[4] % 4);
    cfg->g_error_resilient = 0;
    cfg->g_pass = AOM_RC_ONE_PASS;
    cfg->g_lag_in_frames = 0;
    cfg->g_limit = 0;

    // Rate control
    cfg->rc_end_usage = AOM_VBR;
    cfg->rc_target_bitrate = 1000;
    cfg->rc_min_quantizer = 0;
    cfg->rc_max_quantizer = 63;
    cfg->rc_buf_sz = 1000;
    cfg->rc_buf_initial_sz = 500;
    cfg->rc_buf_optimal_sz = 750;

    // Keyframing
    cfg->kf_mode = AOM_KF_DISABLED;
    cfg->kf_min_dist = 0;
    cfg->kf_max_dist = 0;

    // Tile settings
    cfg->tile_width_count = 0;
    cfg->tile_height_count = 0;

    // Config options
    cfg->encoder_cfg.super_block_size = 64;
    cfg->encoder_cfg.max_partition_size = 64;
    cfg->encoder_cfg.min_partition_size = 8;
    cfg->encoder_cfg.disable_cdef = 0;
    cfg->encoder_cfg.disable_lr = 0;
    cfg->encoder_cfg.disable_tx_64x64 = 0;
    cfg->encoder_cfg.disable_trellis_quant = 0;
    cfg->encoder_cfg.disable_ref_frame_mv = 0;
    cfg->encoder_cfg.disable_one_sided_comp = 0;

    return 0;
}

int LLVMFuzzerTestOneInput_234(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t res;
    int ver = AOM_ENCODER_ABI_VERSION;

    // Initialize configuration
    if (init_encoder_cfg(&cfg, Data, Size) != 0) {
        return 0;
    }

    // Get the AV1 encoder interface
    // aom_codec_av1_cx() takes no arguments. It returns the encoder interface.
    iface = aom_codec_av1_cx();
    if (!iface) {
        return 0;
    }

    // Initialize the encoder context
    res = aom_codec_enc_init_ver(&ctx, iface, &cfg, 0, ver);
    if (res != AOM_CODEC_OK) {
        // If init fails, no need to destroy
        return 0;
    }

    // Attempt to call the target control functions if the context is valid
    // We use a subset of data to determine which controls to try
    // This helps explore different code paths

    // 1. AV1E_SET_ENABLE_ONESIDED_COMP
    // The value is derived from the fuzzed data (0 or 1)
    int val_onesided = (Data[0] % 2);
    res = aom_codec_control(&ctx, AV1E_SET_ENABLE_ONESIDED_COMP, val_onesided);
    (void)res; // Ignore return for fuzzing stability, but it exercises the code

    // 2. AV1E_SET_DISABLE_TRELLIS_QUANT
    int val_trellis = (Data[1] % 2);
    res = aom_codec_control(&ctx, AV1E_SET_DISABLE_TRELLIS_QUANT, val_trellis);
    (void)res;

    // 3. AV1E_SET_ENABLE_REF_FRAME_MVS
    int val_refmv = (Data[2] % 2);
    res = aom_codec_control(&ctx, AV1E_SET_ENABLE_REF_FRAME_MVS, val_refmv);
    (void)res;

    // 4. AV1E_SET_ENABLE_TX64
    int val_tx64 = (Data[3] % 2);
    res = aom_codec_control(&ctx, AV1E_SET_ENABLE_TX64, val_tx64);
    (void)res;

    // 5. AV1E_SET_RTC_EXTERNAL_RC
    int val_rtce = (Data[4] % 2);
    res = aom_codec_control(&ctx, AV1E_SET_RTC_EXTERNAL_RC, val_rtce);
    (void)res;

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}