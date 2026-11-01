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
#include <stdio.h>

#include "aom/aom_decoder.h"
#include "aom/aom_encoder.h"
#include "aom/aom_codec.h"
#include "aom/aom_frame_buffer.h"
#include "aom/aom_external_partition.h"
#include "aom/aomcx.h"
#include "aom/aom_image.h"
#include "aom/aomdx.h"
#include "aom/aom_integer.h"
#include "aom/aom.h"

static int get_random_int(const uint8_t *data, size_t size, int min_val, int max_val) {
    if (size == 0) return min_val;
    int val = data[0] % (max_val - min_val + 1) + min_val;
    return val;
}

int LLVMFuzzerTestOneInput_163(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    aom_codec_ctx_t ctx;
    aom_codec_enc_cfg_t cfg;
    aom_codec_iface_t *iface;
    aom_codec_err_t res;
    aom_codec_flags_t flags = 0;

    // Get the encoder interface first
    iface = aom_codec_av1_cx();
    if (iface == NULL) return 0;

    // Initialize encoder configuration
    if (aom_codec_enc_config_default(iface, &cfg, 0) != AOM_CODEC_OK) {
        return 0;
    }

    cfg.g_w = 16;
    cfg.g_h = 16;
    cfg.g_timebase.num = 1;
    cfg.g_timebase.den = 30;
    cfg.rc_target_bitrate = 100;

    // Initialize the codec context
    res = aom_codec_enc_init(&ctx, iface, &cfg, flags);
    if (res != AOM_CODEC_OK) {
        return 0;
    }

    // Fuzz AV1E_SET_DISABLE_TRELLIS_QUANT
    int disable_trellis_quant = get_random_int(Data, Size, 0, 1);
    aom_codec_control(&ctx, AV1E_SET_DISABLE_TRELLIS_QUANT, disable_trellis_quant);

    // Fuzz AV1E_SET_ENABLE_RESTORATION
    int enable_restoration = get_random_int(Data, Size, 0, 1);
    aom_codec_control(&ctx, AV1E_SET_ENABLE_RESTORATION, enable_restoration);

    // Fuzz AV1E_SET_CDF_UPDATE_MODE
    int cdf_update_mode = get_random_int(Data, Size, 0, 2); // Assuming valid range is 0-2
    aom_codec_control(&ctx, AV1E_SET_CDF_UPDATE_MODE, cdf_update_mode);

    // Fuzz AV1E_SET_AQ_MODE
    int aq_mode = get_random_int(Data, Size, 0, 5); // Assuming valid range is 0-5
    aom_codec_control(&ctx, AV1E_SET_AQ_MODE, aq_mode);

    // Fuzz AV1E_SET_ENABLE_GLOBAL_MOTION
    int enable_global_motion = get_random_int(Data, Size, 0, 1);
    aom_codec_control(&ctx, AV1E_SET_ENABLE_GLOBAL_MOTION, enable_global_motion);

    // Fuzz AV1E_SET_DELTALF_MODE
    int delf_mode = get_random_int(Data, Size, 0, 3); // Assuming valid range is 0-3
    aom_codec_control(&ctx, AV1E_SET_DELTALF_MODE, delf_mode);

    // Cleanup
    aom_codec_destroy(&ctx);

    return 0;
}