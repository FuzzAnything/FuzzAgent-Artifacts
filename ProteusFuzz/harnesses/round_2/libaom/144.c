// This fuzz driver is generated for library libaom, aiming to fuzz the following functions:
// aom_codec_av1_cx at av1_cx_iface.c:5218:20 in aomcx.h
// aom_codec_enc_config_default at aom_encoder.c:100:17 in aom_encoder.h
// aom_codec_enc_init_ver at aom_encoder.c:38:17 in aom_encoder.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_control at aom_codec.c:88:17 in aom_codec.h
// aom_codec_destroy at aom_codec.c:68:17 in aom_codec.h
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <aom/aom_codec.h>
#include <aom/aom_encoder.h>
#include <aom/aom_decoder.h>
#include <aom/aom_image.h>
#include <aom/aom_integer.h>
#include <aom/aomdx.h>
#include <aom/aomcx.h>

/*
 * Helper function to initialize an encoder context with a random configuration.
 * Returns 0 on success, -1 on failure.
 */
static int init_encoder_context(aom_codec_ctx_t *codec, aom_codec_enc_cfg_t *cfg,
                                const uint8_t *data, size_t size) {
    const aom_codec_iface_t *iface;
    aom_codec_err_t ret;

    // Use the standard AV1 encoder interface
    iface = aom_codec_av1_cx();
    if (!iface) return -1;

    ret = aom_codec_enc_config_default(iface, cfg, 0);
    if (ret != AOM_CODEC_OK) return -1;

    // Use fuzzing data to randomize configuration slightly
    if (size > 0) {
        cfg->g_w = (data[0] % 1024) + 16;
        cfg->g_h = (data[1] % 1024) + 16;
        cfg->g_timebase.num = (data[2] % 100) + 1;
        cfg->g_timebase.den = (data[3] % 1000) + 1;
        cfg->g_profile = data[4] % 4;
        cfg->rc_target_bitrate = (data[5] % 10000) + 100;
    }

    ret = aom_codec_enc_init(codec, iface, cfg, 0);
    if (ret != AOM_CODEC_OK) return -1;

    return 0;
}

int LLVMFuzzerTestOneInput_144(const uint8_t *Data, size_t Size) {
    aom_codec_ctx_t codec;
    aom_codec_enc_cfg_t cfg;
    aom_codec_err_t ret;
    int val;

    if (Size < 6) return 0;

    // Initialize encoder context
    if (init_encoder_context(&codec, &cfg, Data, Size) != 0) {
        return 0;
    }

    // Extract values from fuzzing data to drive controls
    val = (Data[0] % 2); // 0 or 1 for boolean flags
    int param_val = (int)(Data[1] % 100); // Small integer parameter

    // 1. AV1E_SET_ENABLE_INTERINTRA_WEDGE
    // Sets whether to enable interintra wedge partitioning
    ret = aom_codec_control(&codec, AV1E_SET_ENABLE_INTERINTRA_WEDGE, val);
    (void)ret; // Ignore return for fuzzing, but in production check for errors

    // 2. AV1E_SET_COLOR_PRIMARIES
    // Sets color primaries. Valid values are typically 0-9.
    int color_primaries = Data[2] % 10;
    ret = aom_codec_control(&codec, AV1E_SET_COLOR_PRIMARIES, color_primaries);
    (void)ret;

    // 3. AV1E_SET_ALLOW_WARPED_MOTION
    // Sets whether to allow warped motion.
    ret = aom_codec_control(&codec, AV1E_SET_ALLOW_WARPED_MOTION, val);
    (void)ret;

    // 4. AOME_SET_ENABLEAUTOBWDREF
    // Sets whether to enable auto backward reference.
    ret = aom_codec_control(&codec, AOME_SET_ENABLEAUTOBWDREF, val);
    (void)ret;

    // 5. AV1E_SET_COEFF_COST_UPD_FREQ
    // Sets coefficient cost update frequency.
    int freq = Data[3] % 1000;
    ret = aom_codec_control(&codec, AV1E_SET_COEFF_COST_UPD_FREQ, freq);
    (void)ret;

    // 6. AOME_USE_REFERENCE
    // Sets reference frame usage.
    int ref_frame = Data[4] % 8; // 0-7 for reference frames
    ret = aom_codec_control(&codec, AOME_USE_REFERENCE, ref_frame);
    (void)ret;

    // Cleanup: Destroy the codec context
    aom_codec_destroy(&codec);

    return 0;
}